#include "parser.h"
#include "lexer.h"
#include "memory.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>


enum
{
  ASSOCIATIVITY_NONE,
  ASSOCIATIVITY_L,
  ASSOCIATIVITY_R,
};


struct operator
{
  enum token_kind kind;
  int p;
  int a;
};


static struct operator OPERATOR_TABLE[] = {
  { TOKEN_DEQUAL,  30, ASSOCIATIVITY_NONE },
  { TOKEN_NEQUAL,  30, ASSOCIATIVITY_NONE },

  { TOKEN_LT,      40, ASSOCIATIVITY_NONE },
  { TOKEN_GT,      40, ASSOCIATIVITY_NONE },
  { TOKEN_LTEQUAL, 40, ASSOCIATIVITY_NONE },
  { TOKEN_GTEQUAL, 40, ASSOCIATIVITY_NONE },

  { TOKEN_PLUS,    50, ASSOCIATIVITY_L },
  { TOKEN_MINUS,   50, ASSOCIATIVITY_L },

  { TOKEN_STAR,    60, ASSOCIATIVITY_L },
  { TOKEN_SLASH,   60, ASSOCIATIVITY_L },
};


enum
{
  OPERATOR_TABLE_SIZE = sizeof (OPERATOR_TABLE) / sizeof (struct operator),
};


static bool
parser_fetch_operator (struct token *token, struct operator *operator)
{
  for (size_t i = 0; i < OPERATOR_TABLE_SIZE; ++i)
    {
      struct operator current = OPERATOR_TABLE[i];

      if (token_match (token, current.kind))
        {
          *operator = current;
          return true;
        }
    }

  return false;
}


struct parser *
parser_create (const char *const file, const char *source)
{
  struct lexer *lexer;

  lexer = lexer_create (file, source);

  struct parser *parser;

  parser = aa_malloc (sizeof (struct parser));

  parser->lexer = lexer;

  return parser;
}


static void
parser_advance (struct parser *parser)
{
  parser->current = lexer_next (parser->lexer);
}


static bool
parser_match (struct parser *parser, enum token_kind kind)
{
  return token_match (parser->current, kind);
}


static void
parser_error_expect (struct parser *parser, const char *a, const char *b)
{
  error (parser->current->location, "unexpected %s, expected %s", b, a);
}


static void
parser_expect (struct parser *parser, enum token_kind kind)
{
  if (parser_match (parser, kind))
    return;

  const char *a = token_kind_string (kind);
  const char *b = token_kind_string (parser->current->kind);

  parser_error_expect (parser, a, b);

  exit (1);
}


static void
parser_expect_advance (struct parser *parser, enum token_kind kind)
{
  parser_expect (parser, kind);

  parser_advance (parser);
}


static struct tree *parser_parse_top (struct parser *);

static struct tree *parser_parse_top_function_definition (struct parser *);

static struct tree *parser_parse_statement (struct parser *);

static struct tree *parser_parse_statement_if (struct parser *);

static struct tree *parser_parse_statement_while (struct parser *);

static struct tree *parser_parse_statement_for (struct parser *);

static struct tree *parser_parse_statement_compound (struct parser *);

static struct tree *parser_parse_statement_variable_declaration (struct parser *);

static struct tree *parser_parse_statement_return (struct parser *);

static struct tree *parser_parse_statement_print (struct parser *);


static struct tree *parser_parse_expression_assignment (struct parser *);

static struct tree *parser_parse_expression_binary (struct parser *);

static struct tree *parser_parse_expression_cast (struct parser *);

static struct tree *parser_parse_expression_call (struct parser *);

static struct tree *parser_parse_expression_access (struct parser *);


static struct tree *parser_parse_primary (struct parser *);

static struct tree *parser_parse_primary_group (struct parser *);

static struct tree *parser_parse_primary_reference (struct parser *);

static struct tree *parser_parse_primary_dereference (struct parser *);

static struct tree *parser_parse_primary_integer (struct parser *);

static struct tree *parser_parse_primary_identifier (struct parser *);


static struct tree *parser_parse_program (struct parser *);

