#ifndef PARSER_H
#define PARSER_H


#include "tree.h"
#include "lexer.h"


struct parser
{
  struct lexer *lexer;

  struct token *current;
};


struct parser *parser_create (const char *const, const char *);

struct tree *parser_parse (struct parser *);


#endif // PARSER_H

