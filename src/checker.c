#include "checker.h"
#include "tree.h"
#include "type.h"
#include "memory.h"

#include <stdlib.h>


struct checker *
checker_create (void)
{
  struct checker *checker;

  checker = aa_malloc (sizeof (struct checker));

  return checker;
}


static void checker_check_node_fdefinition (struct checker *, struct tree *);


static void checker_check_node_if (struct checker *, struct tree *);

static void checker_check_node_while (struct checker *, struct tree *);

static void checker_check_node_for (struct checker *, struct tree *);

static void checker_check_node_compound (struct checker *, struct tree *);

static void checker_check_node_vdeclaration (struct checker *, struct tree *);

static void checker_check_node_return (struct checker *, struct tree *);

static void checker_check_node_print (struct checker *, struct tree *);


static void checker_check_node_cast (struct checker *, struct tree *);

static void checker_check_node_call (struct checker *, struct tree *);

static void checker_check_node_assignment (struct checker *, struct tree *);

static void checker_check_node_binary (struct checker *, struct tree *);

static void checker_check_node_reference (struct checker *, struct tree *);

static void checker_check_node_dereference (struct checker *, struct tree *);


static void checker_check_node_program (struct checker *, struct tree *);


static void
checker_check_type (struct checker *checker, struct type *type, struct location location)
{
  if (type == TYPE_ERROR)
    return;

  switch (type->kind)
    {
    case TYPE_ARRAY:
      {
        struct type_node_array *node = &type->d.array;

        if (node->size->d.integer.value == 0)
          {
            error (location, "zero sized 'array'");

            exit (1);
          }

        checker_check_type (checker, node->base, location);
      }
      break;
    default:
      break;
    }
}


static void
checker_check_expression (struct checker *checker, struct tree *tree)
{
  if (!tree)
    return;

  switch (tree->kind)
    {
    case TREE_CAST:
      checker_check_node_cast (checker, tree);
      break;
    case TREE_CALL:
      checker_check_node_call (checker, tree);
      break;
    case TREE_ASSIGNMENT:
      checker_check_node_assignment (checker, tree);
      break;
    case TREE_BINARY:
      checker_check_node_binary (checker, tree);
      break;
    case TREE_REFERENCE:
      checker_check_node_reference (checker, tree);
      break;
    case TREE_DEREFERENCE:
      checker_check_node_dereference (checker, tree);
      break;
    // case TREE_INTEGER:
    //   checker_check_node_integer (checker, tree);
    //   break;
    // case TREE_IDENTIFIER:
    //   checker_check_node_identifier (checker, tree);
    //   break;
    default:
      break;
    }
}


static void
checker_check_lvalue (struct checker *checker, struct tree *tree)
{
  if (!tree)
    return;

  if (tree_is_rvalue (tree))
    {
      error (tree->location, "right-value expression used as left-value");

      exit (1);
    }

  checker_check_expression (checker, tree);

  if (type_is_void (tree_type (tree)))
    {
      error (tree->location, "'void' expression used as left-value");

      exit (1);
    }
}


static void
checker_check_rvalue (struct checker *checker, struct tree *tree)
{
  if (!tree)
    return;

  checker_check_expression (checker, tree);

  if (type_is_void (tree_type (tree)))
    {
      error (tree->location, "'void' expression used as right-value");

      exit (1);
    }
}


static void
checker_check_statement (struct checker *checker, struct tree *tree)
{
  if (!tree)
    return;

  switch (tree->kind)
    {
    case TREE_FDEFINITION:
      checker_check_node_fdefinition (checker, tree);
      break;
    case TREE_IF:
      checker_check_node_if (checker, tree);
      break;
    case TREE_WHILE:
      checker_check_node_while (checker, tree);
      break;
    case TREE_FOR:
      checker_check_node_for (checker, tree);
      break;
    case TREE_COMPOUND:
      checker_check_node_compound (checker, tree);
      break;
    case TREE_VDECLARATION:
      checker_check_node_vdeclaration (checker, tree);
      break;
    case TREE_RETURN:
      checker_check_node_return (checker, tree);
      break;
    case TREE_PRINT:
      checker_check_node_print (checker, tree);
      break;
    case TREE_PROGRAM:
      checker_check_node_program (checker, tree);
      break;
    default:
      checker_check_expression (checker, tree);
      break;
    }
}


static void
checker_check_node_fdefinition (struct checker *checker, struct tree *tree)
{
  struct tree_node_fdefinition *node = &tree->d.fdefinition;

  checker_check_type (checker, node->type, tree->location);

  for (struct tree *t = node->parameter1; t; t = t->next)
    checker_check_statement (checker, t);

  checker_check_statement (checker, node->body);
}


static void
checker_check_node_if (struct checker *checker, struct tree *tree)
{
  struct tree_node_if *node = &tree->d.if_s;

  checker_check_rvalue (checker, node->condition);

  checker_check_statement (checker, node->branch_a);
  checker_check_statement (checker, node->branch_b);
}


static void
checker_check_node_while (struct checker *checker, struct tree *tree)
{
  struct tree_node_while *node = &tree->d.while_s;

  checker_check_rvalue (checker, node->condition);

  checker_check_statement (checker, node->body);
}


static void
checker_check_node_for (struct checker *checker, struct tree *tree)
{
  struct tree_node_for *node = &tree->d.for_s;

  checker_check_rvalue (checker, node->init);
  checker_check_rvalue (checker, node->condition);
  checker_check_rvalue (checker, node->increment);

  checker_check_statement (checker, node->body);
}


