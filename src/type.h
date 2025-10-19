#ifndef TYPE_H
#define TYPE_H


#include "error.h"

#include <stdbool.h>


enum type_width
{
  WIDTH_8  = 8,
  WIDTH_16 = 16,
  WIDTH_32 = 32,
  WIDTH_64 = 64,
};


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

  TYPE_FUNCTION,
};


const char *type_kind_string (enum type_kind);

enum type_width type_kind_width (enum type_kind);

bool type_kind_is_assignable (enum type_kind);

bool type_kind_is_label (enum type_kind);

bool type_kind_is_integer (enum type_kind);

bool type_kind_is_signed (enum type_kind);

bool type_kind_is_pointer (enum type_kind);

bool type_kind_is_array (enum type_kind);

bool type_kind_is_scalar (enum type_kind);

bool type_kind_is_complete (enum type_kind);

bool type_kind_is_callable (enum type_kind);

enum type_kind type_kind_promote_integer (enum type_kind, enum type_kind);


struct tree;


#define TYPE_ERROR NULL


struct tree *type_create (struct location, enum type_kind);

struct tree *type_create_pointer (struct location, struct tree *);

struct tree *type_create_array (struct location, struct tree *, struct tree *);

bool type_match (struct tree *, enum type_kind);

size_t type_size (struct tree *);

size_t type_alignment (struct tree *);

size_t type_element_size (struct tree *);

enum type_width type_width (struct tree *);

bool type_is_assignable (struct tree *);

bool type_is_label (struct tree *);

bool type_is_integer (struct tree *);

bool type_is_signed (struct tree *);

bool type_is_pointer (struct tree *);

bool type_is_pointer_to (struct tree *, enum type_kind);

bool type_is_array (struct tree *);

bool type_is_scalar (struct tree *);

bool type_is_complete (struct tree *);

bool type_is_callable (struct tree *);

struct tree *type_decay (struct tree *);

struct tree *type_reference (struct tree *);

struct tree *type_dereference (struct tree *);

enum type_kind type_promote_integer (struct tree *, struct tree *);


#endif // TYPE_H

