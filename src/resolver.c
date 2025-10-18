#include "resolver.h"
#include "scope.h"
#include "memory.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>


static void
resolver_scope_push (struct resolver *resolver)
{
  resolver->scope = scope_create (resolver->scope, 128);
}


static void
resolver_scope_pop (struct resolver *resolver)
{
  resolver->scope = resolver->scope->parent;
}


struct resolver *
resolver_create (void)
{
  struct resolver *resolver;

  resolver = aa_malloc (sizeof (struct resolver));

  resolver->scope = NULL;

  resolver_scope_push (resolver);

  return resolver;
}


static struct tree *
resolver_resolve_lvalue (struct resolver *resolver, struct tree *tree)
{
  return resolver_resolve (resolver, tree);
}


static struct tree *
resolver_resolve_rvalue (struct resolver *resolver, struct tree *tree)
{
  tree = resolver_resolve (resolver, tree);

  if (type_is_label (tree->type))
    {
      // TODO: Use tree_wrap. (It doesn't support root == target, yet.)
      struct tree *reference;

      reference = tree_create (tree->location, TREE_REFERENCE);

      tree_set_type (reference, type_decay (tree->type));

      tree_append (reference, tree);

      struct tree *next = tree->next;

      tree->next = NULL;

      tree = reference;

      tree->next = next;
    }

  return tree;
}


static void
resolver_resolve_all (struct resolver *resolver, struct tree *tree,
                      struct tree *(*resolve_function) (struct resolver *, struct tree *))
{
  struct tree *prev = NULL;
  struct tree *curr = tree->child;

  while (curr)
    {
      struct tree *new = resolve_function (resolver, curr);

      if (prev)
        prev->next = new;
      else
        tree->child = new;

      prev = new;
      curr = new->next;
    }
}


static struct tree *resolver_resolve_if (struct resolver *, struct tree *);

static struct tree *resolver_resolve_while (struct resolver *, struct tree *);

static struct tree *resolver_resolve_for (struct resolver *, struct tree *);

static struct tree *resolver_resolve_compound (struct resolver *, struct tree *);

static struct tree *resolver_resolve_variable_declaration (struct resolver *, struct tree *);

static struct tree *resolver_resolve_print (struct resolver *, struct tree *);


static struct tree *resolver_resolve_cast (struct resolver *, struct tree *);

static struct tree *resolver_resolve_assignment (struct resolver *, struct tree *);

static struct tree *resolver_resolve_binary (struct resolver *, struct tree *);

static struct tree *resolver_resolve_reference (struct resolver *, struct tree *);

static struct tree *resolver_resolve_dereference (struct resolver *, struct tree *);

static struct tree *resolver_resolve_integer (struct resolver *, struct tree *);

static struct tree *resolver_resolve_identifier (struct resolver *, struct tree *);


static struct tree *resolver_resolve_program (struct resolver *, struct tree *);


static struct tree *
resolver_resolve_if (struct resolver *resolver, struct tree *tree)
{
  resolver_resolve_all (resolver, tree, resolver_resolve_rvalue);

  struct tree *node_cond = tree->child;

  tree_wrap_cast_p_if (tree, node_cond, TYPE_U8);

  return tree;
}


static struct tree *
resolver_resolve_while (struct resolver *resolver, struct tree *tree)
{
  resolver_resolve_all (resolver, tree, resolver_resolve_rvalue);

  struct tree *node_cond = tree->child;

  tree_wrap_cast_p_if (tree, node_cond, TYPE_U8);

  return tree;
}


static struct tree *
resolver_resolve_for (struct resolver *resolver, struct tree *tree)
{
  resolver_resolve_all (resolver, tree, resolver_resolve_rvalue);

  struct tree *node_cond = tree->child->next;

  tree_wrap_cast_p_if (tree, node_cond, TYPE_U8);

  return tree;
}


