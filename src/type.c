#include "tree.h"
#include "type.h"

#include <assert.h>
#include <stddef.h>


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
type_kind_width (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_I8:
      return WIDTH_8;
    case TYPE_I16:
      return WIDTH_16;
    case TYPE_I32:
      return WIDTH_32;
    case TYPE_I64:
      return WIDTH_64;
    case TYPE_U8:
      return WIDTH_8;
    case TYPE_U16:
      return WIDTH_16;
    case TYPE_U32:
      return WIDTH_32;
    case TYPE_U64:
      return WIDTH_64;
    case TYPE_POINTER:
    case TYPE_ARRAY:
      return WIDTH_64;
    default:
      return 0;
    }
}


bool
type_kind_is_assignable (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_ARRAY:
      return false;
    default:
      return true;
    }
}


bool
type_kind_is_label (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_ARRAY:
      return true;
    default:
      return false;
    }
}


bool
type_kind_is_integer (enum type_kind kind)
{
  switch (kind)
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
type_kind_is_signed (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
      return true;
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
      return false;
    default:
      return false;
    }
}


bool
type_kind_is_pointer (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_POINTER:
    case TYPE_ARRAY:
      return true;
    default:
      return false;
    }
}


bool
type_kind_is_array (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_ARRAY:
      return true;
    default:
      return false;
    }
}


bool
type_kind_is_scalar (enum type_kind kind)
{
  switch (kind)
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
type_kind_is_complete (enum type_kind kind)
{
  switch (kind)
    {
    case TYPE_VOID:
      return false;
    default:
      return true;
    }
}


enum type_kind
type_kind_promote_integer (enum type_kind ka, enum type_kind kb)
{
  enum type_width wa = type_kind_width (ka);
  enum type_width wb = type_kind_width (kb);

  bool sa = type_kind_is_signed (ka);
  bool sb = type_kind_is_signed (kb);

  bool ua = !sa;
  bool ub = !sb;

  // Promote small integers to 32-bits.
  // if (wa < WIDTH_32)
  //   {
  //     ka = sa ? TYPE_I32 : TYPE_U32;
  //     wa = WIDTH_32;
  //   }

  // if (wb < WIDTH_32)
  //   {
  //     kb = sb ? TYPE_I32 : TYPE_U32;
  //     wb = WIDTH_32;
  //   }

  if (ka == kb)
    return ka;

  // Sign same; bigger wins.
  if (sa == sb)
    return wa > wb ? ka : kb;

  // Sign mixed
  if (ua && wa >= wb)
    return ka;

  if (ub && wb >= wa)
    return kb;

  if (sa && wa > wb)
    return ka;

  if (sb && wb > wa)
    return kb;

  return ka;
}


struct tree *
type_create (struct location location, enum type_kind type_kind)
{
  struct tree *type;

  type = tree_create (location, TREE_TYPE);

  type->type_kind = type_kind;

  return type;
}


struct tree *
type_create_pointer (struct location location, struct tree *type)
{
  struct tree *pointer;

  pointer = type_create (location, TYPE_POINTER);

  pointer->type = type;

  // tree_append (pointer, type);

  return pointer;
}


struct tree *
type_create_array (struct location location, struct tree *type, struct tree *n)
{
  struct tree *array;

  array = type_create (location, TYPE_ARRAY);

  array->type = type;

  // tree_append (pointer, type);
  tree_append (array, n);

  return array;
}


bool
type_match (struct tree *type, enum type_kind kind)
{
  if (type == TYPE_ERROR)
    return false;

  return type->type_kind == kind;
}


size_t
type_size (struct tree *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->type_kind)
    {
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
        struct tree *node_a = type->child;

        size_t w = type_size (type->type);
        size_t n = node_a->token->data.i;

        return w * n;
      }
    default:
      return 0;
    }
}


size_t
type_alignment (struct tree *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->type_kind)
    {
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
      return type_alignment (type->type);
    default:
      return 0;
    }
}


size_t
type_element_size (struct tree *type)
{
  if (type == TYPE_ERROR)
    return 0;

  switch (type->type_kind)
    {
    case TYPE_POINTER:
      return type_size (type->type);
    case TYPE_ARRAY:
      return type_size (type->type);
    default:
      return type_size (type);
    }
}


enum type_width
type_width (struct tree *type)
{
  if (type == TYPE_ERROR)
    return 0;

  return type_kind_width (type->type_kind);
}


bool
type_is_assignable (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_assignable (type->type_kind);
}


bool
type_is_label (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_label (type->type_kind);
}


bool
type_is_integer (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_integer (type->type_kind);
}


bool
type_is_signed (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_signed (type->type_kind);
}


bool
type_is_pointer (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_pointer (type->type_kind);
}


bool
type_is_array (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_array (type->type_kind);
}


bool
type_is_scalar (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_scalar (type->type_kind);
}


bool
type_is_complete (struct tree *type)
{
  if (type == TYPE_ERROR)
    return false;

  return type_kind_is_complete (type->type_kind);
}


struct tree *
type_decay (struct tree *type)
{
  if (type == TYPE_ERROR)
    return type;

  switch (type->type_kind)
    {
    case TYPE_ARRAY:
      {
        struct tree *pointer = type_create_pointer (type->location, type->type);

        return pointer;
      }
    default:
      return type;
    }
}


struct tree *
type_reference (struct tree *type)
{
  if (type == TYPE_ERROR)
    return type;

  return type_create_pointer (type->location, type);

  // switch (type->type_kind)
  //   {
  //   case TYPE_ARRAY:
  //     // A reference to an array, is a pointer to it's underlying type.
  //     return type_create_pointer (type->location, type->type);
  //   default:
  //     return type_create_pointer (type->location, type);
  //   }
}


struct tree *
type_dereference (struct tree *type)
{
  if (type == TYPE_ERROR)
    return type;

  // NOTE: Assume it's valid.
  return type->type;
}


enum type_kind
type_promote_integer (struct tree *a, struct tree *b)
{
  enum type_kind ka = a->type_kind;
  enum type_kind kb = b->type_kind;

  return type_kind_promote_integer (ka, kb);
}

