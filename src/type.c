#include "type.h"
#include "tree.h"
#include "memory.h"

#include <stdbool.h>
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


enum type_width
type_width (struct type *type)
{
  if (type == TYPE_ERROR)
    return WIDTH_0;

  switch (type->kind)
    {
    case TYPE_VOID:
      return WIDTH_0;

    case TYPE_I8:
      return WIDTH_1;
    case TYPE_I16:
      return WIDTH_2;
    case TYPE_I32:
      return WIDTH_4;
    case TYPE_I64:
      return WIDTH_8;

    case TYPE_U8:
      return WIDTH_1;
    case TYPE_U16:
      return WIDTH_2;
    case TYPE_U32:
      return WIDTH_4;
    case TYPE_U64:
      return WIDTH_8;

    case TYPE_POINTER:
      return WIDTH_8;

    case TYPE_ARRAY:
      return WIDTH_0;
    case TYPE_FUNCTION:
      return WIDTH_0;

    default:
      return WIDTH_0;
    }
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


struct type *
type_create_pointer (struct type *base)
{
  struct type *result;

  result = type_create (TYPE_POINTER);

  result->d.pointer.base = base;

  return result;
}


struct type *
type_create_array (struct tree *size, struct type *base)
{
  struct type *result;

  result = type_create (TYPE_ARRAY);

  result->d.array.size = size;
  result->d.array.base = base;

  return result;
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


bool
type_cast_required (struct type *a, struct type *b)
{
  if (a == TYPE_ERROR || b == TYPE_ERROR)
    return false;

  if (a->kind != b->kind)
    return true;

  enum type_kind kind = a->kind;

  switch (kind)
    {
    case TYPE_POINTER:
      return type_cast_required (a->d.pointer.base, b->d.pointer.base);

    case TYPE_ARRAY:
      return type_cast_required (a->d.array.base, b->d.array.base);

    case TYPE_FUNCTION:
      {
        struct type *a_from1 = a->d.function.from1;
        struct type *b_from1 = b->d.function.from1;

        while (a_from1 && b_from1)
          {
            if (type_cast_required (a_from1, b_from1))
              return true;

            a_from1 = a_from1->next;
            b_from1 = b_from1->next;
          }

        if (a_from1 || b_from1)
          return true;
      }

      return type_cast_required (a->d.function.to, b->d.function.to);

    default:
      return false;
    }
}


bool
type_is_integer (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
      return true;
    default:
      return false;
    }
}


bool
type_is_integer_signed (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
      return true;
    default:
      return false;
    }
}


bool
type_is_pointer (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_POINTER:
      return true;
    default:
      return false;
    }
}


bool
type_is_pointer_to_k (struct type *type, enum type_kind kind)
{
  if (type == TYPE_ERROR)
    return false;

  if (!type_is_pointer (type))
    return false;

  return type->d.pointer.base->kind == kind;
  // return type->d.pointer.base->kind == kind;
}


bool
type_is_label (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_ARRAY:
    case TYPE_FUNCTION:
      return true;
    default:
      return false;
    }
}


bool
type_is_assignable (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_ARRAY:
    case TYPE_FUNCTION:
      return false;
    default:
      return true;
    }
}


bool
type_is_callable (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_FUNCTION:
      return true;
    default:
      return type_is_pointer_to_k (type, TYPE_FUNCTION);
    }
}


struct type *
type_decay (struct type *type)
{
  if (type == TYPE_ERROR)
    return type;

  switch (type->kind)
    {
    // []T -> *T
    case TYPE_ARRAY:
      return type_create_pointer (type->d.array.base);

    // fn -> *fn
    case TYPE_FUNCTION:
      return type_create_pointer (type);

    default:
      return type;
    }
}


struct type *
type_promote (struct type *a, struct type *b)
{
  enum type_width wa = type_width (a);
  enum type_width wb = type_width (b);

  bool sa = type_is_integer_signed (a);
  bool sb = type_is_integer_signed (b);

  bool ua = !sa;
  bool ub = !sb;

  enum type_kind ka = a->kind;
  enum type_kind kb = b->kind;

  if (ka == kb)
    return a;

  // Sign same; bigger wins.
  if (sa == sb)
    return wa > wb ? a : b;

  // Sign mixed
  if (ua && wa >= wb)
    return a;

  if (ub && wb >= wa)
    return b;

  if (sa && wa > wb)
    return a;

  if (sb && wb > wa)
    return b;

  return a;
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

  fprintf (stderr, "\033[90m%s\033[0m\n", type_kind_string (type->kind));

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

        tree_print (node.size, depth + 1);
        type_print (node.base, depth + 1);
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