static struct tree *
resolver_resolve_compound (struct resolver *resolver, struct tree *tree)
{
  resolver_scope_push (resolver);

  resolver_resolve_all (resolver, tree, resolver_resolve_lvalue);

  resolver_scope_pop (resolver);

  return tree;
}


static struct tree *
resolver_resolve_variable_declaration (struct resolver *resolver, struct tree *tree)
{
  const char *key = tree->child->token->data.s;

  struct symbol symbol;

  symbol.key = key;
  symbol.type = tree->type;

  enum scope_set_result result = scope_set (resolver->scope, symbol);

  switch (result)
    {
    case SCOPE_SET_OK:
      break;

    case SCOPE_SET_REDEFINED:
      error (tree->location, "redefined %s", key);
      exit (1);
    }

  return tree;
}


static struct tree *
resolver_resolve_print (struct resolver *resolver, struct tree *tree)
{
  tree->child = resolver_resolve_rvalue (resolver, tree->child);

  tree_wrap_cast_p_if (tree, tree->child, TYPE_I64);

  return tree;
}


static struct tree *
resolver_resolve_cast (struct resolver *resolver, struct tree *tree)
{
  tree->child = resolver_resolve_rvalue (resolver, tree->child);

  return tree;
}


static struct tree *
resolver_resolve_assignment (struct resolver *resolver, struct tree *tree)
{
  struct tree *node_a = tree->child;
  struct tree *node_b = tree->child->next;

  node_a = tree->child = resolver_resolve_lvalue (resolver, node_a);
  node_b = tree->child->next = resolver_resolve_rvalue (resolver, node_b);

  struct tree *type = node_a->type;

  if (type_is_assignable (type) && tree_is_left_value (node_a))
    {
      tree_set_type (tree, type);

      tree_wrap_cast (tree, node_b, type);
    }

  return tree;
}


static struct tree *
resolver_resolve_binary (struct resolver *resolver, struct tree *tree)
{
  tree->child = resolver_resolve_rvalue (resolver, tree->child);
  tree->child->next = resolver_resolve_rvalue (resolver, tree->child->next);

  struct tree *node_a;
  struct tree *node_b;

  node_a = tree->child;
  node_b = tree->child->next;

  struct tree *type_a = node_a->type;
  struct tree *type_b = node_b->type;

  enum token_kind o = tree->token->kind;

  bool ai = type_is_integer (type_a);
  bool bi = type_is_integer (type_b);

  bool ap = type_is_pointer (type_a);
  bool bp = type_is_pointer (type_b);

  switch (o)
    {
    // Relational
    case TOKEN_DEQUAL:
    case TOKEN_NEQUAL:
    case TOKEN_LT:
    case TOKEN_GT:
    case TOKEN_LTEQUAL:
    case TOKEN_GTEQUAL:
      if (ai && bi)
        {
          enum type_kind common = type_promote_integer (type_a, type_b);

          tree_wrap_cast_p_if (tree, node_a, common);
          tree_wrap_cast_p_if (tree, node_b, common);

          tree_set_type (tree, type_create (tree->location, TYPE_U8));

          return tree;
        }

      if (ai && bp)
        {
          tree_wrap_cast (tree, node_a, type_b);

          tree_set_type (tree, type_create (tree->location, TYPE_U8));

          return tree;
        }

      if (ap && bi)
        {
          tree_wrap_cast (tree, node_b, type_a);

          tree_set_type (tree, type_create (tree->location, TYPE_U8));

          return tree;
        }

      if (ap && bp)
        {
          tree_set_type (tree, type_create (tree->location, TYPE_U8));

          return tree;
        }
      break;

    // Arithmetic
    case TOKEN_PLUS:
    case TOKEN_MINUS:
      if (ai && bi)
        {
          enum type_kind common = type_promote_integer (type_a, type_b);

          tree_wrap_cast_p_if (tree, node_a, common);
          tree_wrap_cast_p_if (tree, node_b, common);

          tree_set_type (tree, type_create (tree->location, common));

          return tree;
        }

      if (ai && bp)
        {
          tree_wrap_cast_p_if (tree, node_a, TYPE_I64);

          tree_set_type (tree, type_b);

          return tree;
        }

      if (ap && bi)
        {
          tree_wrap_cast_p_if (tree, node_b, TYPE_I64);

          tree_set_type (tree, type_a);

          return tree;
        }

      if (o == TOKEN_MINUS)
        if (ap && bp)
          {
            tree_set_type (tree, type_create (tree->location, TYPE_I64));

            return tree;
          }
      break;

    case TOKEN_STAR:
    case TOKEN_SLASH:
      if (ai && bi)
        {
          enum type_kind common = type_promote_integer (type_a, type_b);

          tree_wrap_cast_p_if (tree, node_a, common);
          tree_wrap_cast_p_if (tree, node_b, common);

          tree_set_type (tree, type_create (tree->location, common));

          return tree;
        }
      break;

    default:
      break;
    }

  return tree;
}


