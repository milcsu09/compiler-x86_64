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

  "struct",
  "struct_name",

  "fn",
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

    default:
      return WIDTH_0;
    }
}


size_t
type_size (struct type *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->kind)
    {
    case TYPE_VOID:
      return 0;

    case TYPE_I8:
      return 1;
    case TYPE_I16:
      return 2;
    case TYPE_I32:
      return 4;
    case TYPE_I64:
      return 8;

    case TYPE_U8:
      return 1;
    case TYPE_U16:
      return 2;
    case TYPE_U32:
      return 4;
    case TYPE_U64:
      return 8;

    case TYPE_POINTER:
      return 8;

    case TYPE_ARRAY:
      {
        struct type_node_array *node = &type->d.array;

        size_t n = node->size;
        size_t s = type_size (node->base);

        return n * s;
      }

    case TYPE_STRUCT:
      {
        struct type_node_struct *node = &type->d.struct_t;

        size_t offset = 0;
        size_t alignment = 0;

        for (struct type *t = node->field1; t; t = t->next)
          {
            size_t t_alignment = type_alignment (t);
            size_t t_size = type_size (t);

            if (t_alignment > alignment)
                alignment = t_alignment;

            offset = (offset + t_alignment - 1) & ~(t_alignment - 1);

            offset += t_size;
          }

        offset = (offset + alignment - 1) & ~(alignment - 1);

        return offset;
      }

    default:
      return 0;
    }
}


size_t
type_element_size (struct type *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->kind)
    {
    case TYPE_POINTER:
      fprintf (stderr, "ELEMENT SIZE of %s = %zu\n", type_kind_string (type->d.pointer.base->kind),
               type_size (type->d.pointer.base));

      return type_size (type->d.pointer.base);
    case TYPE_ARRAY:
      return type_size (type->d.array.base);
    default:
      return type_size (type);
    }
}


size_t
type_alignment (struct type *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->kind)
    {
    case TYPE_VOID:
      return 0;

    case TYPE_I8:
      return 1;
    case TYPE_I16:
      return 2;
    case TYPE_I32:
      return 4;
    case TYPE_I64:
      return 8;

    case TYPE_U8:
      return 1;
    case TYPE_U16:
      return 2;
    case TYPE_U32:
      return 4;
    case TYPE_U64:
      return 8;

    case TYPE_POINTER:
      return 8;

    case TYPE_ARRAY:
      return type_alignment (type->d.array.base);
    case TYPE_STRUCT:
      {
        struct type_node_struct node = type->d.struct_t;

        size_t alignment = 0;

        for (struct type *t = node.field1; t; t = t->next)
          {
            size_t t_alignment = type_alignment (t);

            if (t_alignment > alignment)
              alignment = t_alignment;
          }

        return alignment;
      }

    default:
      return 0;
    }
}


void
type_string (struct type *type, char *buffer, size_t size)
{
  if (!type || size == 0)
    return;

  switch (type->kind)
    {
    case TYPE_POINTER:
      {
        int n = snprintf (buffer, size, "*");

        if (n < 0 || (size_t)n >= size)
          return;

        type_string (type->d.pointer.base, buffer + n, size - n);
      }
      break;

    case TYPE_ARRAY:
      {
        int n = snprintf (buffer, size, "[%zu]", type->d.array.size);

        if (n < 0 || (size_t)n >= size)
          return;

        type_string (type->d.array.base, buffer + n, size - n);
      }
      break;

    case TYPE_STRUCT:
      snprintf (buffer, size, "struct %s", type->d.struct_t.name);
      break;

    case TYPE_STRUCT_NAME:
      snprintf (buffer, size, "struct %s", type->d.struct_name.name);
      break;

    default:
      snprintf (buffer, size, "%s", type_kind_string (type->kind));
      break;
    }
}

struct type *
type_create (struct location location, enum type_kind kind)
{
  struct type *type;

  type = aa_malloc (sizeof (struct type));

  type->location = location;

  type->next = NULL;

  type->kind = kind;

  return type;
}


struct type *
type_create_pointer (struct location location, struct type *base)
{
  struct type *result;

  result = type_create (location, TYPE_POINTER);

  result->d.pointer.base = base;

  return result;
}


struct type *
type_create_array (struct location location, size_t size, struct type *base)
{
  struct type *result;

  result = type_create (location, TYPE_ARRAY);

  result->d.array.size = size;
  result->d.array.base = base;

  return result;
}


struct type *
type_shallow_copy (struct type *type)
{
  struct type *copy;

  copy = type_create (type->location, type->kind);

  // IMPORTANT!
  copy->next = NULL;

  copy->d = type->d;

  return copy;
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
type_is_incomplete (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_VOID:
    case TYPE_STRUCT_NAME:
      return true;
    default:
      return false;
    }
}


bool
type_is_void (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_VOID:
      return true;
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
type_is_scalar (struct type *type)
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
    case TYPE_POINTER:
      return true;
    default:
      return false;
    }
}


bool
type_is_composite (struct type *type)
{
  if (type == TYPE_ERROR)
    return false;

  switch (type->kind)
    {
    case TYPE_STRUCT:
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
    case TYPE_STRUCT:
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
type_element (struct type *type)
{
  if (type == TYPE_ERROR)
    return type;

  switch (type->kind)
    {
    case TYPE_POINTER:
      return type->d.pointer.base;
    case TYPE_ARRAY:
      return type->d.array.base;
    default:
      return type;
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
      return type_create_pointer (type->d.array.base->location, type->d.array.base);

    // fn -> *fn
    case TYPE_FUNCTION:
      return type_create_pointer (type->location, type);

    default:
      return type;
    }
}


struct type *
type_find_common (struct type *a, struct type *b)
{
  if (a == TYPE_ERROR)
    return NULL;

  if (b == TYPE_ERROR)
    return NULL;

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
      fprintf (stderr, "\033[38;5;242mundefined\033[0m\n");
      return;
    }

  fprintf (stderr, "\033[38;5;242m%s\033[0m\n", type_kind_string (type->kind));

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

        type_print_indent (depth + 1);
        fprintf (stderr, "\033[38;5;100m%zu\033[0m\n", node.size);

        type_print (node.base, depth + 1);
      }
      break;
    case TYPE_STRUCT:
      {
        struct type_node_struct node = type->d.struct_t;

        type_print_indent (depth + 1);
        fprintf (stderr, "\033[91m%s\033[0m\n", node.name);

        for (struct type *t = node.field1; t; t = t->next)
          type_print (t, depth + 1);
      }
      break;
    case TYPE_STRUCT_NAME:
      {
        struct type_node_struct_name node = type->d.struct_name;

        type_print_indent (depth + 1);
        fprintf (stderr, "\033[91m%s\033[0m\n", node.name);
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

