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


const char *type_kind_string (enum type_kind);


struct type_node_pointer
{
  struct type *base;
};


struct type_node_array
{
  struct tree *size;
  struct type *base;
};


struct type_node_struct
{
  struct type *field1;

  struct scope *scope;
};


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
  struct type_node_struct struct_t;
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


enum type_width type_width (struct type *);

size_t type_size (struct type *);

size_t type_element_size (struct type *);

size_t type_alignment (struct type *);


#define TYPE_ERROR NULL


struct type *type_create (struct location, enum type_kind);

struct type *type_create_pointer (struct location, struct type *);

struct type *type_create_array (struct location, struct tree *, struct type *);


void type_append (struct type **, struct type *);


bool type_cast_required (struct type *, struct type *);


bool type_is_incomplete (struct type *);

bool type_is_void (struct type *);

bool type_is_integer (struct type *);

bool type_is_integer_signed (struct type *);

bool type_is_pointer (struct type *);

bool type_is_pointer_to_k (struct type *, enum type_kind);

bool type_is_label (struct type *);

bool type_is_scalar (struct type *);


bool type_is_composite (struct type *);

bool type_is_assignable (struct type *);

bool type_is_callable (struct type *);


struct type *type_decay (struct type *);

struct type *type_promote (struct type *, struct type *);


void type_print (struct type *, int);


#endif // TYPE_H

