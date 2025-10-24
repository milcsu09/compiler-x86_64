#ifndef LEXER_H
#define LEXER_H


struct token;

struct lexer;


struct lexer *lexer_create (const char *const, const char *);

struct token *lexer_next (struct lexer *);

struct token *lexer_peek (struct lexer *);


#endif // LEXER_H

