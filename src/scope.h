#ifndef SCOPE_H
#define SCOPE_H


#include "error.h"

#include <stdbool.h>
#include <stddef.h>


struct tree;


struct symbol
{
  const char *key;

  size_t offset;

  struct tree *type;
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

void scope_set2 (struct scope *, struct symbol, struct location);

void scope_get2 (struct scope *, const char *, struct symbol *, struct location);


#endif // SCOPE_H

