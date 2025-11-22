#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


const struct location location_none = {0};


static void
diagnostic_va (struct location location, const char *type, const char *format, va_list va)
{
  if (location.file != NULL)
    {
      // fprintf (stderr, "\033[94m%s\033[0m:", location.file);
      // fprintf (stderr, "\033[94m%ld\033[0m:", location.line);
      // fprintf (stderr, "\033[94m%ld\033[0m:", location.column);
      // fprintf (stderr, " ");
      fprintf (stderr, "%s:%ld:%ld: ", location.file, location.line, location.column);
    }

  fprintf (stderr, "%s: ", type);
  vfprintf (stderr, format, va);
  fprintf (stderr, "\n");
}


void
error (struct location location, const char *format, ...)
{
  va_list va;
  va_start (va, format);

  diagnostic_va (location, "\033[1;91merror\033[0m", format, va);

  va_end (va);
}


void
warning (struct location location, const char *format, ...)
{
  va_list va;
  va_start (va, format);

  diagnostic_va (location, "\033[1;35mwarning\033[0m", format, va);

  va_end (va);
}


void
note (struct location location, const char *format, ...)
{
  va_list va;
  va_start (va, format);

  diagnostic_va (location, "\033[1;94mnote\033[0m", format, va);

  va_end (va);
}

void
info (struct location location, const char *format, ...)
{
  va_list va;
  va_start (va, format);

  diagnostic_va (location, "\033[1;90minfo\033[0m", format, va);

  va_end (va);
}

