#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

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


static FILE *
file_open (const char *path, const char *mode)
{
  FILE *file = fopen (path, mode);

  if (file == NULL)
    {
      error (location_none, "%s: %s", path, strerror (errno));
      exit (1);
    }

  return file;
}


static void
file_close (FILE *file)
{
  fclose (file);
}


static char *
file_read (const char *path)
{
  FILE *file = file_open (path, "r");

  fseek (file, 0, SEEK_END);
  size_t file_size = ftell (file);
  fseek (file, 0, SEEK_SET);

  char *buffer = aa_malloc (file_size + 1);

  (void)fread (buffer, 1, file_size, file);

  buffer[file_size] = '\0';

  file_close (file);

  return buffer;
}


static void
strip_extension (char *buffer, size_t size, const char *path)
{
  snprintf (buffer, size, "%s", path);

  char *slash = strrchr (buffer, '/');
  char *dot = strrchr (slash ? slash + 1 : buffer, '.');

  if (dot)
    *dot = '\0';
}


struct flags
{
  char *path;

  bool p;
  bool sr;
  bool sc;

  bool S;

  bool o_stdout;

  char *ldflags;
};


void
compile_file (struct flags flags)
{
  struct timeval t0, t1;

  char path_base[256];
  char path_s[256];
  char path_o[256];
  char path_u[256];

  strip_extension (path_base, sizeof path_base, flags.path);

  snprintf (path_s, sizeof path_s, "%.252s.s", path_base);
  snprintf (path_o, sizeof path_o, "%.252s.o", path_base);
  snprintf (path_u, sizeof path_u, "%s", path_base);

  gettimeofday (&t0, NULL);

  char *source = file_read (flags.path);

  struct parser *parser = parser_create (flags.path, source);

  struct tree *tree = parser_parse (parser);

  if (flags.p)
    {
      tree_print (tree, 0);
      return;
    }

  // Pass 1
  struct resolver *resolver = resolver_create ();

  resolver_resolve (resolver, tree);

  if (flags.sr)
    {
      tree_print (tree, 0);
      return;
    }

  // Pass 2
  struct checker *checker = checker_create ();

  checker_check (checker, tree);

  if (flags.sc)
    {
      tree_print (tree, 0);
      return;
    }

  FILE *fs = stdout;

  if (!flags.o_stdout)
    {
      fs = fopen (path_s, "w");

      if (fs == NULL)
        {
          error (location_none, "%s: %s", path_s, strerror (errno));
          exit (1);
        }
    }

  // Generate
  struct cg *cg = cg_create (fs);

  cg_generate (cg, tree);

  if (!flags.o_stdout)
    fclose(fs);

  gettimeofday (&t1, NULL);

  info (location_none, "Compiler %9.4fs (%s)", dt_s (t0, t1), flags.o_stdout ? "stdout" : path_s);

  if (flags.S)
    return;

  gettimeofday (&t0, NULL);

  char cmd[2048];

  snprintf (cmd, sizeof cmd, "nasm -f elf64 %s -o %s", path_s, path_o);

  if (system (cmd) != 0)
    {
      error (location_none, "NASM failed");
      exit (1);
    }

  gettimeofday (&t1, NULL);

  info (location_none, "    NASM %9.4fs (%s)", dt_s (t0, t1), path_o);

  gettimeofday (&t0, NULL);

  snprintf (cmd, sizeof cmd, "gcc -no-pie %s -o %s %s", path_o, path_u, flags.ldflags);

  if (system (cmd) != 0)
    {
      error (location_none, "GCC failed");
      exit (1);
    }

  gettimeofday (&t1, NULL);

  info (location_none, "     GCC %9.4fs (%s)", dt_s (t0, t1), path_u);
}


static void
usage (const char *path)
{
  fprintf (stderr, "Usage: %s [options] <file>\n", path);
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -h, --help         Show this help message and exit.\n");
  fprintf (stderr, "  -p                 Stop after parsing and dump the AST.\n");
  fprintf (stderr, "  -sr                Stop after semantic resolution and dump the AST.\n");
  fprintf (stderr, "  -sc                Stop after semantic checking and dump the AST.\n");
  fprintf (stderr, "  -S                 Stop after generating assembly.\n");
  fprintf (stderr, "  --stdout           Implies -S; dump generated assembly to standard\n");
  fprintf (stderr, "                     output.\n");
  fprintf (stderr, "  --ldflags=<flags>  Additional linker flags.\n");
}


int
main (int argc, char **argv)
{
  atexit (aa_free);

  struct flags flags;

  flags.p = false;
  flags.sr = false;
  flags.sc = false;
  flags.S = false;
  flags.o_stdout = false;
  flags.path = NULL;

  flags.ldflags = "";

  for (int i = 1; i < argc; ++i)
    {
      if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "--help") == 0)
        {
          usage (argv[0]);
          exit (0);
        }

      if (strcmp (argv[i], "-p") == 0)
        flags.p = true;

      else if (strcmp (argv[i], "-sr") == 0)
        flags.sr = true;

      else if (strcmp (argv[i], "-sc") == 0)
        flags.sc = true;

      else if (strcmp (argv[i], "-S") == 0)
        flags.S = true;

      else if (strcmp (argv[i], "--stdout") == 0)
        {
          flags.S = true;
          flags.o_stdout = true;
        }

      else if (strncmp (argv[i], "--ldflags=", 10) == 0)
        flags.ldflags = argv[i] + 10;

      else
        flags.path = argv[i];
    }

  if (flags.path == NULL)
    {
      error (location_none, "no input");
      exit (1);
    }

  compile_file (flags);

  return 0;
}

