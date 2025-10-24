#include "tree.h"
#include "type.h"
#include "memory.h"

#include <stdio.h>


static const char *const BINARY_OPERATOR_STRING[] = {
  "+",
  "-",
  "*",
  "/",
  "==",
  "!=",
  "<",
  ">",
  "<=",
  ">=",
};


const char *
binary_operator_string (enum binary_operator operator)
{
  return BINARY_OPERATOR_STRING[operator];
}


static const char *const TREE_KIND_STRING[] = {
  "fdefinition",
  "empty",
  "if",
  "while",
  "for",
  "compound",
  "vdeclaration",
  "return",
  "print",
  "cast",
  "call",
  "assignment",
  "binary",
  "reference",
  "dereference",
  "integer",
  "identifier",
  "program",
};


const char *
tree_kind_string (enum tree_kind kind)
{
  return TREE_KIND_STRING[kind];
}


struct tree *
tree_create (struct location location, enum tree_kind kind)
{
  struct tree *tree;

  tree = aa_malloc (sizeof (struct tree));

  tree->location = location;

  tree->next = NULL;

  tree->kind = kind;

  return tree;
}


void
tree_append (struct tree **head, struct tree *node)
{
  if (*head == NULL)
    {
      *head = node;
      return;
    }

  struct tree *current = *head;

  while (current->next != NULL)
    {
      current = current->next;
    }

  current->next = node;
}


struct type *
tree_type (struct tree *tree)
{
  switch (tree->kind)
    {
    case TREE_FDEFINITION:
      return tree->d.fdefinition.type;
    case TREE_VDECLARATION:
      return tree->d.vdeclaration.type;
    case TREE_CAST:
      return tree->d.cast.type;
    case TREE_CALL:
      return tree->d.call.type;
    case TREE_ASSIGNMENT:
      return tree->d.assignment.type;
    case TREE_BINARY:
      return tree->d.binary.type;
    case TREE_REFERENCE:
      return tree->d.reference.type;
    case TREE_DEREFERENCE:
      return tree->d.dereference.type;
    case TREE_INTEGER:
      return tree->d.integer.type;
    case TREE_IDENTIFIER:
      return tree->d.identifier.type;
    default:
      return NULL;
    }
}


bool
tree_is_lvalue (struct tree *tree)
{
  switch (tree->kind)
    {
    case TREE_IDENTIFIER:
    case TREE_DEREFERENCE:
      return true;
    default:
      return false;
    }
}


bool
tree_is_rvalue (struct tree *tree)
{
  return !tree_is_lvalue (tree);
}


static void
tree_print_indent (int depth)
{
  for (int i = 0; i < depth; ++i)
    fprintf (stderr, "    ");
}


void
tree_print (struct tree *tree, int depth)
{
  tree_print_indent (depth);

  if (!tree)
    {
      fprintf (stderr, "\033[90mundefined\033[0m\n");
      return;
    }

  fprintf (stderr, "%s\n", tree_kind_string (tree->kind));

  switch (tree->kind)
    {
    case TREE_FDEFINITION:
      {
        struct tree_node_fdefinition node = tree->d.fdefinition;

        type_print (node.type, depth + 1);

        tree_print_indent (depth + 1);

        fprintf (stderr, "\033[91m%s\033[0m\n", node.name);

        for (struct tree *t = node.parameter1; t; t = t->next)
          tree_print (t, depth + 1);

        tree_print (node.body, depth + 1);
      }
      break;
    case TREE_EMPTY:
      break;
    case TREE_IF:
      {
        struct tree_node_if node = tree->d.if_s;

        tree_print (node.condition, depth + 1);
        tree_print (node.branch_a, depth + 1);
        tree_print (node.branch_b, depth + 1);
      }
      break;
    case TREE_WHILE:
      {
        struct tree_node_while node = tree->d.while_s;

        tree_print (node.condition, depth + 1);
        tree_print (node.body, depth + 1);
      }
      break;
    case TREE_FOR:
      {
        struct tree_node_for node = tree->d.for_s;

        tree_print (node.init, depth + 1);
        tree_print (node.condition, depth + 1);
        tree_print (node.increment, depth + 1);
        tree_print (node.body, depth + 1);
      }
      break;
    case TREE_COMPOUND:
      {
        struct tree_node_compound node = tree->d.compound;

        for (struct tree *t = node.statement1; t; t = t->next)
          tree_print (t, depth + 1);
      }
      break;
    case TREE_VDECLARATION:
      {
        struct tree_node_vdeclaration node = tree->d.vdeclaration;

        type_print (node.type, depth + 1);

        tree_print_indent (depth + 1);

        fprintf (stderr, "\033[91m%s\033[0m\n", node.name);
      }
      break;
    case TREE_RETURN:
      {
        struct tree_node_return node = tree->d.return_s;

        tree_print (node.value, depth + 1);
      }
      break;
    case TREE_PRINT:
      {
        struct tree_node_print node = tree->d.print;

        tree_print (node.value, depth + 1);
      }
      break;
    case TREE_CAST:
      {
        struct tree_node_cast node = tree->d.cast;

        type_print (node.type, depth + 1);

        tree_print (node.value, depth + 1);
      }
      break;
    case TREE_CALL:
      {
        struct tree_node_call node = tree->d.call;

        type_print (node.type, depth + 1);

        tree_print (node.callee, depth + 1);

        for (struct tree *t = node.argument1; t; t = t->next)
          tree_print (t, depth + 1);
      }
      break;
    case TREE_ASSIGNMENT:
      {
        struct tree_node_assignment node = tree->d.assignment;

        type_print (node.type, depth + 1);

        tree_print (node.lhs, depth + 1);
        tree_print (node.rhs, depth + 1);
      }
      break;
    case TREE_BINARY:
      {
        struct tree_node_binary node = tree->d.binary;

        type_print (node.type, depth + 1);

        tree_print_indent (depth + 1);

        fprintf (stderr, "%s\n", binary_operator_string (node.o));

        tree_print (node.lhs, depth + 1);
        tree_print (node.rhs, depth + 1);
      }
      break;
    case TREE_REFERENCE:
      {
        struct tree_node_reference node = tree->d.reference;

        type_print (node.type, depth + 1);

        tree_print (node.value, depth + 1);
      }
      break;
    case TREE_DEREFERENCE:
      {
        struct tree_node_dereference node = tree->d.dereference;

        type_print (node.type, depth + 1);

        tree_print (node.value, depth + 1);
      }
      break;
    case TREE_INTEGER:
      {
        struct tree_node_integer node = tree->d.integer;

        type_print (node.type, depth + 1);

        tree_print_indent (depth + 1);

        fprintf (stderr, "\033[38;5;100m%ld\033[0m\n", node.value);
      }
      break;
    case TREE_IDENTIFIER:
      {
        struct tree_node_identifier node = tree->d.identifier;

        type_print (node.type, depth + 1);

        tree_print_indent (depth + 1);

        fprintf (stderr, "\033[91m%s\033[0m\n", node.value);
      }
      break;
    case TREE_PROGRAM:
      {
        struct tree_node_program node = tree->d.program;

        for (struct tree *t = node.top_level1; t; t = t->next)
          tree_print (t, depth + 1);
      }
      break;
    }
}

