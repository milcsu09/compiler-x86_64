#include "token.h"
#include "memory.h"


static const char *const TOKEN_KIND_STRING[] = {
  "EOF",

  "'='",
  "'=='",
  "'!='",
  "'<'",
  "'>'",
  "'<='",
  "'>='",

  "'.'",
  "','",
  "':'",
  "';'",

  "'->'",

  "'&'",
  "'+'",
  "'-'",
  "'*'",
  "'/'",
  "'%'",

  "'('",
  "')'",
  "'['",
  "']'",
  "'{'",
  "'}'",

  "integer",
  "string",

  "identifier",

  "'or'",
  "'and'",

  "'extern'",
  "'fn'",
  "'struct'",
  "'return'",
  "'if'",
  "'else'",
  "'while'",
  "'for'",
  "'print'",

  "'void'",
  "'i8'",
  "'i16'",
  "'i32'",
  "'i64'",
  "'u8'",
  "'u16'",
  "'u32'",
  "'u64'",
};


const char *
token_kind_string (enum token_kind kind)
{
  return TOKEN_KIND_STRING[kind];
}


struct token *
token_create (struct location location, enum token_kind kind)
{
  struct token *token;

  token = aa_malloc (sizeof (struct token));

  token->location = location;

  token->kind = kind;

  return token;
}


struct token *
token_create_i (struct location location, enum token_kind kind, long i)
{
  struct token *token;

  token = token_create (location, kind);

  token->d.i = i;

  return token;
}


struct token *
token_create_s (struct location location, enum token_kind kind, char *s)
{
  struct token *token;

  token = token_create (location, kind);

  token->d.s = s;

  return token;
}


bool
token_match (struct token *token, enum token_kind kind)
{
  return token->kind == kind;
}

