#ifndef TYPE_H
#define TYPE_H


#include "error.h"

#include <stddef.h>
#include <stdbool.h>


struct tree;


enum type_kind
{
  TYPE_VOID,

  TYPE_I8,
  TYPE_I16,
  TYPE_I32,
  TYPE_I64,

  TYPE_U8,
  TYPE_U16,
  TYPE_U32,
  TYPE_U64,

  TYPE_POINTER,
  TYPE_ARRAY,

  TYPE_STRUCT,
  TYPE_STRUCT_NAME,

  TYPE_UNION,
  TYPE_UNION_NAME,

  TYPE_FUNCTION,
};


const char *
type_kind_string (enum type_kind kind);


struct type_node_pointer
{
  struct type *base;
};


struct type_node_array
{
  // struct tree *size;

  size_t size;
  struct type *base;
};


struct type_node_struct
{
  char *name;

  struct type *field1;

  struct scope *scope;
};


// A reference to a struct type, which may or may not exist.
struct type_node_struct_name
{
  char *name;
};


struct type_node_union
{
  char *name;

  struct type *field1;

  struct scope *scope;
};


// A reference to a union type, which may or may not exist.
struct type_node_union_name
{
  char *name;
};


struct type_node_function
{
  struct type *from1;
  struct type *to;
};


union type_data
{
  struct type_node_pointer     pointer;
  struct type_node_array       array;

  struct type_node_struct      struct_;
  struct type_node_struct_name struct_name;

  struct type_node_union       union_;
  struct type_node_union_name  union_name;

  struct type_node_function    function;
};


struct type
{
  struct location location;

  union type_data d;

  struct type *next;

  enum type_kind kind;
};


enum type_width
{
  WIDTH_0 = 0,
  WIDTH_1 = 1,
  WIDTH_2 = 2,
  WIDTH_4 = 4,
  WIDTH_8 = 8,
};


enum type_width
type_width (struct type *type);

size_t
type_size (struct type *type);

size_t
type_element_size (struct type *type);

size_t
type_alignment (struct type *type);


void
type_string (struct type *type, char *buffer, size_t size);

struct type *
type_create (struct location location, enum type_kind kind);

struct type *
type_create_pointer (struct location location, struct type *base);

struct type *
type_create_array (struct location location, size_t size, struct type *base);

struct type *
type_shallow_copy (struct type *type);


void
type_append (struct type **head, struct type *node);


bool
type_cast_required (struct type *a, struct type *b);


enum type_flag
{
  TYPE_FLAG_VOID           = 1 << 0,
  TYPE_FLAG_INCOMPLETE     = 1 << 1,
  TYPE_FLAG_INTEGER        = 1 << 2,
  TYPE_FLAG_SIGNED         = 1 << 3,
  TYPE_FLAG_POINTER        = 1 << 4,
  TYPE_FLAG_LABEL          = 1 << 5,
  TYPE_FLAG_SCALAR         = 1 << 6,
  TYPE_FLAG_NAMED          = 1 << 7,
  TYPE_FLAG_COMPOSITE      = 1 << 8,
  TYPE_FLAG_NOT_ASSIGNABLE = 1 << 9,
  TYPE_FLAG_CALLABLE       = 1 << 10,
};


unsigned long long
type_trait (struct type *type);


bool
type_is_pointer_to_k (struct type *type, enum type_kind kind);

bool
type_is_callable (struct type *type);


#define type_is_void(type) (type_trait (type) & TYPE_FLAG_VOID)

#define type_is_incomplete(type) (type_trait (type) & TYPE_FLAG_INCOMPLETE)

#define type_is_integer(type) (type_trait (type) & TYPE_FLAG_INTEGER)

#define type_is_signed(type) (type_trait (type) & TYPE_FLAG_SIGNED)

#define type_is_pointer(type) (type_trait (type) & TYPE_FLAG_POINTER)

#define type_is_label(type) (type_trait (type) & TYPE_FLAG_LABEL)

#define type_is_scalar(type) (type_trait (type) & TYPE_FLAG_SCALAR)

#define type_is_named(type) (type_trait (type) & TYPE_FLAG_NAMED)

#define type_is_composite(type) (type_trait (type) & TYPE_FLAG_COMPOSITE)

#define type_is_assignable(type) (!(type_trait (type) & TYPE_FLAG_NOT_ASSIGNABLE))

#define type_is_not_assignable(type) (type_trait (type) & TYPE_FLAG_NOT_ASSIGNABLE)


struct type *
type_element (struct type *type);

struct type *
type_decay (struct type *type);

struct type *
type_find_common (struct type *a, struct type *b);


void
type_print (struct type *type, int depth);


#endif // TYPE_H