static struct tree *parser_parse_type_primary (struct parser *);

static struct tree *parser_parse_type (struct parser *);


static struct tree *
parser_parse_body (struct parser *parser, enum token_kind kind_until, enum tree_kind kind,
                   struct tree *(*parse_function) (struct parser *))
{
  struct tree *result;

  result = tree_create (parser->current->location, kind);

  while (1)
    {
      enum token_kind current = parser->current->kind;

      if (current == kind_until)
        return result;

      if (current == TOKEN_EOF)
        return result;

      struct tree *child;

      child = parse_function (parser);

      tree_append (result, child);

      switch (child->tree_kind)
        {
        case TREE_FUNCTION_DEFINITION:
        case TREE_IF:
        case TREE_WHILE:
        case TREE_FOR:
        case TREE_COMPOUND:
          break; // No semicolon!
        default:
          parser_expect_advance (parser, TOKEN_SEMICOLON);
          break;
        }
    }
}


static struct tree *
parser_parse_top (struct parser *parser)
{
  if (parser_match (parser, TOKEN_FN))
    return parser_parse_top_function_definition (parser);

  const char *b = token_kind_string (parser->current->kind);

  parser_error_expect (parser, "top-level declaration", b);

  exit (1);
}


static struct tree *
parser_parse_top_function_definition (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_FN);

  struct tree *result;

  result = tree_create (location, TREE_FUNCTION_DEFINITION);

  struct tree *f_type;

  f_type = type_create (location, TYPE_FUNCTION);

  tree_set_type (result, f_type);

  struct tree *f_name;

  f_name = parser_parse_primary_identifier (parser);

  tree_append (result, f_name);

  parser_expect_advance (parser, TOKEN_LPAREN);

  while (!parser_match (parser, TOKEN_RPAREN))
    {
      struct tree *p;

      p = parser_parse_statement_variable_declaration (parser);

      tree_append (result, p);

      tree_set_type (p, type_decay (p->type));

      tree_append (f_type, p->type);

      if (parser_match (parser, TOKEN_COMMA))
        parser_advance (parser);
      else
        parser_expect (parser, TOKEN_RPAREN);
    }

  parser_expect_advance (parser, TOKEN_RPAREN);

  struct tree *r_type;

  r_type = parser_parse_type (parser);

  tree_set_type (f_type, r_type);

  struct tree *body;

  body = parser_parse_statement_compound (parser);

  tree_append (result, body);

  return result;
}


static struct tree *
parser_parse_statement (struct parser *parser)
{
  if (parser_match (parser, TOKEN_SEMICOLON))
    return tree_create (parser->current->location, TREE_EMPTY);

  if (parser_match (parser, TOKEN_IF))
    return parser_parse_statement_if (parser);

  if (parser_match (parser, TOKEN_WHILE))
    return parser_parse_statement_while (parser);

  if (parser_match (parser, TOKEN_FOR))
    return parser_parse_statement_for (parser);

  if (parser_match (parser, TOKEN_LBRACE))
    return parser_parse_statement_compound (parser);

  if (parser_match (parser, TOKEN_IDENTIFIER))
    {
      struct token *peek = lexer_peek (parser->lexer);

      if (peek->kind != TOKEN_COLON)
        return parser_parse_expression_assignment (parser);

      return parser_parse_statement_variable_declaration (parser);
    }

  if (parser_match (parser, TOKEN_RETURN))
    return parser_parse_statement_return (parser);

  if (parser_match (parser, TOKEN_PRINT))
    return parser_parse_statement_print (parser);

  return parser_parse_expression_assignment (parser);
}


static struct tree *
parser_parse_statement_if (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_IF);

  struct tree *result;

  result = tree_create (location, TREE_IF);

  struct tree *condition;
  struct tree *then;

  condition = parser_parse_expression_assignment (parser);

  then = parser_parse_statement_compound (parser);

  tree_append (result, condition);
  tree_append (result, then);

  if (!parser_match (parser, TOKEN_ELSE))
    return result;

  parser_advance (parser);

  struct tree *otherwise;

  if (parser_match (parser, TOKEN_IF))
    otherwise = parser_parse_statement_if (parser);
  else
    otherwise = parser_parse_statement_compound (parser);

  tree_append (result, otherwise);

  return result;
}


