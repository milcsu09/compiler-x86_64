#ifndef PARSER_H
#define PARSER_H


struct tree;

struct parser;


struct parser *
parser_create (const char *const file, const char *source);

struct tree *
parser_parse (struct parser *parser);


#endif // PARSER_H

