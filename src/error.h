#ifndef ERROR_H
#define ERROR_H


#include <stddef.h>


struct location
{
  const char *file;
  size_t line;
  size_t column;
};


extern const struct location location_none;


void bug (const char *, ...);

void error (struct location, const char *, ...);
void warning (struct location, const char *, ...);
void note (struct location, const char *, ...);


#endif // ERROR_H

