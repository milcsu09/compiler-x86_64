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


struct type_node_function
{
  struct type *from1;
  struct type *to;
};


union type_data
{
  struct type_node_pointer pointer;
  struct type_node_array array;
  struct type_node_struct struct_;
  struct type_node_struct_name struct_name;
  struct type_node_function function;
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


#define TYPE_ERROR NULL


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


bool
type_is_incomplete (struct type *type);

bool
type_is_void (struct type *type);

bool
type_is_integer (struct type *type);

bool
type_is_integer_signed (struct type *type);

bool
type_is_pointer (struct type *type);

bool
type_is_pointer_to_k (struct type *type, enum type_kind kind);

bool
type_is_label (struct type *type);

bool
type_is_scalar (struct type *type);

bool
type_is_named (struct type *type);

bool
type_is_composite (struct type *type);

bool
type_is_assignable (struct type *type);

bool
type_is_callable (struct type *type);


struct type *
type_element (struct type *type);

struct type *
type_decay (struct type *type);

struct type *
type_find_common (struct type *a, struct type *b);


void
type_print (struct type *type, int depth);


#endif // TYPE_H

