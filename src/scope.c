#include "scope.h"
#include "memory.h"


#include <assert.h>
#include <string.h>
#include <stdlib.h>


struct scope *
scope_create (struct scope *parent, size_t capacity)
{
  struct scope *scope;

  scope = aa_malloc (sizeof (struct scope) + capacity * sizeof (struct symbol));

  scope->parent = parent;

  scope->size = 0;
  scope->capacity = capacity;

  return scope;
}


enum scope_set_result
scope_set (struct scope *scope, struct symbol symbol)
{
  for (size_t i = 0; i < scope->size; ++i)
    if (strcmp (scope->data[i].key, symbol.key) == 0)
      {
        return SCOPE_SET_REDEFINED;
      }

  assert (scope->size < scope->capacity);

  scope->data[scope->size++] = symbol;

  return SCOPE_SET_OK;
}


enum scope_get_result
scope_get (struct scope *scope, const char *key, struct symbol *symbol)
{
  for (size_t i = 0; i < scope->size; ++i)
    {
      struct symbol current = scope->data[i];

      if (strcmp (current.key, key) == 0)
        {
          symbol->type = current.type;
          symbol->offset = current.offset;
          symbol->storage = current.storage;
          symbol->key = current.key;
          return SCOPE_GET_OK;
        }
    }

  if (scope->parent)
    return scope_get (scope->parent, key, symbol);

  return SCOPE_GET_UNDEFINED;
}



void
scope_set2 (struct scope *scope, struct symbol symbol, struct location location)
{
  enum scope_set_result result = scope_set (scope, symbol);

  switch (result)
    {
    case SCOPE_SET_OK:
      break;
    case SCOPE_SET_REDEFINED:
      error (location, "redefined %s", symbol.key);
      exit (1);
    }
}


void
scope_get2 (struct scope *scope, const char *key, struct symbol *symbol, struct location location)
{
  enum scope_get_result result = scope_get (scope, key, symbol);

  switch (result)
    {
    case SCOPE_GET_OK:
      break;
    case SCOPE_GET_UNDEFINED:
      error (location, "undefined %s", key);
      exit (1);
    }
}


