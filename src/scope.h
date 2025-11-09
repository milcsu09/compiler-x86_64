#ifndef SCOPE_H
#define SCOPE_H


#include "error.h"

#include <stdbool.h>
#include <stddef.h>


struct tree;


enum symbol_scope
{
  SYMBOL_GLOBAL,
  SYMBOL_LOCAL,
  SYMBOL_FIELD,
};


// struct symbol_global
// {
// };


struct symbol_local
{
  size_t offset;
};


struct symbol_field
{
  size_t offset;
};


union symbol_data
{
  // struct symbol_global global;

  struct symbol_local local;
  struct symbol_field field;
};


struct symbol
{
  enum symbol_scope scope;

  const char *name;

  struct type *type;

  union symbol_data d;
};


struct scope
{
  struct scope *parent;

  size_t size;
  size_t capacity;

  struct symbol data[];
};


struct scope *scope_create (struct scope *, size_t);


enum scope_set_result
{
  SCOPE_SET_OK,
  SCOPE_SET_REDEFINED,
};


enum scope_get_result
{
  SCOPE_GET_OK,
  SCOPE_GET_UNDEFINED,
};


enum scope_set_result scope_set (struct scope *, struct symbol);

enum scope_get_result scope_get (struct scope *, const char *, struct symbol *);

void scope_set_validate (struct scope *, struct symbol, struct location);

void scope_get_validate (struct scope *, const char *, struct symbol *, struct location);


#endif // SCOPE_H