static struct tree *
parser_parse_statement_while (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_WHILE);

  struct tree *result;

  result = tree_create (location, TREE_WHILE);

  struct tree *condition;
  struct tree *then;

  condition = parser_parse_expression_assignment (parser);

  then = parser_parse_statement_compound (parser);

  tree_append (result, condition);
  tree_append (result, then);

  return result;
}


static struct tree *
parser_parse_statement_for (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_FOR);

  struct tree *result;

  result = tree_create (location, TREE_FOR);

  struct tree *a;
  struct tree *b;
  struct tree *c;
  struct tree *body;

  if (parser_match (parser, TOKEN_SEMICOLON))
    a = tree_create (location, TREE_EMPTY);
  else
    a = parser_parse_expression_assignment (parser);

  parser_expect_advance (parser, TOKEN_SEMICOLON);

  b = parser_parse_expression_assignment (parser);

  parser_expect_advance (parser, TOKEN_SEMICOLON);

  if (parser_match (parser, TOKEN_LBRACE))
    c = tree_create (location, TREE_EMPTY);
  else
    c = parser_parse_expression_assignment (parser);

  body = parser_parse_statement_compound (parser);

  tree_append (result, a);
  tree_append (result, b);
  tree_append (result, c);
  tree_append (result, body);

  return result;
}


static struct tree *
parser_parse_statement_compound (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_LBRACE);

  struct tree *result;

  result = parser_parse_body (parser, TOKEN_RBRACE, TREE_COMPOUND, parser_parse_statement);

  result->location = location;

  parser_expect_advance (parser, TOKEN_RBRACE);

  return result;
}


static struct tree *
parser_parse_statement_variable_declaration (struct parser *parser)
{
  struct tree *id = parser_parse_primary_identifier (parser);

  parser_expect_advance (parser, TOKEN_COLON);

  // parser_expect_advance (parser, TOKEN_I64);

  struct tree *type = parser_parse_type (parser);

  struct tree *result = tree_create (id->location, TREE_VARIABLE_DECLARATION);

  tree_set_type (result, type);

  tree_append (result, id);

  return result;
}


static struct tree *
parser_parse_statement_return (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_RETURN);

  struct tree *result;

  result = tree_create (location, TREE_RETURN);

  if (parser_match (parser, TOKEN_SEMICOLON))
    return result;

  struct tree *child;

  child = parser_parse_expression_assignment (parser);

  tree_append (result, child);

  return result;
}


static struct tree *
parser_parse_statement_print (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_PRINT);

  struct tree *child;

  child = parser_parse_expression_assignment (parser);

  struct tree *result;

  result = tree_create (location, TREE_PRINT);

  tree_append (result, child);

  return result;
}


static struct tree *
parser_parse_expression_assignment (struct parser *parser)
{
  struct tree *left;

  left = parser_parse_expression_binary (parser);

  if (!parser_match (parser, TOKEN_EQUAL))
    return left;

  parser_advance (parser);

  struct tree *right;

  right = parser_parse_expression_assignment (parser);

  struct tree *assignment;

  assignment = tree_create (left->location, TREE_ASSIGNMENT);

  tree_append (assignment, left);
  tree_append (assignment, right);

  return assignment;
}


static struct tree *
parser_parse_expression_binary_base (struct parser *parser, int p)
{
  struct tree *left;

  left = parser_parse_expression_cast (parser);

  struct operator operator;

  if (!parser_fetch_operator (parser->current, &operator))
    return left;

  while ((operator.a == ASSOCIATIVITY_NONE && operator.p > p) ||
         (operator.a == ASSOCIATIVITY_L && operator.p > p) ||
         (operator.a == ASSOCIATIVITY_R && operator.p >= p))
    {
      if (operator.a == ASSOCIATIVITY_NONE && p > 0)
        {
          error (parser->current->location, "non-associative operator used consecutively");

          exit (1);
        }

      struct token *middle = parser->current;

      parser_advance (parser);

      struct tree *right;

      if (operator.a == ASSOCIATIVITY_L)
        right = parser_parse_expression_binary_base (parser, operator.p);
      else
        right = parser_parse_expression_binary_base (parser, operator.p - 1);

      struct tree *binary;

      binary = tree_create_token (left->location, TREE_BINARY, middle);

      tree_append (binary, left);
      tree_append (binary, right);

      left = binary;

      if (!parser_fetch_operator (parser->current, &operator))
        return left;
    }

  return left;
}


