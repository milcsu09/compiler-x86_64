#include "lexer.h"
#include "token.h"
#include "memory.h"
#include "string.h"

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct lexer
{
  struct location location;

  const char *current;
};


struct lexer *
lexer_create (const char *const file, const char *current)
{
  struct lexer *lexer;

  lexer = aa_malloc (sizeof (struct lexer));

  lexer->location.file = file;
  lexer->location.line = 1;
  lexer->location.column = 1;

  lexer->current = current;

  return lexer;
}


static void
lexer_advance (struct lexer *lexer)
{
  if (*lexer->current == '\0')
    return;

  char c = *lexer->current++;

  if (*lexer->current == '\0')
    return;

  if (c == '\n')
    {
      lexer->location.line++;
      lexer->location.column = 1;
    }
  else
    lexer->location.column++;
}


static void
lexer_advance_n (struct lexer *lexer, size_t n)
{
  while (n--)
    lexer_advance (lexer);
}


static struct token *
lexer_advance_token (struct lexer *lexer, enum token_kind kind)
{
  struct token *token;

  token = token_create (lexer->location, kind);

  lexer_advance (lexer);

  return token;
}


static struct token *
lexer_advance_n_token (struct lexer *lexer, size_t n, enum token_kind kind)
{
  struct token *token;

  token = token_create (lexer->location, kind);

  lexer_advance_n (lexer, n);

  return token;
}


static bool
lexer_match_start (struct lexer *lexer, const char *s)
{
  size_t length = strlen (s);

  return strncmp (lexer->current, s, length) == 0;
}


static bool
lexer_is_digit (char c, int base)
{
  switch (base)
    {
    case 2:
      return c == '0' || c == '1';
    case 8:
      return c >= '0' && c <= '7';
    case 10:
      return isdigit ((unsigned char)c);
    case 16:
      return isxdigit ((unsigned char)c);
    }

  return false;
}


struct keyword
{
  const char *key;
  enum token_kind kind;
};


static struct keyword KEYWORD_TABLE[] = {
  { "or",     TOKEN_OR },
  { "and",    TOKEN_AND },

  { "extern", TOKEN_EXTERN },
  { "fn",     TOKEN_FN },
  { "struct", TOKEN_STRUCT },
  { "return", TOKEN_RETURN },
  { "if",     TOKEN_IF },
  { "else",   TOKEN_ELSE },
  { "while",  TOKEN_WHILE },
  { "for",    TOKEN_FOR },
  { "print",  TOKEN_PRINT },

  { "void",   TOKEN_VOID },
  { "i8",     TOKEN_I8 },
  { "i16",    TOKEN_I16 },
  { "i32",    TOKEN_I32 },
  { "i64",    TOKEN_I64 },
  { "u8",     TOKEN_U8 },
  { "u16",    TOKEN_U16 },
  { "u32",    TOKEN_U32 },
  { "u64",    TOKEN_U64 },
};


enum
{
  KEYWORD_TABLE_SIZE = sizeof (KEYWORD_TABLE) / sizeof (struct keyword),
};


struct token *
lexer_next (struct lexer *lexer)
{
  while (isspace ((unsigned char)*lexer->current))
    lexer_advance (lexer);

  while (lexer->current[0] == '/' && lexer->current[1] == '/')
    {
      while (*lexer->current && *lexer->current != '\n')
        lexer_advance (lexer);

      while (isspace ((unsigned char)*lexer->current))
        lexer_advance (lexer);
    }

  struct location location = lexer->location;

  const char *start = lexer->current;

  char c = *start;

  if (c == '\0')
    return token_create (location, TOKEN_EOF);

  if (isdigit ((unsigned char)c))
    {
      int base = 10;

      if (start[0] == '0')
        switch (start[1])
          {
          case 'b':
          case 'B':
            lexer_advance_n (lexer, 2);

            start = start + 2;
            base = 2;

            break;

          case 'o':
          case 'O':
            lexer_advance_n (lexer, 2);

            start = start + 2;
            base = 8;

            break;

          case 'x':
          case 'X':
            lexer_advance_n (lexer, 2);

            start = start + 2;
            base = 16;

            break;
          }

      while (lexer_is_digit (*lexer->current, base))
        lexer_advance (lexer);

      errno = 0;
      long i = strtoul (start, NULL, base);

      if (errno == ERANGE)
        warning (location, "number out of range");

      return token_create_i (location, TOKEN_INTEGER, i);
    }

  if (isalpha (c) || c == '_')
    {
      while (isalnum ((unsigned char)*lexer->current) || *lexer->current == '_')
        lexer_advance (lexer);

      size_t length = lexer->current - start;

      for (size_t i = 0; i < KEYWORD_TABLE_SIZE; ++i)
        {
          struct keyword current = KEYWORD_TABLE[i];

          if (strlen (current.key) != length)
            continue;

          if (strncmp (start, current.key, length) == 0)
            return token_create (location, current.kind);
        }

      char *s = string_copy_n (start, length);

      return token_create_s (location, TOKEN_IDENTIFIER, s);
    }

  if (ispunct ((unsigned char)c))
    {
      switch (c)
        {
        case '=':
          if (lexer_match_start (lexer, "=="))
            return lexer_advance_n_token (lexer, 2, TOKEN_DE);
          return lexer_advance_token (lexer, TOKEN_EQ);

        case '!':
          if (lexer_match_start (lexer, "!="))
            return lexer_advance_n_token (lexer, 2, TOKEN_NE);
          break;

        case '<':
          if (lexer_match_start (lexer, "<="))
            return lexer_advance_n_token (lexer, 2, TOKEN_LE);
          return lexer_advance_token (lexer, TOKEN_L);
        case '>':
          if (lexer_match_start (lexer, ">="))
            return lexer_advance_n_token (lexer, 2, TOKEN_GE);
          return lexer_advance_token (lexer, TOKEN_G);

        case '.':
          return lexer_advance_token (lexer, TOKEN_DOT);
        case ',':
          return lexer_advance_token (lexer, TOKEN_COMMA);
        case ':':
          return lexer_advance_token (lexer, TOKEN_COLON);
        case ';':
          return lexer_advance_token (lexer, TOKEN_SEMICOLON);

        case '&':
          return lexer_advance_token (lexer, TOKEN_AMPERSAND);
        case '+':
          return lexer_advance_token (lexer, TOKEN_PLUS);
        case '-':
          if (lexer_match_start (lexer, "->"))
            return lexer_advance_n_token (lexer, 2, TOKEN_ARROW);
          return lexer_advance_token (lexer, TOKEN_MINUS);
        case '*':
          return lexer_advance_token (lexer, TOKEN_STAR);
        case '/':
          return lexer_advance_token (lexer, TOKEN_SLASH);
        case '%':
          return lexer_advance_token (lexer, TOKEN_PERCENT);

        case '(':
          return lexer_advance_token (lexer, TOKEN_LPAREN);
        case ')':
          return lexer_advance_token (lexer, TOKEN_RPAREN);
        case '[':
          return lexer_advance_token (lexer, TOKEN_LBRACKET);
        case ']':
          return lexer_advance_token (lexer, TOKEN_RBRACKET);
        case '{':
          return lexer_advance_token (lexer, TOKEN_LBRACE);
        case '}':
          return lexer_advance_token (lexer, TOKEN_RBRACE);

        default:
          break;
        }
    }

  error (lexer->location, "unexpected character");

  exit (1);
}


struct token *
lexer_peek (struct lexer *lexer)
{
  struct lexer copy = *lexer;

  return lexer_next (&copy);
}

