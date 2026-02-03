#include "scope.h"
#include "memory.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>


struct symbol *
symbol_create (enum symbol_kind kind, const char *key, struct type *value)
{
  struct symbol *symbol;

  symbol = aa_malloc (sizeof (struct symbol));

  symbol->kind = kind;

  symbol->name = key;

  symbol->type = value;

  return symbol;
}


struct scope *
scope_create (struct scope *parent)
{
  struct scope *scope;

  scope = aa_malloc (sizeof (struct scope));

  scope->parent = parent;

  scope->head = NULL;

  return scope;
}


enum scope_set_result
scope_set (struct scope *scope, struct symbol *symbol)
{
  for (struct symbol *s = scope->head; s; s = s->next)
    if (strcmp (s->name, symbol->name) == 0)
      {
        return SCOPE_SET_REDEFINED;
      }

  symbol->next = scope->head;

  scope->head = symbol;

  return SCOPE_SET_OK;
}


enum scope_get_result
scope_get (struct scope *scope, const char *key, struct symbol **symbol)
{
  do
    {
      for (struct symbol *s = scope->head; s; s = s->next)
        if (strcmp (s->name, key) == 0)
          {
            if (symbol)
              *symbol = s;

            return SCOPE_GET_OK;
          }
    }
  while ((scope = scope->parent));

  return SCOPE_GET_UNDEFINED;
}


void
scope_set_validate (struct scope *scope, struct symbol *symbol, struct location location)
{
  enum scope_set_result result = scope_set (scope, symbol);

  switch (result)
    {
    case SCOPE_SET_OK:
      break;

    case SCOPE_SET_REDEFINED:
      error (location, "redefined '%s'", symbol->name);
      exit (1);
    }
}


void
scope_get_validate (struct scope *scope, const char *key, struct symbol **symbol,
                    struct location location)
{
  enum scope_get_result result = scope_get (scope, key, symbol);

  switch (result)
    {
    case SCOPE_GET_OK:
      break;

    case SCOPE_GET_UNDEFINED:
      error (location, "undefined '%s'", key);
      exit (1);
    }
}


void
scope_set_assert (struct scope *scope, struct symbol *symbol)
{
  assert (scope_set (scope, symbol) == SCOPE_SET_OK);
}


void
scope_get_assert (struct scope *scope, const char *key, struct symbol **symbol)
{
  assert (scope_get (scope, key, symbol) == SCOPE_GET_OK);
}