static struct tree *
parser_parse_expression_binary (struct parser *parser)
{
  return parser_parse_expression_binary_base (parser, 0);
}


static struct tree *
parser_parse_expression_cast (struct parser *parser)
{
  struct tree *left;

  left = parser_parse_expression_call (parser);

  if (!parser_match (parser, TOKEN_COLON))
    return left;

  parser_advance (parser);

  struct tree *type;

  type = parser_parse_type (parser);

  struct tree *cast;

  cast = tree_create (left->location, TREE_CAST);

  tree_append (cast, left);

  tree_set_type (cast, type);

  return cast;
}


static struct tree *
parser_parse_expression_call (struct parser *parser)
{
  struct tree *a;

  a = parser_parse_expression_access (parser);

  while (parser_match (parser, TOKEN_LPAREN))
    {
      struct tree *call;

      call = tree_create (a->location, TREE_CALL);

      tree_append (call, a);

      parser_advance (parser);

      while (!parser_match (parser, TOKEN_RPAREN))
        {
          struct tree *p;

          p = parser_parse_expression_assignment (parser);

          tree_append (call, p);

          if (parser_match (parser, TOKEN_COMMA))
            parser_advance (parser);
          else
            parser_expect (parser, TOKEN_RPAREN);
        }

      parser_expect_advance (parser, TOKEN_RPAREN);

      a = call;
    }

  return a;
}


static struct tree *
parser_parse_expression_access (struct parser *parser)
{
  struct tree *a;

  a = parser_parse_primary (parser);

  // NOTE: Access doesn't produce it's own tree node, rather it combines '+' with dereference.
  while (parser_match (parser, TOKEN_LBRACKET))
    {
      parser_advance (parser);

      struct tree *b;

      b = parser_parse_expression_assignment (parser);

      parser_expect_advance (parser, TOKEN_RBRACKET);

      struct tree *binary;

      binary = tree_create_token_kind (a->location, TREE_BINARY, TOKEN_PLUS);

      tree_append (binary, a);
      tree_append (binary, b);

      struct tree *deref;

      deref = tree_create (a->location, TREE_DEREFERENCE);

      tree_append (deref, binary);

      a = deref;
    }

  return a;
}


static struct tree *
parser_parse_primary (struct parser *parser)
{
  switch (parser->current->kind)
    {
    case TOKEN_AMPERSAND:
      return parser_parse_primary_reference (parser);
    case TOKEN_STAR:
      return parser_parse_primary_dereference (parser);
    case TOKEN_LPAREN:
      return parser_parse_primary_group (parser);
    case TOKEN_INTEGER:
      return parser_parse_primary_integer (parser);
    case TOKEN_IDENTIFIER:
      return parser_parse_primary_identifier (parser);
    default:
      break;
    }

  const char *b = token_kind_string (parser->current->kind);

  parser_error_expect (parser, "expression", b);

  exit (1);
}


struct tree *
parser_parse_primary_group (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_LPAREN);

  struct tree *result;

  result = parser_parse_expression_assignment (parser);

  result->location = location;

  parser_expect_advance (parser, TOKEN_RPAREN);

  return result;
}


static struct tree *
parser_parse_primary_reference (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_AMPERSAND);

  struct tree *child;

  child = parser_parse_expression_call (parser);

  struct tree *result;

  result = tree_create (location, TREE_REFERENCE);

  tree_append (result, child);

  return result;
}


static struct tree *
parser_parse_primary_dereference (struct parser *parser)
{
  struct location location = parser->current->location;

  parser_expect_advance (parser, TOKEN_STAR);

  struct tree *child;

  child = parser_parse_expression_call (parser);

  struct tree *result;

  result = tree_create (location, TREE_DEREFERENCE);

  tree_append (result, child);

  return result;

}


