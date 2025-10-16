#include "tree.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>


static const char *const TREE_KIND_STRING[] = {
  "empty",
  "if",
  "while",
  "for",
  "compound",
  "variable_declaration",
  "print",

  "cast",
  "assignment",
  "binary",
  "reference",
  "dereference",
  "scale",
  "integer",
  "identifier",

  "type",

  "program",
};


const char *
tree_kind_string (enum tree_kind kind)
{
  return TREE_KIND_STRING[kind];
}


struct tree *
tree_create (struct location location, enum tree_kind tree_kind)
{
  struct tree *tree;

  tree = aa_malloc (sizeof (struct tree));

  tree->location = location;

  tree->child = NULL;
  tree->next = NULL;

  tree->type = NULL;

  tree->tree_kind = tree_kind;

  return tree;
}


struct tree *
tree_create_token (struct location location, enum tree_kind tree_kind, struct token *token)
{
  struct tree *tree;

  tree = tree_create (location, tree_kind);

  tree->token = token;

  return tree;
}


struct tree *
tree_create_token_kind (struct location location, enum tree_kind tree_kind,
                        enum token_kind token_kind)
{
  return tree_create_token (location, tree_kind, token_create (location, token_kind));
}


void
tree_attach (struct tree *tree, struct tree *node)
{
  struct tree *current = tree;

  while (current->next)
    current = current->next;

  current->next = node;
}


void
tree_append (struct tree *tree, struct tree *node)
{
  if (!tree->child)
    tree->child = node;
  else
    tree_attach (tree->child, node);
}


void
tree_morph (struct tree *tree, enum tree_kind kind)
{
  tree->tree_kind = kind;
}


void
tree_set_type (struct tree *tree, struct tree *type)
{
  tree->type = type;
}


void
tree_truncate (struct tree *tree)
{
  tree->next = NULL;
}


void
tree_wrap (struct tree *root, struct tree *target, struct tree *wrapper)
{
  struct tree *prev = NULL;
  struct tree *curr = root->child;

  while (curr && curr != target)
    {
      prev = curr;
      curr = curr->next;
    }

  if (curr != target)
    return;

  if (prev)
    prev->next = wrapper;
  else
    root->child = wrapper;

  wrapper->next = target->next;

  target->next = NULL;

  tree_append (wrapper, target);
}


void
tree_wrap_cast (struct tree *root, struct tree *target, struct tree *type)
{
  struct tree *cast;

  cast = tree_create (target->location, TREE_CAST);

  tree_set_type (cast, type);

  tree_wrap (root, target, cast);
}


void
tree_wrap_cast_p (struct tree *root, struct tree *target, enum type_kind type_kind)
{
  struct tree *type;

  type = type_create (target->location, type_kind);

  tree_wrap_cast (root, target, type);
}


bool
tree_wrap_cast_p_if (struct tree *root, struct tree *target, enum type_kind type_kind)
{
  // if (target->type->type_kind != type_kind)
  if (!type_match (target->type, type_kind))
    {
      tree_wrap_cast_p (root, target, type_kind);
      return true;
    }

  return false;
}


void
tree_debug_print_base (struct tree *tree, size_t previous_line, size_t indent)
{
  if (!tree)
    return;

  size_t line = tree->location.line;

  if (previous_line != line)
    fprintf (stderr, "%*zu ", (int)indent + 4, line);
  else
    fprintf (stderr, "%*s ", (int)indent + 4, "");

  fprintf (stderr, "\033[96m%-11s\033[0m ", tree_kind_string (tree->tree_kind));

  if (tree->tree_kind == TREE_TYPE)
    fprintf (stderr, "(\033[91m%s\033[0m)", type_kind_string (tree->type_kind));

  struct token *token = tree->token;

  if (token)
    switch (token->kind)
      {
      case TOKEN_INTEGER:
        fprintf (stderr, "(\033[95m%ld\033[0m)", token->data.i);
        break;
      case TOKEN_IDENTIFIER:
        fprintf (stderr, "(\033[0m%s\033[0m)", token->data.s);
        break;
      default:
        fprintf (stderr, "(\033[0m%s\033[0m)", token_kind_string (token->kind));
        break;
      }

  fprintf (stderr, "\n");

  if (tree->type != TYPE_ERROR)
    {
      tree_debug_print_base (tree->type, line, indent + 4);
      fprintf (stderr, "%*s ", (int)indent + 8, "");
      fprintf (stderr, "\033[90m//\033[0m\n");
    }

  tree_debug_print_base (tree->child, line, indent + 4);
  tree_debug_print_base (tree->next, line, indent);
}


void
tree_debug_print (struct tree *tree)
{
  tree_debug_print_base (tree, 0, 0);
}


bool
tree_is_left_value (struct tree *tree)
{
  switch (tree->tree_kind)
    {
    // case TREE_CAST:
    //   return tree_is_left_value (tree->child);
    case TREE_DEREFERENCE:
    case TREE_IDENTIFIER:
      return true;
    default:
      return false;
    }
}

