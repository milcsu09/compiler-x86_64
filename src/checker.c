#include "checker.h"
#include "memory.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>


struct checker *
checker_create (void)
{
  struct checker *checker;

  checker = aa_malloc (sizeof (struct checker));

  return checker;
}


static void checker_check_function_definition (struct checker *, struct tree *);

static void checker_check_if (struct checker *, struct tree *);

static void checker_check_while (struct checker *, struct tree *);

static void checker_check_for (struct checker *, struct tree *);

static void checker_check_compound (struct checker *, struct tree *);

static void checker_check_variable_declaration (struct checker *, struct tree *);

static void checker_check_print (struct checker *, struct tree *);


static void checker_check_cast (struct checker *, struct tree *);

static void checker_check_assignment (struct checker *, struct tree *);

static void checker_check_binary (struct checker *, struct tree *);

static void checker_check_reference (struct checker *, struct tree *);

static void checker_check_dereference (struct checker *, struct tree *);


static void checker_check_type (struct checker *, struct tree *);


static void checker_check_program (struct checker *, struct tree *);


static void
checker_check_function_definition (struct checker *checker, struct tree *tree)
{
  for (struct tree *current = tree->child; current; current = current->next)
    checker_check (checker, current);
}


static void
checker_check_if (struct checker *checker, struct tree *tree)
{
  struct tree *node_cond = tree->child;
  struct tree *node_then = tree->child->next;
  struct tree *node_else = tree->child->next->next;

  checker_check (checker, node_cond);
  checker_check (checker, node_then);

  if (node_else)
    checker_check (checker, node_else);
}


static void
checker_check_while (struct checker *checker, struct tree *tree)
{
  struct tree *node_cond = tree->child;
  struct tree *node_then = tree->child->next;

  checker_check (checker, node_cond);
  checker_check (checker, node_then);
}


static void
checker_check_for (struct checker *checker, struct tree *tree)
{
  struct tree *node_a    = tree->child;
  struct tree *node_b    = tree->child->next;
  struct tree *node_c    = tree->child->next->next;
  struct tree *node_body = tree->child->next->next->next;

  checker_check (checker, node_a);
  checker_check (checker, node_b);
  checker_check (checker, node_c);
  checker_check (checker, node_body);
}


static void
checker_check_compound (struct checker *checker, struct tree *tree)
{
  for (struct tree *current = tree->child; current; current = current->next)
    checker_check (checker, current);
}


static void
checker_check_variable_declaration (struct checker *checker, struct tree *tree)
{
  checker_check_type (checker, tree->type);
}


static void
checker_check_print (struct checker *checker, struct tree *tree)
{
  checker_check (checker, tree->child);
}


static void
checker_check_cast (struct checker *checker, struct tree *tree)
{
  struct tree *type = tree->type;

  checker_check_type (checker, type);

  if (!type_is_scalar (type))
    {
      const char *name = type_kind_string (type->type_kind);

      error (tree->location, "cast to non-scalar type '%s' is invalid", name);

      exit (1);
    }

  checker_check (checker, tree->child);
}


static void
checker_check_assignment (struct checker *checker, struct tree *tree)
{
  struct tree *node_a = tree->child;
  struct tree *node_b = tree->child->next;

  checker_check (checker, node_a);
  checker_check (checker, node_b);

  struct tree *type = node_a->type;

  checker_check_type (checker, type);

  if (!type_is_assignable (type))
    {
      const char *name = type_kind_string (type->type_kind);

      error (node_a->location, "expression with type '%s' is not assignable", name);

      exit (1);
    }

  if (!tree_is_left_value (node_a))
    {
      error (node_a->location, "expression is not assignable");

      exit (1);
    }
}


static void
checker_check_binary (struct checker *checker, struct tree *tree)
{
  struct tree *node_a;
  struct tree *node_b;

  node_a = tree->child;
  node_b = tree->child->next;

  checker_check (checker, node_a);
  checker_check (checker, node_b);

  struct tree *type_a = node_a->type;
  struct tree *type_b = node_b->type;

  checker_check_type (checker, type_a);
  checker_check_type (checker, type_b);

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
        return;

      if (ai && bp)
        return;

      if (ap && bi)
        return;

      if (ap && bp)
        return;
      break;

    // Arithmetic
    case TOKEN_PLUS:
    case TOKEN_MINUS:
      if (ai && bi)
        return;

      if (ai && bp)
        return;

      if (ap && bi)
        return;

      if (o == TOKEN_MINUS)
        if (ap && bp)
          return;
      break;

    case TOKEN_STAR:
    case TOKEN_SLASH:
      if (ai && bi)
        return;
      break;

    default:
      break;
    }

  const char *name_operator = token_kind_string (o);
  const char *name_a = type_kind_string (type_a->type_kind);
  const char *name_b = type_kind_string (type_b->type_kind);

  error (tree->token->location, "operator %s between type '%s' and type '%s' is invalid",
         name_operator, name_a, name_b);

  exit (1);
}


static void
checker_check_reference (struct checker *checker, struct tree *tree)
{
  checker_check (checker, tree->child);
  checker_check_type  (checker, tree->type);

  if (!tree_is_left_value (tree->child))
    {
      error (tree->child->location, "expression is not addressable");

      exit (1);
    }
}


static void
checker_check_dereference (struct checker *checker, struct tree *tree)
{
  checker_check (checker, tree->child);
  checker_check_type (checker, tree->type);

  struct tree *type = tree->child->type;

  if (!type_is_pointer (type))
    {
      error (tree->child->location, "expression is not dereferenceable");

      exit (1);
    }
}


static void
checker_check_type (struct checker *checker, struct tree *type)
{
  if (type == TYPE_ERROR)
    return;

  if (!type_is_complete (type))
    {
      const char *name = type_kind_string (type->type_kind);

      error (type->location, "type '%s' is incomplete", name);

      exit (1);
    }

  switch (type->type_kind)
    {
    case TYPE_ARRAY:
      if (type->child->token->data.i <= 0)
        {
          error (type->location, "zero-sized 'array' is invalid");

          exit (1);
        }
      checker_check_type (checker, type->type);
      break;
    default:
      break;
    }
}


static void
checker_check_program (struct checker *checker, struct tree *tree)
{
  for (struct tree *current = tree->child; current; current = current->next)
    checker_check (checker, current);
}


void
checker_check (struct checker *checker, struct tree *tree)
{
  switch (tree->tree_kind)
    {
    case TREE_FUNCTION_DEFINITION:
      checker_check_function_definition (checker, tree);
      break;
    case TREE_IF:
      checker_check_if (checker, tree);
      break;
    case TREE_WHILE:
      checker_check_while (checker, tree);
      break;
    case TREE_FOR:
      checker_check_for (checker, tree);
      break;
    case TREE_COMPOUND:
      checker_check_compound (checker, tree);
      break;
    case TREE_VARIABLE_DECLARATION:
      checker_check_variable_declaration (checker, tree);
      break;
    case TREE_PRINT:
      checker_check_print (checker, tree);
      break;
    case TREE_CAST:
      checker_check_cast (checker, tree);
      break;
    case TREE_ASSIGNMENT:
      checker_check_assignment (checker, tree);
      break;
    case TREE_BINARY:
      checker_check_binary (checker, tree);
      break;
    case TREE_REFERENCE:
      checker_check_reference (checker, tree);
      break;
    case TREE_DEREFERENCE:
      checker_check_dereference (checker, tree);
      break;
    case TREE_PROGRAM:
      checker_check_program (checker, tree);
      break;
    default:
      break;
    }
}