struct tree *
parser_parse_primary_atom (struct parser *parser, enum token_kind token_kind,
                           enum tree_kind tree_kind)
{
  parser_expect (parser, token_kind);

  struct tree *result;

  result = tree_create (parser->current->location, tree_kind);

  result->token = parser->current;

  parser_advance (parser);

  return result;
}


static struct tree *
parser_parse_primary_integer (struct parser *parser)
{
  return parser_parse_primary_atom (parser, TOKEN_INTEGER, TREE_INTEGER);
}


static struct tree *
parser_parse_primary_identifier (struct parser *parser)
{
  return parser_parse_primary_atom (parser, TOKEN_IDENTIFIER, TREE_IDENTIFIER);
}


static struct tree *
parser_parse_program (struct parser *parser)
{
  return parser_parse_body (parser, TOKEN_EOF, TREE_PROGRAM, parser_parse_top);
}


static struct tree *
parser_parse_type_primary (struct parser *parser)
{
  struct location location = parser->current->location;

  switch (parser->current->kind)
    {
    case TOKEN_VOID:
      parser_advance (parser);
      return type_create (location, TYPE_VOID);
    case TOKEN_I8:
      parser_advance (parser);
      return type_create (location, TYPE_I8);
    case TOKEN_I16:
      parser_advance (parser);
      return type_create (location, TYPE_I16);
    case TOKEN_I32:
      parser_advance (parser);
      return type_create (location, TYPE_I32);
    case TOKEN_I64:
      parser_advance (parser);
      return type_create (location, TYPE_I64);
    case TOKEN_U8:
      parser_advance (parser);
      return type_create (location, TYPE_U8);
    case TOKEN_U16:
      parser_advance (parser);
      return type_create (location, TYPE_U16);
    case TOKEN_U32:
      parser_advance (parser);
      return type_create (location, TYPE_U32);
    case TOKEN_U64:
      parser_advance (parser);
      return type_create (location, TYPE_U64);
    default:
      break;
    }

  const char *b = token_kind_string (parser->current->kind);

  parser_error_expect (parser, "type", b);

  exit (1);
}


static struct tree *
parser_parse_type (struct parser *parser)
{
  struct location location = parser->current->location;

  switch (parser->current->kind)
    {
    case TOKEN_STAR:
      parser_advance (parser);

      return type_create_pointer (location, parser_parse_type (parser));
    case TOKEN_LBRACKET:
      {
        parser_expect_advance (parser, TOKEN_LBRACKET);

        struct tree *n;
        n = parser_parse_primary_integer (parser);

        // if (parser_match (parser, TOKEN_RBRACKET))
        //   n = NULL;
        // else
        //   n = parser_parse_primary_integer (parser);

        parser_expect_advance (parser, TOKEN_RBRACKET);

        return type_create_array (location, parser_parse_type (parser), n);
      }
      break;
    case TOKEN_FN:
      {
        parser_advance (parser);

        struct tree *fn;

        fn = type_create (location, TYPE_FUNCTION);

        parser_expect_advance (parser, TOKEN_LPAREN);

        while (!parser_match (parser, TOKEN_RPAREN))
          {
            struct tree *t;

            t = parser_parse_type (parser);

            tree_append (fn, t);

            if (parser_match (parser, TOKEN_COMMA))
              parser_advance (parser);
            else
              parser_expect (parser, TOKEN_RPAREN);
          }

        parser_expect_advance (parser, TOKEN_RPAREN);

        struct tree *t;

        t = parser_parse_type (parser);

        tree_set_type (fn, t);

        // NOTE: Function type is always a pointer to a function object.
        return type_create_pointer (location, fn);
      }
      break;
    default:
      return parser_parse_type_primary (parser);
    }
}


struct tree *
parser_parse (struct parser *parser)
{
  parser_advance (parser);

  struct tree *result = parser_parse_program (parser); // parser_parse_binary (parser);

  parser_expect (parser, TOKEN_EOF);

  return result;
}

