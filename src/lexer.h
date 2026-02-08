#ifndef LEXER_H
#define LEXER_H


struct token;

struct lexer;


struct lexer *
lexer_create (const char *const file, const char *current);

struct token *
lexer_next (struct lexer *lexer);

struct token *
lexer_peek (struct lexer *lexer);


#endif // LEXER_H

