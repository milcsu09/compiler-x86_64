#ifndef ERROR_H
#define ERROR_H


#include <stddef.h>


#define unreachable()                                                                              \
  unreachable_at (__FILE__, __LINE__)

#define unreachable1(x)                                                                            \
  (unreachable_at (__FILE__, __LINE__), (x))


void
unreachable_at (char *file, int line);


struct location
{
  const char *file;
  size_t line;
  size_t column;
};


extern const struct location location_none;


void
error (struct location location, const char *format, ...);

void
warning (struct location location, const char *format, ...);

void
note (struct location location, const char *format, ...);

void
info (struct location location, const char *format, ...);


#endif // ERROR_H

