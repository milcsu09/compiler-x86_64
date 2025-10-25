#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "cg.h"
#include "tree.h"
#include "memory.h"
#include "parser.h"
#include "resolver.h"
#include "checker.h"


// static double
// dt_ms (struct timeval t0, struct timeval t1)
// {
//   return (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
// }


static double
dt_s (struct timeval t0, struct timeval t1)
{
  return (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1000000.0;
}


char *
read_file (const char *path)
{
  FILE *file = fopen (path, "r");

  if (file == NULL)
    {
      error (location_none, "%s: %s", path, strerror (errno));
      exit (1);
    }

  fseek (file, 0, SEEK_END);
  size_t file_size = ftell (file);
  fseek (file, 0, SEEK_SET);

  char *buffer = aa_malloc (file_size + 1);

  (void)fread (buffer, 1, file_size, file);

  buffer[file_size] = '\0';

  fclose (file);

  return buffer;
}


void
strip_extension (char *buffer, size_t size, const char *path)
{
  strncpy (buffer, path, size);

  buffer[size - 1] = '\0';

  char *dot = strrchr (buffer, '.');

  if (dot)
    *dot = '\0';
}


void
compile_file (const char *path, char *ld_flags)
{
  struct timeval t0, t1;

  char path_base[256];
  char path_s[256];
  char path_o[256];
  char path_u[256];

  strip_extension (path_base, sizeof path_base, path);

  snprintf (path_s, sizeof path_s, "%.252s.s", path_base);
  snprintf (path_o, sizeof path_o, "%.252s.o", path_base);
  snprintf (path_u, sizeof path_u, "%s", path_base);

  gettimeofday (&t0, NULL);

  char *source = read_file (path);

  struct parser *parser = parser_create (path, source);

  struct tree *tree = parser_parse (parser);

  // tree_print (tree, 0);

  // Pass 1
  struct resolver *resolver = resolver_create ();

  resolver_resolve (resolver, tree);

  // tree_print (tree, 0);

  // Pass 2
  struct checker *checker = checker_create ();

  checker_check (checker, tree);

  FILE *fs = fopen (path_s, "w");

  if (fs == NULL)
    {
      error (location_none, "%s: %s", path_s, strerror (errno));
      exit (1);
    }

  // Generate
  struct cg *cg = cg_create (fs);

  cg_generate (cg, tree);

  fclose(fs);

  gettimeofday (&t1, NULL);

  note (location_none, "Compiler %9.4fs", dt_s (t0, t1));

  gettimeofday (&t0, NULL);

  char cmd[2048];

  snprintf (cmd, sizeof cmd, "nasm -f elf64 %s -o %s", path_s, path_o);

  if (system (cmd) != 0)
    {
      error (location_none, "NASM failed");
      exit (1);
    }

  gettimeofday (&t1, NULL);

  note (location_none, "    NASM %9.4fs", dt_s (t0, t1));

  gettimeofday (&t0, NULL);

  snprintf (cmd, sizeof cmd, "gcc -no-pie %s -o %s %s", path_o, path_u, ld_flags);

  if (system (cmd) != 0)
    {
      error (location_none, "GCC failed");
      exit (1);
    }

  gettimeofday (&t1, NULL);

  note (location_none, "     GCC %9.4fs", dt_s (t0, t1));
}


int
main (int argc, char **argv)
{
  if (argc < 2)
    {
      error (location_none, "USAGE: %s <file>", argv[0]);
      return 1;
    }

  atexit (aa_free);

  char *ld_flags = "";

  if (argc > 2)
    ld_flags = argv[2];

  compile_file (argv[1], ld_flags);

  return 0;
}

