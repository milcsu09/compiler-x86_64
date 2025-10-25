#ifndef TOKEN_H
#define TOKEN_H


#include "error.h"

#include <stdbool.h>


union token_data
{
  long i;

  char *s;
};


enum token_kind
{
  TOKEN_EOF,

  TOKEN_EQ,
  TOKEN_DE,
  TOKEN_NE,
  TOKEN_L,
  TOKEN_G,
  TOKEN_LE,
  TOKEN_GE,

  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_SEMICOLON,

  TOKEN_AMPERSAND,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_PERCENT,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_INTEGER,

  TOKEN_IDENTIFIER,

  TOKEN_OR,
  TOKEN_AND,

  TOKEN_EXTERN,
  TOKEN_FN,
  TOKEN_RETURN,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_PRINT,

  TOKEN_VOID,
  TOKEN_I8,
  TOKEN_I16,
  TOKEN_I32,
  TOKEN_I64,
  TOKEN_U8,
  TOKEN_U16,
  TOKEN_U32,
  TOKEN_U64,
};


const char *token_kind_string (enum token_kind);


struct token
{
  struct location location;

  enum token_kind kind;

  union token_data d;
};


struct token *token_create (struct location, enum token_kind);

struct token *token_create_i (struct location, enum token_kind, long);

struct token *token_create_s (struct location, enum token_kind, char *);

bool token_match (struct token *, enum token_kind);


#endif // TOKEN_H

