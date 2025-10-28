#include "resolver.h"
#include "tree.h"
#include "type.h"
#include "scope.h"
#include "memory.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct resolver
{
  struct tree_node_fdefinition *function;

  struct scope *scope;
  struct scope *scope_struct;
};


static void
resolver_scope_push (struct resolver *resolver)
{
  resolver->scope = scope_create (resolver->scope, SCOPE_CAPACITY);
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
  resolver->scope_struct = scope_create (NULL, SCOPE_CAPACITY);

  resolver_scope_push (resolver);

  return resolver;
}


static struct type *resolver_resolve_type (struct resolver *, struct type *);

static struct tree *resolver_resolve_lvalue (struct resolver *, struct tree *);

static struct tree *resolver_resolve_rvalue (struct resolver *, struct tree *);


static struct tree *
resolver_tree_create_cast (struct tree *value, struct type *type_b)
{
  struct type *type_a = tree_type (value);

  if (type_cast_required (type_a, type_b))
    {
      struct tree *result;

      result = tree_create (value->location, TREE_CAST);

      result->d.cast.value = value;

      result->d.cast.type = type_b;

      result->next = value->next;

      value->next = NULL;

      return result;
    }

  return value;
}


static struct tree *
resolver_tree_create_scale (struct tree *value, struct type *type_base)
{
  struct tree *result;

  result = tree_create (value->location, TREE_SCALE);

  result->d.scale.value = value;

  result->d.scale.type = tree_type (value);

  result->d.scale.type_base = type_base;

  result->next = value->next;

  value->next = NULL;

  return result;
}


static void resolver_resolve_node_fdeclaration (struct resolver *, struct tree *);

static void resolver_resolve_node_fdefinition (struct resolver *, struct tree *);

static void resolver_resolve_node_struct (struct resolver *, struct tree *);


static void resolver_resolve_node_if (struct resolver *, struct tree *);

static void resolver_resolve_node_while (struct resolver *, struct tree *);

static void resolver_resolve_node_for (struct resolver *, struct tree *);

static void resolver_resolve_node_compound (struct resolver *, struct tree *);

static void resolver_resolve_node_vdeclaration (struct resolver *, struct tree *);

static void resolver_resolve_node_return (struct resolver *, struct tree *);

static void resolver_resolve_node_print (struct resolver *, struct tree *);


