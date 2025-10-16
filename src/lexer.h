#ifndef LEXER_H
#define LEXER_H


#include "token.h"


struct lexer
{
  struct location location;

  const char *current;
};


struct lexer *lexer_create (const char *const, const char *);

struct token *lexer_next (struct lexer *);

struct token *lexer_peek (struct lexer *);


#endif // LEXER_H

