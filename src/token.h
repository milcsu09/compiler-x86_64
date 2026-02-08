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

  TOKEN_DOT,
  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_SEMICOLON,
  TOKEN_ARROW,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_BANG,
  TOKEN_TILDE,

  TOKEN_PLUS,
  TOKEN_MINUS,

  TOKEN_STAR,
  TOKEN_SLASH,

  TOKEN_PERCENT,

  TOKEN_CARET,
  TOKEN_PIPE,
  TOKEN_AMPERSAND,

  TOKEN_SHL,
  TOKEN_SHR,

  TOKEN_EQ,

  TOKEN_DE,
  TOKEN_NE,
  TOKEN_L,
  TOKEN_G,
  TOKEN_LE,
  TOKEN_GE,

  TOKEN_INTEGER,
  TOKEN_STRING,

  TOKEN_IDENTIFIER,

  TOKEN_OR,
  TOKEN_AND,

  TOKEN_FN,
  TOKEN_EXTERN,
  TOKEN_STRUCT,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_RETURN,
  TOKEN_BREAK,
  TOKEN_CONTINUE,
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


const char *
token_kind_string (enum token_kind kind);


struct token
{
  struct location location;

  enum token_kind kind;

  union token_data d;
};


struct token *
token_create (struct location location, enum token_kind kind);

struct token *
token_create_i (struct location location, enum token_kind kind, long i);

struct token *
token_create_s (struct location location, enum token_kind kind, char *s);

bool
token_match (struct token *token, enum token_kind kind);


#endif // TOKEN_H

