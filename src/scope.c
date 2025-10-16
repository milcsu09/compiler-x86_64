#include "scope.h"
#include "memory.h"


#include <assert.h>
#include <string.h>


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
          symbol->key = current.key;
          return SCOPE_GET_OK;
        }
    }

  if (scope->parent)
    return scope_get (scope->parent, key, symbol);

  return SCOPE_GET_UNDEFINED;
}


/*
bool
scope_load (struct scope *scope, const char *key, struct symbol *symbol)
{
  while (scope)
    {
      for (size_t i = 0; i < scope->size; ++i)
        {
          struct symbol current = scope->data[i];

          if (strcmp (current.key, key) == 0)
            {
              if (symbol)
                {
                  symbol->type = current.type;
                  symbol->offset = current.offset;
                  symbol->key = current.key;
                }

              return true;
            }
        }

      scope = scope->parent;
    }

  return false;
}


bool
scope_exists (struct scope *scope, const char *key)
{
  for (size_t i = 0; i < scope->size; ++i)
    if (strcmp (scope->data[i].key, key) == 0)
      return true;
  return false;
}
*/

