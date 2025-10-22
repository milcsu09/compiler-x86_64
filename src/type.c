#include "type.h"
#include "tree.h"
#include "memory.h"

#include <stdio.h>


static const char *const TYPE_KIND_STRING[] = {
  "void",

  "i8",
  "i16",
  "i32",
  "i64",

  "u8",
  "u16",
  "u32",
  "u64",

  "pointer",
  "array",

  "function",
};


const char *
type_kind_string (enum type_kind kind)
{
  return TYPE_KIND_STRING[kind];
}


struct type *
type_create (enum type_kind kind)
{
  struct type *type;

  type = aa_malloc (sizeof (struct type));

  type->next = NULL;

  type->kind = kind;

  return type;
}


void
type_append (struct type **head, struct type *node)
{
  if (*head == NULL)
    {
      *head = node;
      return;
    }

  struct type *current = *head;

  while (current->next != NULL)
    {
      current = current->next;
    }

  current->next = node;
}


static void
type_print_indent (int depth)
{
  for (int i = 0; i < depth; ++i)
    fprintf (stderr, "    ");
}


void
type_print (struct type *type, int depth)
{
  type_print_indent (depth);

  if (!type)
    {
      fprintf (stderr, "\033[90mundefined\033[0m\n");
      return;
    }

  fprintf (stderr, "\033[93m%s\033[0m\n", type_kind_string (type->kind));

  switch (type->kind)
    {
    case TYPE_POINTER:
      {
        struct type_node_pointer node = type->d.pointer;

        type_print (node.base, depth + 1);
      }
      break;
    case TYPE_ARRAY:
      {
        struct type_node_array node = type->d.array;

        type_print (node.base, depth + 1);
        tree_print (node.size, depth + 1);
      }
      break;
    case TYPE_FUNCTION:
      {
        struct type_node_function node = type->d.function;

        for (struct type *t = node.from1; t; t = t->next)
          type_print (t, depth + 1);

        type_print (node.to, depth + 1);
      }
      break;
    default:
      break;
    }
}