static void
checker_check_node_compound (struct checker *checker, struct tree *tree)
{
  struct tree_node_compound *node = &tree->d.compound;

  for (struct tree *t = node->statement1; t; t = t->next)
    checker_check_statement (checker, t);
}


static void
checker_check_node_vdeclaration (struct checker *checker, struct tree *tree)
{
  struct tree_node_vdeclaration *node = &tree->d.vdeclaration;

  checker_check_type (checker, node->type, tree->location);
}


static void
checker_check_node_return (struct checker *checker, struct tree *tree)
{
  struct tree_node_return *node = &tree->d.return_s;

  checker_check_rvalue (checker, node->value);
}


static void
checker_check_node_print (struct checker *checker, struct tree *tree)
{
  struct tree_node_print *node = &tree->d.print;

  checker_check_rvalue (checker, node->value);
}


static void
checker_check_node_cast (struct checker *checker, struct tree *tree)
{
  struct tree_node_cast *node = &tree->d.cast;

  checker_check_type (checker, node->type, tree->location);

  checker_check_rvalue (checker, node->value);

  if (!type_is_scalar (node->type))
    {
      const char *kind = type_kind_string (node->type->kind);

      error (tree->location, "cast to non-scalar type '%s'", kind);

      exit (1);
    }
}


static void
checker_check_node_call (struct checker *checker, struct tree *tree)
{
  struct tree_node_call *node = &tree->d.call;

  checker_check_type (checker, node->type, tree->location);

  struct type *type_callee = tree_type (node->callee);

  // NOTE: For now, only function pointers can be called.
  if (!type_is_pointer_to_k (type_callee, TYPE_FUNCTION))
    {
      const char *kind = type_kind_string (type_callee->kind);

      error (tree->location, "call to non-callable type '%s'", kind);

      exit (1);
    }

  for (struct tree *t = node->argument1; t; t = t->next)
    checker_check_rvalue (checker, t);
}


static void
checker_check_node_assignment (struct checker *checker, struct tree *tree)
{
  struct tree_node_assignment *node = &tree->d.assignment;

  checker_check_type (checker, node->type, tree->location);

  checker_check_lvalue (checker, node->lhs);

  struct type *type = tree_type (node->lhs);

  if (!type_is_assignable (type))
    {
      const char *name = type_kind_string (type->kind);

      error (tree->location, "assignment to non-assignable type '%s'", name);

      exit (1);
    }

  checker_check_rvalue (checker, node->rhs);
}


static void
checker_check_node_binary (struct checker *checker, struct tree *tree)
{
  struct tree_node_binary *node = &tree->d.binary;

  checker_check_type (checker, node->type, tree->location);

  checker_check_rvalue (checker, node->lhs);
  checker_check_rvalue (checker, node->rhs);

  struct type *type_a = tree_type (node->lhs);
  struct type *type_b = tree_type (node->rhs);

  enum binary_operator o = node->o;

  bool a_i = type_is_integer (type_a);
  bool b_i = type_is_integer (type_b);

  bool a_p = type_is_pointer (type_a);
  bool b_p = type_is_pointer (type_b);

  switch (o)
    {
    // Arithmetic
    case BINARY_ADD:
    case BINARY_SUB:
      if (a_i && b_i)
        return;

      if (a_i && b_p)
        return;

      if (a_p && b_i)
        return;

      if (o == BINARY_SUB)
        if (a_p && b_p)
          return;
      break;

    case BINARY_MUL:
    case BINARY_DIV:
      if (a_i && b_i)
        return;
      break;

    case BINARY_CMP_EQ:
    case BINARY_CMP_NE:
    case BINARY_CMP_L:
    case BINARY_CMP_G:
    case BINARY_CMP_LE:
    case BINARY_CMP_GE:
      if (a_i && b_i)
        return;

      if (a_i && b_p)
        return;

      if (a_p && b_i)
        return;

      if (a_p && b_p)
        return;
      break;

    default:
      break;
    }

  const char *name_operator = binary_operator_string (o);
  const char *name_a = type_kind_string (type_a->kind);
  const char *name_b = type_kind_string (type_b->kind);

  error (tree->location, "operator '%s' between '%s' and '%s' is not allowed", name_operator,
         name_a, name_b);

  exit (1);
}


static void
checker_check_node_reference (struct checker *checker, struct tree *tree)
{
  struct tree_node_reference *node = &tree->d.reference;

  checker_check_type (checker, node->type, tree->location);

  checker_check_lvalue (checker, node->value);
}


static void
checker_check_node_dereference (struct checker *checker, struct tree *tree)
{
  struct tree_node_dereference *node = &tree->d.dereference;

  checker_check_type (checker, node->type, tree->location);

  checker_check_rvalue (checker, node->value);

  struct type *type = tree_type (node->value);

  if (!type_is_pointer (type))
    {
      const char *name = type_kind_string (type->kind);

      error (tree->location, "dereference of non-dereferencable type '%s'", name);

      exit (1);
    }
}


// static void
// checker_check_node_integer (struct checker *checker, struct tree *tree)
// {
//   struct tree_node_integer *node = &tree->d.integer;
// }
// 
// 
// static void
// checker_check_node_identifier (struct checker *checker, struct tree *tree)
// {
//   struct tree_node_identifier *node = &tree->d.identifier;
// }


static void
checker_check_node_program (struct checker *checker, struct tree *tree)
{
  struct tree_node_program *node = &tree->d.program;

  for (struct tree *t = node->top_level1; t; t = t->next)
    checker_check_statement (checker, t);
}


void
checker_check (struct checker *checker, struct tree *tree)
{
  checker_check_statement (checker, tree);
}

