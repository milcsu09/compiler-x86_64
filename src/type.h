#ifndef TYPE_H
#define TYPE_H


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

  TYPE_FUNCTION,
};


const char *type_kind_string (enum type_kind);


struct type_node_pointer
{
  struct type *base;
};


struct type_node_array
{
  struct type *base;
  struct tree *size;
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
  struct type_node_function function;
};


struct type
{
  union type_data d;

  struct type *next;

  enum type_kind kind;
};


struct type *type_create (enum type_kind);

void type_append (struct type **, struct type *);

void type_print (struct type *, int);


#endif // TYPE_H

