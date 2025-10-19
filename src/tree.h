#ifndef TREE_H
#define TREE_H


#include "token.h"
#include "type.h"


enum tree_kind
{
  // Top-level
  // TODO: TREE_FUNCTION_DECLARATION,
  TREE_FUNCTION_DEFINITION,

  // Statements
  TREE_EMPTY,
  TREE_IF,
  TREE_WHILE,
  TREE_FOR,
  TREE_COMPOUND,
  TREE_VARIABLE_DECLARATION,
  TREE_RETURN,
  TREE_PRINT,

  // Expressions
  TREE_CAST,
  TREE_CALL,
  TREE_ASSIGNMENT,
  TREE_BINARY,
  TREE_REFERENCE,
  TREE_DEREFERENCE,
  TREE_INTEGER,
  TREE_IDENTIFIER,

  // Miscellaneous
  TREE_TYPE,

  TREE_PROGRAM,
};


const char *tree_kind_string (enum tree_kind);


struct tree
{
  struct location location;

  struct token *token;

  struct tree *child;
  struct tree *next;

  struct tree *type;

  enum tree_kind tree_kind;
  enum type_kind type_kind;
};


struct tree *tree_create (struct location, enum tree_kind);

struct tree *tree_create_token (struct location, enum tree_kind, struct token *);

struct tree *tree_create_token_kind (struct location, enum tree_kind, enum token_kind);

void tree_attach (struct tree *, struct tree *);

void tree_append (struct tree *, struct tree *);

void tree_morph (struct tree *, enum tree_kind);

void tree_set_type (struct tree *, struct tree *);

void tree_wrap (struct tree *, struct tree *, struct tree *);

struct tree *tree_wrap_cast (struct tree *, struct tree *, struct tree *);

struct tree *tree_wrap_cast_p (struct tree *root, struct tree *target, enum type_kind);

bool tree_wrap_cast_p_if (struct tree *root, struct tree *target, enum type_kind);

void tree_debug_print (struct tree *);


bool tree_is_left_value (struct tree *);


#endif // TREE_H

