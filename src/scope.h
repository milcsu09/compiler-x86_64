#ifndef SCOPE_H
#define SCOPE_H


#include "error.h"

#include <stdbool.h>
#include <stddef.h>


struct tree;


enum symbol_kind
{
  SYMBOL_GLOBAL,
  SYMBOL_LOCAL,

  SYMBOL_FIELD,
};


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
  struct symbol_local local;
  struct symbol_field field;
};


struct symbol
{
  union symbol_data d;

  struct symbol *next;

  enum symbol_kind kind;

  const char *name;

  struct type *type;
};


struct symbol *symbol_create (enum symbol_kind, const char *, struct type *);


struct scope
{
  struct scope *parent;

  struct symbol *head;
};


struct scope *scope_create (struct scope *);


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


enum scope_set_result scope_set (struct scope *, struct symbol *);

enum scope_get_result scope_get (struct scope *, const char *, struct symbol **);

void scope_set_validate (struct scope *, struct symbol *, struct location);

void scope_get_validate (struct scope *, const char *, struct symbol **, struct location);

void scope_set_assert (struct scope *, struct symbol *);

void scope_get_assert (struct scope *, const char *, struct symbol **);


#endif // SCOPE_H

