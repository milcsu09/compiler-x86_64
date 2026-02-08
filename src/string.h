#ifndef STRING_H
#define STRING_H


#include <stddef.h>


char *
string_copy (const char *s);

char *
string_copy_n (const char *s, size_t length);

char *
string_copy_until (const char *s, const char *e);

char
char_escape (char c);

void
string_escape (char *s);


#endif // STRING_H


