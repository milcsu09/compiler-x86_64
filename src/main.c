#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "analyzer.h"
#include "cg.h"
#include "tree.h"
#include "string.h"
#include "memory.h"
#include "parser.h"


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
  char **paths;
  size_t paths_sz;

  bool p;
  bool s;
  bool S;
  bool c;

  bool o_stdout;

  char *ldflags;
};


void
compile_files (struct flags *flags)
{
  char gcc_command[0x10000] = { 0 };

  size_t gcc_command_sz = snprintf (gcc_command, sizeof gcc_command, "gcc -no-pie");

  bool abandon = flags->c;

  const char *out = NULL;

  for (size_t i = 0; i < flags->paths_sz; ++i)
    {
      const char *path = flags->paths[i];

      char path_base[256], path_s[256], path_o[256];

      strip_extension (path_base, sizeof path_base, path);

      if (out == NULL)
        out = string_copy (path_base);

      snprintf (path_s, sizeof path_s, "%.252s.s", path_base);
      snprintf (path_o, sizeof path_o, "%.252s.o", path_base);

      char *source = file_read (path);

      struct parser *parser = parser_create (path, source);

      struct tree *tree = parser_parse (parser);

      if (flags->p)
        {
          tree_print (tree, 0);

          abandon = true;

          continue;
        }

      struct analyzer *analyzer = analyzer_create ();

      analyzer_analyze (analyzer, tree);

      if (flags->s)
        {
          tree_print (tree, 0);

          abandon = true;

          continue;
        }

      FILE *fs = stdout;

      if (!flags->o_stdout)
        fs = file_open (path_s, "w");

      struct cg *cg = cg_create (fs);

      cg_generate (cg, tree);

      if (!flags->o_stdout)
        file_close (fs);

      if (flags->S)
        {
          abandon = true;

          continue;
        }

      char cmd[2048];

      snprintf (cmd, sizeof cmd, "nasm -f elf64 %s -o %s", path_s, path_o);

      info (location_none, "[CMD] %s", cmd);

      if (system (cmd) != 0)
        {
          error (location_none, "NASM failed");
          exit (1);
        }

      gcc_command_sz += snprintf (gcc_command + gcc_command_sz, sizeof gcc_command - gcc_command_sz,
                                  " %s", path_o);
    }

  if (abandon)
    return;

  gcc_command_sz += snprintf (gcc_command + gcc_command_sz, sizeof gcc_command - gcc_command_sz,
                              " -o %s %s", out, flags->ldflags);

  info (location_none, "[CMD] %s", gcc_command);

  if (system (gcc_command) != 0)
    {
      error (location_none, "GCC failed");
      exit (1);
    }
}


static void
usage (const char *path)
{
  fprintf (stderr, "Usage: %s [options] <file>\n", path);
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -h, --help         Show this help message and exit.\n");
  fprintf (stderr, "  -p                 Stop after parsing and dump the AST.\n");
  fprintf (stderr, "  -s                 Stop after semantic analyzer and dump the AST.\n");
  fprintf (stderr, "  -S                 Stop after generating assembly.\n");
  fprintf (stderr, "  --stdout           Implies -S; dump generated assembly to standard\n");
  fprintf (stderr, "                     output.\n");
  fprintf (stderr, "  -c                 Stop after generating object files\n");
  fprintf (stderr, "  --ldflags=<flags>  Additional linker flags.\n");
}


int
main (int argc, char **argv)
{
  atexit (aa_free);

  struct flags flags;

  flags.p = false;
  flags.s = false;
  flags.S = false;
  flags.o_stdout = false;

  flags.ldflags = "";

  flags.paths = aa_malloc (argc * sizeof (char *));

  for (int i = 1; i < argc; ++i)
    {
      if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "--help") == 0)
        {
          usage (argv[0]);
          exit (0);
        }

      if (strcmp (argv[i], "-p") == 0)
        flags.p = true;

      else if (strcmp (argv[i], "-s") == 0)
        flags.s = true;

      else if (strcmp (argv[i], "-S") == 0)
        flags.S = true;

      else if (strcmp (argv[i], "--stdout") == 0)
        {
          flags.S = true;
          flags.o_stdout = true;
        }

      else if (strcmp (argv[i], "-c") == 0)
        {
          flags.c = true;
        }

      else if (strncmp (argv[i], "--ldflags=", 10) == 0)
        flags.ldflags = argv[i] + 10;

      else
        flags.paths[flags.paths_sz++] = argv[i];
    }

  if (flags.paths_sz == 0)
    {
      error (location_none, "no input");
      exit (1);
    }

  compile_files (&flags);

  return 0;
}

