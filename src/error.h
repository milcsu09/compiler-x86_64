#ifndef ERROR_H
#define ERROR_H


#include <stddef.h>


#define unreachable()                                                                              \
  unreachable_at (__FILE__, __LINE__)

#define unreachable1(x)                                                                            \
  (unreachable_at (__FILE__, __LINE__), (x))


void unreachable_at (char *, int);


struct location
{
  const char *file;
  size_t line;
  size_t column;
};


extern const struct location location_none;


void error (struct location, const char *, ...);

void warning (struct location, const char *, ...);

void note (struct location, const char *, ...);

void info (struct location, const char *, ...);


#endif // ERROR_H