static struct tree *
resolver_resolve_reference (struct resolver *resolver, struct tree *tree)
{
  tree->child = resolver_resolve_lvalue (resolver, tree->child);

  struct tree *type = tree->child->type;

  if (tree_is_left_value (tree->child))
    tree_set_type (tree, type_reference (type));

  return tree;
}


static struct tree *
resolver_resolve_dereference (struct resolver *resolver, struct tree *tree)
{
  tree->child = resolver_resolve_rvalue (resolver, tree->child);

  struct tree *type = tree->child->type;

  if (type_is_pointer (type))
    tree_set_type (tree, type_dereference (type));

  return tree;
}


static struct tree *
resolver_resolve_integer (struct resolver *resolver, struct tree *tree)
{
  (void)resolver;

  tree_set_type (tree, type_create (tree->location, TYPE_I64));

  return tree;
}


static struct tree *
resolver_resolve_identifier (struct resolver *resolver, struct tree *tree)
{
  const char *key = tree->token->data.s;

  struct symbol symbol;

  enum scope_get_result result = scope_get (resolver->scope, key, &symbol);

  switch (result)
    {
    case SCOPE_GET_OK:
      tree_set_type (tree, symbol.type);
      break;

    case SCOPE_GET_UNDEFINED:
      error (tree->location, "undefined %s", key);
      exit (1);
    }

  return tree;
}


static struct tree *
resolver_resolve_program (struct resolver *resolver, struct tree *tree)
{
  resolver_resolve_all (resolver, tree, resolver_resolve_lvalue);

  return tree;
}


struct tree *
resolver_resolve (struct resolver *resolver, struct tree *tree)
{
  switch (tree->tree_kind)
    {
    case TREE_IF:
      return resolver_resolve_if (resolver, tree);
    case TREE_WHILE:
      return resolver_resolve_while (resolver, tree);
    case TREE_FOR:
      return resolver_resolve_for (resolver, tree);
    case TREE_COMPOUND:
      return resolver_resolve_compound (resolver, tree);
    case TREE_VARIABLE_DECLARATION:
      return resolver_resolve_variable_declaration (resolver, tree);
    case TREE_PRINT:
      return resolver_resolve_print (resolver, tree);
    case TREE_CAST:
      return resolver_resolve_cast (resolver, tree);
    case TREE_ASSIGNMENT:
      return resolver_resolve_assignment (resolver, tree);
    case TREE_BINARY:
      return resolver_resolve_binary (resolver, tree);
    case TREE_REFERENCE:
      return resolver_resolve_reference (resolver, tree);
    case TREE_DEREFERENCE:
      return resolver_resolve_dereference (resolver, tree);
    case TREE_INTEGER:
      return resolver_resolve_integer (resolver, tree);
    case TREE_IDENTIFIER:
      return resolver_resolve_identifier (resolver, tree);
    case TREE_PROGRAM:
      return resolver_resolve_program (resolver, tree);
    default:
      return tree;
    }
}

