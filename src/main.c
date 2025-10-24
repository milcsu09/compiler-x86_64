#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "parser.h"
#include "resolver.h"
#include "checker.h"


char *
read_file (const char *path)
{
  FILE *file = fopen (path, "r");

  if (file == NULL)
    {
      perror (path);
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


int
main (void)
{
  atexit (aa_free);

  const char *f = "main.txt";

  char *s = read_file (f);

  // PARSE
  struct parser *parser = parser_create (f, s);

  struct tree *tree = parser_parse (parser);

  // tree_print (tree, 0);

  // PASS 1
  struct resolver *resolver = resolver_create ();

  resolver_resolve (resolver, tree);

  // tree_print (tree, 0);

  // PASS 2
  struct checker *checker = checker_create ();

  checker_check (checker, tree);

  return 0;
}

