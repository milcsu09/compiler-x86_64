#include "scope.h"
#include "memory.h"

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
    if (strcmp (scope->data[i].name, symbol.name) == 0)
      {
        return SCOPE_SET_REDEFINED;
      }

  if (scope->size >= scope->capacity)
    {
      error (location_none, "too many entries in scope (max. %zu)", scope->capacity);

      exit (1);
    }

  scope->data[scope->size++] = symbol;

  return SCOPE_SET_OK;
}


enum scope_get_result
scope_get (struct scope *scope, const char *name, struct symbol *symbol)
{
  for (size_t i = 0; i < scope->size; ++i)
    {
      struct symbol current = scope->data[i];

      if (strcmp (current.name, name) == 0)
        {
          *symbol = current;
          return SCOPE_GET_OK;
        }
    }

  if (scope->parent)
    return scope_get (scope->parent, name, symbol);

  return SCOPE_GET_UNDEFINED;
}



void
scope_set_validate (struct scope *scope, struct symbol symbol, struct location location)
{
  enum scope_set_result result = scope_set (scope, symbol);

  switch (result)
    {
    case SCOPE_SET_OK:
      break;
    case SCOPE_SET_REDEFINED:
      error (location, "redefined %s", symbol.name);
      exit (1);
    }
}


void
scope_get_validate (struct scope *scope, const char *name, struct symbol *symbol,
                    struct location location)
{
  enum scope_get_result result = scope_get (scope, name, symbol);

  switch (result)
    {
    case SCOPE_GET_OK:
      break;
    case SCOPE_GET_UNDEFINED:
      error (location, "undefined %s", name);
      exit (1);
    }
}