static struct tree *resolver_resolve_node_cast (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_call (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_assignment (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_access (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_binary (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_reference (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_dereference (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_integer (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_string (struct resolver *, struct tree *);

static struct tree *resolver_resolve_node_identifier (struct resolver *, struct tree *);


static void resolver_resolve_node_program (struct resolver *, struct tree *);


static struct type *
resolver_resolve_type (struct resolver *resolver, struct type *type)
{
  if (type == TYPE_ERROR)
    return type;

  switch (type->kind)
    {
    // case TYPE_POINTER:
    //   {
    //     struct type_node_pointer *node = &type->d.pointer;

    //     node->base = resolver_resolve_type (resolver, node->base);

    //     return type;
    //     // switch (node->base->kind)
    //     //   {
    //     //   case TYPE_STRUCT_NAME:
    //     //     return type;
    //     //   default:
    //     //     node->base = resolver_resolve_type (resolver, node->base);
    //     //     return type;
    //     //   }
    //   }

    case TYPE_ARRAY:
      {
        struct type_node_array *node = &type->d.array;

        node->base = resolver_resolve_type (resolver, node->base);

        return type;
      }

    case TYPE_FUNCTION:
      {
        struct type_node_function *node = &type->d.function;

        struct type *prev = NULL;
        struct type *curr = node->from1;

        while (curr)
          {
            struct type *next = resolver_resolve_type (resolver, curr);

            if (prev)
              prev->next = next;
            else
              node->from1 = next;

            prev = next;
            curr = next->next;
          }

        node->to = resolver_resolve_type (resolver, node->to);

        return type;
      }

    case TYPE_STRUCT:
      {
        struct type_node_struct *node = &type->d.struct_t;

        struct type *prev = NULL;
        struct type *curr = node->field1;

        while (curr)
          {
            struct type *next = resolver_resolve_type (resolver, curr);

            if (prev)
              prev->next = next;
            else
              node->field1 = next;

            prev = next;
            curr = next->next;
          }

        return type;
      }

    case TYPE_STRUCT_NAME:
      {
        struct type_node_struct_name *node = &type->d.struct_name;

        struct symbol symbol;

        // NOTE: Don't check for invalidity
        enum scope_get_result result = scope_get (resolver->scope_struct, node->name, &symbol);

        switch (result)
          {
          case SCOPE_GET_OK:
            return type_shallow_copy (symbol.type);

          default:
            return type;
          }
      }

    default:
      return type;
    }
}


struct tree *
resolver_resolve_expression (struct resolver *resolver, struct tree *tree)
{
  if (!tree)
    return NULL;

  switch (tree->kind)
    {
    case TREE_CAST:
      return resolver_resolve_node_cast (resolver, tree);
    case TREE_CALL:
      return resolver_resolve_node_call (resolver, tree);
    case TREE_ASSIGNMENT:
      return resolver_resolve_node_assignment (resolver, tree);
    case TREE_ACCESS:
      return resolver_resolve_node_access (resolver, tree);
    case TREE_BINARY:
      return resolver_resolve_node_binary (resolver, tree);
    case TREE_REFERENCE:
      return resolver_resolve_node_reference (resolver, tree);
    case TREE_DEREFERENCE:
      return resolver_resolve_node_dereference (resolver, tree);
    case TREE_INTEGER:
      return resolver_resolve_node_integer (resolver, tree);
    case TREE_STRING:
      return resolver_resolve_node_string (resolver, tree);
    case TREE_IDENTIFIER:
      return resolver_resolve_node_identifier (resolver, tree);
    default:
      return NULL;
    }
}


static struct tree *
resolver_resolve_lvalue (struct resolver *resolver, struct tree *tree)
{
  return resolver_resolve_expression (resolver, tree);
}


static struct tree *
resolver_resolve_rvalue (struct resolver *resolver, struct tree *tree)
{
  if (!tree)
    return NULL;

  struct tree *value = resolver_resolve_expression (resolver, tree);

  struct type *type = tree_type (value);

  if (type_is_label (type))
    {
      struct tree *result;

      result = tree_create (tree->location, TREE_REFERENCE);

      result->d.reference.value = value;

      result->d.reference.type = type_decay (type);

      result->next = value->next;

      value->next = NULL;

      return result;
    }

  return value;
}


static void
resolver_resolve_statement (struct resolver *resolver, struct tree *tree)
{
  if (!tree)
    return;

  switch (tree->kind)
    {
    case TREE_FDECLARATION:
      resolver_resolve_node_fdeclaration (resolver, tree);
      break;
    case TREE_FDEFINITION:
      resolver_resolve_node_fdefinition (resolver, tree);
      break;
    case TREE_STRUCT:
      resolver_resolve_node_struct (resolver, tree);
      break;
    case TREE_IF:
      resolver_resolve_node_if (resolver, tree);
      break;
    case TREE_WHILE:
      resolver_resolve_node_while (resolver, tree);
      break;
    case TREE_FOR:
      resolver_resolve_node_for (resolver, tree);
      break;
    case TREE_COMPOUND:
      resolver_resolve_node_compound (resolver, tree);
      break;
    case TREE_VDECLARATION:
      resolver_resolve_node_vdeclaration (resolver, tree);
      break;
    case TREE_RETURN:
      resolver_resolve_node_return (resolver, tree);
      break;
    case TREE_PRINT:
      resolver_resolve_node_print (resolver, tree);
      break;
    case TREE_PROGRAM:
      resolver_resolve_node_program (resolver, tree);
      break;

    default:
      resolver_resolve_expression (resolver, tree);
      break;
    }
}


static void
resolver_resolve_node_fdeclaration (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_fdeclaration *node = &tree->d.fdeclaration;

  node->type = resolver_resolve_type (resolver, node->type);

  struct symbol symbol;

  symbol.scope = SYMBOL_GLOBAL;
  symbol.name = node->name;
  symbol.type = node->type;

  scope_set_validate (resolver->scope, symbol, tree->location);
}


static void
resolver_resolve_node_fdefinition (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_fdefinition *node = &tree->d.fdefinition;

  assert (node->type != NULL);

  resolver->function = node;

  // NOTE: Set BEFORE resolve. Functions can refer to themselves.
  struct symbol symbol;

  symbol.scope = SYMBOL_GLOBAL;
  symbol.name = node->name;
  symbol.type = node->type;

  scope_set_validate (resolver->scope, symbol, tree->location);

  resolver_scope_push (resolver);

  struct type_node_function *type_node = &node->type->d.function;

  struct type *prev_t = type_node->from1;

  for (struct tree *t = node->parameter1; t; t = t->next)
    {
      resolver_resolve_statement (resolver, t);

      struct type *curr_t = tree_type (t);

      if (prev_t)
        prev_t->next = curr_t;
      else
        type_node->from1 = curr_t;

      prev_t = curr_t;
    }

  // NOTE: Resolve body iteratively to avoid scope creation.
  for (struct tree *t = node->body->d.compound.statement1; t; t = t->next)
    resolver_resolve_statement (resolver, t);

  resolver_scope_pop (resolver);

  node->type = resolver_resolve_type (resolver, node->type);
}


static void
resolver_resolve_node_struct (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_struct *node = &tree->d.struct_s;

  assert (node->type != NULL);

  struct type_node_struct *type_node = &node->type->d.struct_t;

  struct scope *save = resolver->scope;

  resolver->scope = type_node->scope = scope_create (NULL, SCOPE_CAPACITY);

  struct type *prev_t = type_node->field1;

  for (struct tree *t = node->field1; t; t = t->next)
    {
      resolver_resolve_statement (resolver, t);

      struct type *curr_t = tree_type (t);

      if (prev_t)
        prev_t->next = curr_t;
      else
        type_node->field1 = curr_t;

      prev_t = curr_t;
    }

  // Compute field offsets.
  // TODO: Hacky. Resolver should probably not do this.
  size_t offset = 0;

  size_t i = 0;

  for (struct tree *t = node->field1; t; t = t->next, ++i)
    {
      assert (i < type_node->scope->size);

      struct symbol *symbol = &type_node->scope->data[i];

      size_t t_alignment = type_alignment (symbol->type);
      size_t t_size = type_size (symbol->type);

      symbol->scope = SYMBOL_FIELD;

      offset = (offset + t_alignment - 1) & ~(t_alignment - 1);

      symbol->d.field.offset = offset;

      offset += t_size;
    }

  resolver->scope = save;

  node->type = resolver_resolve_type (resolver, node->type);

  // NOTE: Set AFTER resolve. Structures cannot refer to themselves.
  struct symbol symbol;

  symbol.name = node->name;
  symbol.type = node->type;

  scope_set_validate (resolver->scope_struct, symbol, tree->location);

  type_node->name = node->name;
}


static void
resolver_resolve_node_if (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_if *node = &tree->d.if_s;

  node->condition = resolver_resolve_rvalue (resolver, node->condition);

  node->condition = resolver_tree_create_cast (node->condition, type_create (tree->location, TYPE_U8));

  resolver_resolve_statement (resolver, node->branch_a);
  resolver_resolve_statement (resolver, node->branch_b);
}


static void
resolver_resolve_node_while (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_while *node = &tree->d.while_s;

  node->condition = resolver_resolve_rvalue (resolver, node->condition);

  node->condition = resolver_tree_create_cast (node->condition, type_create (tree->location, TYPE_U8));

  resolver_resolve_statement (resolver, node->body);
}


static void
resolver_resolve_node_for (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_for *node = &tree->d.for_s;

  node->init = resolver_resolve_rvalue (resolver, node->init);

  node->condition = resolver_resolve_rvalue (resolver, node->condition);

  node->condition = resolver_tree_create_cast (node->condition, type_create (tree->location, TYPE_U8));

  node->increment = resolver_resolve_rvalue (resolver, node->increment);

  resolver_resolve_statement (resolver, node->body);
}


static void
resolver_resolve_node_compound (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_compound *node = &tree->d.compound;

  resolver_scope_push (resolver);

  for (struct tree *t = node->statement1; t; t = t->next)
    resolver_resolve_statement (resolver, t);

  resolver_scope_pop (resolver);
}


static void
resolver_resolve_node_vdeclaration (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_vdeclaration *node = &tree->d.vdeclaration;

  node->type = resolver_resolve_type (resolver, node->type);

  struct symbol symbol;

  symbol.scope = SYMBOL_LOCAL;
  symbol.name = node->name;
  symbol.type = node->type;

  scope_set_validate (resolver->scope, symbol, tree->location);
}


static void
resolver_resolve_node_return (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_return *node = &tree->d.return_s;

  struct type_node_function type_function = resolver->function->type->d.function;

  node->value = resolver_resolve_rvalue (resolver, node->value);

  if (node->value && type_function.to->kind != TYPE_VOID)
    {
      node->value = resolver_tree_create_cast (node->value, type_function.to);
    }
}


static void
resolver_resolve_node_print (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_print *node = &tree->d.print;

  node->value = resolver_resolve_rvalue (resolver, node->value);

  node->value = resolver_tree_create_cast (node->value, type_create (tree->location, TYPE_I64));
}


static struct tree *
resolver_resolve_node_cast (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_cast *node = &tree->d.cast;

  node->type = resolver_resolve_type (resolver, node->type);

  node->value = resolver_resolve_rvalue (resolver, node->value);

  if (!type_cast_required (node->type, tree_type (node->value)))
    return node->value;

  return tree;
}


static struct tree *
resolver_resolve_node_call (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_call *node = &tree->d.call;

  node->callee = resolver_resolve_rvalue (resolver, node->callee);

  struct type *type_callee = tree_type (node->callee);

  // NOTE: For now, only function pointers can be called.
  if (!type_is_pointer_to_k (type_callee, TYPE_FUNCTION))
    return tree;

  struct type_node_function type_function = type_callee->d.pointer.base->d.function;

  struct tree *p = NULL;

  struct tree *a = node->argument1;
  struct type *b = type_function.from1;

  size_t p_n = 0;

  while (a && b)
    {
      struct tree *n;

      n = resolver_resolve_rvalue (resolver, a);

      // First 6 arguments are casted to their expected type
      if (p_n < 6)
        n = resolver_tree_create_cast (n, b);

      // Rest of the arguments must be 64-bit, so they're correctly placed on the stack
      else
        n = resolver_tree_create_cast (n, type_create (tree->location, TYPE_I64));

      if (p)
        p->next = n;
      else
        node->argument1 = n;

      p = n;

      a = n->next;
      b = b->next;
      p_n++;
    }

  if (a)
    {
      error (tree->location, "too many arguments to function");
      exit (1);
    }

  if (b)
    {
      error (tree->location, "too few arguments to function");
      exit (1);
    }

  node->type = resolver_resolve_type (resolver, type_function.to);

  return tree;
}


static struct tree *
resolver_resolve_node_assignment (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_assignment *node = &tree->d.assignment;

  if (tree_is_rvalue (node->lhs))
    return tree;

  node->lhs = resolver_resolve_lvalue (resolver, node->lhs);
  node->rhs = resolver_resolve_rvalue (resolver, node->rhs);

  struct type *type = tree_type (node->lhs);

  if (type_is_assignable (type))
    {
      node->rhs = resolver_tree_create_cast (node->rhs, type);
      node->type = resolver_resolve_type (resolver, type);
    }

  return tree;
}


static struct tree *
resolver_resolve_node_access (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_access *node = &tree->d.access;

  if (tree_is_rvalue (node->s))
    return tree;

  node->s = resolver_resolve_lvalue (resolver, node->s);

  // if (type_is_pointer_to_k (tree_type (node->s), TYPE_STRUCT_NAME))
  //   {
  //     struct tree *deref;

  //     deref = tree_create (tree->location, TREE_DEREFERENCE);

  //     deref->next = node->s->next;

  //     node->s->next = NULL;

  //     deref->d.dereference.value = node->s;

  //     node->s = resolver_resolve_lvalue (resolver, deref);
  //   }

  struct type *type = tree_type (node->s);

  if (type_is_composite (type))
    {
      struct type_node_struct type_node = type->d.struct_t;

      struct symbol symbol;

      scope_get_validate (type_node.scope, node->field, &symbol, tree->location);

      node->type = resolver_resolve_type (resolver, symbol.type);
    }

  return tree;
}


static struct tree *
resolver_resolve_node_binary (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_binary *node = &tree->d.binary;

  node->lhs = resolver_resolve_rvalue (resolver, node->lhs);
  node->rhs = resolver_resolve_rvalue (resolver, node->rhs);

  struct type *type_a = tree_type (node->lhs);
  struct type *type_b = tree_type (node->rhs);

  enum binary_operator o = node->o;

  bool a_i = type_is_integer (type_a);
  bool b_i = type_is_integer (type_b);

  bool a_p = type_is_pointer (type_a);
  bool b_p = type_is_pointer (type_b);

  switch (o)
    {
    case BINARY_ADD:
    case BINARY_SUB:
      if (a_i && b_i)
        {
          struct type *common = type_promote (type_a, type_b);

          node->lhs = resolver_tree_create_cast (node->lhs, common);
          node->rhs = resolver_tree_create_cast (node->rhs, common);

          node->type = resolver_resolve_type (resolver, common);

          return tree;
        }

      if (a_i && b_p)
        {
          node->lhs = resolver_tree_create_cast (node->lhs, type_create (tree->location, TYPE_I64));

          node->type = resolver_resolve_type (resolver, type_b);

          struct type *type_base = resolver_resolve_type (resolver, type_element (node->type));

          node->lhs = resolver_tree_create_scale (node->lhs, type_base);

          return tree;
        }

      if (a_p && b_i)
        {
          node->rhs = resolver_tree_create_cast (node->rhs, type_create (tree->location, TYPE_I64));

          node->type = resolver_resolve_type (resolver, type_a);

          struct type *type_base = resolver_resolve_type (resolver, type_element (node->type));

          node->rhs = resolver_tree_create_scale (node->rhs, type_base);

          return tree;
        }

      if (o == BINARY_SUB)
        if (a_p && b_p)
          {
            node->type = type_create (tree->location, TYPE_I64);

            return tree;
          }

      break;

    case BINARY_MUL:
    case BINARY_DIV:
    case BINARY_MOD:
      if (a_i && b_i)
        {
          struct type *common = type_promote (type_a, type_b);

          node->lhs = resolver_tree_create_cast (node->lhs, common);
          node->rhs = resolver_tree_create_cast (node->rhs, common);

          node->type = resolver_resolve_type (resolver, common);

          return tree;
        }

      break;

    case BINARY_CMP_EQ:
    case BINARY_CMP_NE:
    case BINARY_CMP_L:
    case BINARY_CMP_G:
    case BINARY_CMP_LE:
    case BINARY_CMP_GE:
      if (a_i && b_i)
        {
          struct type *common = type_promote (type_a, type_b);

          node->lhs = resolver_tree_create_cast (node->lhs, common);
          node->rhs = resolver_tree_create_cast (node->rhs, common);

          node->type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (a_i && b_p)
        {
          struct type *common = type_create (tree->location, TYPE_I64);

          node->lhs = resolver_tree_create_cast (node->lhs, common);
          node->rhs = resolver_tree_create_cast (node->rhs, common);

          node->type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (a_p && b_i)
        {
          struct type *common = type_create (tree->location, TYPE_I64);

          node->lhs = resolver_tree_create_cast (node->lhs, common);
          node->rhs = resolver_tree_create_cast (node->rhs, common);

          node->type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (a_p && b_p)
        {
          node->type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      break;

    case BINARY_LOR:
    case BINARY_LAND:
      {
        struct type *common = type_promote (type_a, type_b);

        node->lhs = resolver_tree_create_cast (node->lhs, common);
        node->rhs = resolver_tree_create_cast (node->rhs, common);

        node->type = type_create (tree->location, TYPE_U8);

        return tree;
      }
      break;

    default:
      break;
    }

  return tree;
}


static struct tree *
resolver_resolve_node_reference (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_reference *node = &tree->d.reference;

  if (tree_is_rvalue (node->value))
    return tree;

  node->value = resolver_resolve_lvalue (resolver, node->value);

  node->type = type_create_pointer (tree->location, tree_type (node->value));

  return tree;
}


static struct tree *
resolver_resolve_node_dereference (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_dereference *node = &tree->d.dereference;

  node->value = resolver_resolve_rvalue (resolver, node->value);

  struct type *type = tree_type (node->value);

  if (type_is_pointer (type))
    {
      node->type = resolver_resolve_type (resolver, type->d.pointer.base);
    }

  return tree;
}


static struct tree *
resolver_resolve_node_integer (struct resolver *resolver, struct tree *tree)
{
  (void)resolver;

  struct tree_node_integer *node = &tree->d.integer;

  node->type = type_create (tree->location, TYPE_I64);

  return tree;
}


static struct tree *
resolver_resolve_node_string (struct resolver *resolver, struct tree *tree)
{
  (void)resolver;

  struct tree_node_string *node = &tree->d.string;

  node->type = type_create_pointer (tree->location, type_create (tree->location, TYPE_U8));

  return tree;
}


static struct tree *
resolver_resolve_node_identifier (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_identifier *node = &tree->d.identifier;

  struct symbol symbol;

  scope_get_validate (resolver->scope, node->value, &symbol, tree->location);

  node->type = resolver_resolve_type (resolver, symbol.type);

  return tree;
}


static void
resolver_resolve_node_program (struct resolver *resolver, struct tree *tree)
{
  struct tree_node_program *node = &tree->d.program;

  for (struct tree *t = node->top_level1; t; t = t->next)
    resolver_resolve_statement (resolver, t);
}


void
resolver_resolve (struct resolver *resolver, struct tree *tree)
{
  resolver_resolve_statement (resolver, tree);
}

