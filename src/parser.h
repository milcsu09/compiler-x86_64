#ifndef PARSER_H
#define PARSER_H


struct tree;

struct parser;


struct parser *parser_create (const char *const, const char *);

struct tree *parser_parse (struct parser *);


#endif // PARSER_H

