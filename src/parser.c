#include "parser.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"
#include "type.h"
#include "memory.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>


// TODO: Too much repetition


struct parser
{
  struct location location;

  struct lexer *lexer;

  struct token *current;
};


enum
{
  ASSOCIATIVITY_L,
  ASSOCIATIVITY_R,
};


struct operator
{
  enum token_kind kind;
  enum binary_operator o;
  int p;
  int a;
};


static struct operator OPERATOR_TABLE[] = {
  { TOKEN_OR,         BINARY_LOR,     10, ASSOCIATIVITY_L },
  { TOKEN_AND,        BINARY_LAND,    20, ASSOCIATIVITY_L },

  { TOKEN_PIPE,       BINARY_BOR,     30, ASSOCIATIVITY_L },

  { TOKEN_CARET,      BINARY_BXOR,    40, ASSOCIATIVITY_L },

  { TOKEN_AMPERSAND,  BINARY_BAND,    50, ASSOCIATIVITY_L },

  { TOKEN_DE,         BINARY_CMP_EQ,  60, ASSOCIATIVITY_L },
  { TOKEN_NE,         BINARY_CMP_NE,  60, ASSOCIATIVITY_L },

  { TOKEN_L,          BINARY_CMP_L,   70, ASSOCIATIVITY_L },
  { TOKEN_G,          BINARY_CMP_G,   70, ASSOCIATIVITY_L },
  { TOKEN_LE,         BINARY_CMP_LE,  70, ASSOCIATIVITY_L },
  { TOKEN_GE,         BINARY_CMP_GE,  70, ASSOCIATIVITY_L },

  { TOKEN_SHL,        BINARY_SHL,     80, ASSOCIATIVITY_L },
  { TOKEN_SHR,        BINARY_SHR,     80, ASSOCIATIVITY_L },

  { TOKEN_PLUS,       BINARY_ADD,     90, ASSOCIATIVITY_L },
  { TOKEN_MINUS,      BINARY_SUB,     90, ASSOCIATIVITY_L },

  { TOKEN_STAR,       BINARY_MUL,     100, ASSOCIATIVITY_L },
  { TOKEN_SLASH,      BINARY_DIV,     100, ASSOCIATIVITY_L },
  { TOKEN_PERCENT,    BINARY_MOD,     100, ASSOCIATIVITY_L },
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

  parser->location = parser->current->location;
}


static bool
parser_match (struct parser *parser, enum token_kind kind)
{
  return token_match (parser->current, kind);
}


static void
parser_error_expect (struct parser *parser, const char *a, const char *b)
{
  error (parser->location, "unexpected %s, expected %s", b, a);
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

static struct tree *parser_parse_top_fdeclaration (struct parser *);

static struct tree *parser_parse_top_fdefinition (struct parser *);

static struct tree *parser_parse_top_struct (struct parser *);

static struct tree *parser_parse_statement (struct parser *);

static struct tree *parser_parse_statement_if (struct parser *);

static struct tree *parser_parse_statement_while (struct parser *);

static struct tree *parser_parse_statement_for (struct parser *);

static struct tree *parser_parse_statement_compound (struct parser *);

static struct tree *parser_parse_statement_vdeclaration (struct parser *);

static struct tree *parser_parse_statement_return (struct parser *);

static struct tree *parser_parse_statement_print (struct parser *);


static struct tree *parser_parse_expression_assignment (struct parser *);

static struct tree *parser_parse_expression_binary (struct parser *);

static struct tree *parser_parse_expression_cast (struct parser *);

// static struct tree *parser_parse_expression_access (struct parser *);

static struct tree *parser_parse_expression_postfix (struct parser *);

static struct tree *parser_parse_expression_call (struct parser *);

// static struct tree *parser_parse_expression_index (struct parser *);


static struct tree *parser_parse_primary (struct parser *);

static struct tree *parser_parse_primary_group (struct parser *);

static struct tree *parser_parse_primary_reference (struct parser *);

static struct tree *parser_parse_primary_dereference (struct parser *);

static struct tree *parser_parse_primary_integer (struct parser *);

static struct tree *parser_parse_primary_string (struct parser *);

static struct tree *parser_parse_primary_identifier (struct parser *);


static struct tree *parser_parse_program (struct parser *);


static struct type *parser_parse_type_primary (struct parser *);

static struct type *parser_parse_type (struct parser *);


static struct tree *
parser_parse_top (struct parser *parser)
{
  if (parser_match (parser, TOKEN_EXTERN))
    {
      parser_advance (parser);

      return parser_parse_top_fdeclaration (parser);
    }

  if (parser_match (parser, TOKEN_FN))
    return parser_parse_top_fdefinition (parser);

  if (parser_match (parser, TOKEN_STRUCT))
    return parser_parse_top_struct (parser);

  const char *b = token_kind_string (parser->current->kind);

  parser_error_expect (parser, "top-level declaration", b);

  exit (1);
}


static struct tree *
parser_parse_top_fdeclaration (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_FDECLARATION);

  parser_expect_advance (parser, TOKEN_FN);

  struct type *type;

  type = type_create (result->location, TYPE_FUNCTION);

  parser_expect (parser, TOKEN_IDENTIFIER);

  char *name = parser->current->d.s;

  parser_advance (parser);

  parser_expect_advance (parser, TOKEN_LPAREN);

  while (!parser_match (parser, TOKEN_RPAREN))
    {
      struct type *parameter_type;

      parameter_type = type_decay (parser_parse_type (parser));

      type_append (&type->d.function.from1, parameter_type);

      if (parser_match (parser, TOKEN_COMMA))
        parser_advance (parser);
      else
        parser_expect (parser, TOKEN_RPAREN);
    }

  parser_expect_advance (parser, TOKEN_RPAREN);

  if (parser_match (parser, TOKEN_ARROW))
    {
      parser_advance (parser);

      type->d.function.to = type_decay (parser_parse_type (parser));
    }
  else
    type->d.function.to = type_create (result->location, TYPE_VOID);

  result->d.fdeclaration.name = name;

  result->d.fdeclaration.type = type;

  return result;

}


static struct tree *
parser_parse_top_fdefinition (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_FDEFINITION);

  parser_expect_advance (parser, TOKEN_FN);

  struct type *type;

  type = type_create (result->location, TYPE_FUNCTION);

  parser_expect (parser, TOKEN_IDENTIFIER);

  char *name = parser->current->d.s;

  parser_advance (parser);

  parser_expect_advance (parser, TOKEN_LPAREN);

  while (!parser_match (parser, TOKEN_RPAREN))
    {
      struct tree *parameter;

      parameter = parser_parse_statement_vdeclaration (parser);

      // struct type *parameter_type;

      // parameter_type = type_decay (parameter->d.vdeclaration.type);

      parameter->d.vdeclaration.type = type_decay (parameter->d.vdeclaration.type);

      tree_append (&result->d.fdefinition.parameter1, parameter);

      // type_append (&type->d.function.from1, parameter_type);

      if (parser_match (parser, TOKEN_COMMA))
        parser_advance (parser);
      else
        parser_expect (parser, TOKEN_RPAREN);
    }

  parser_expect_advance (parser, TOKEN_RPAREN);

  if (parser_match (parser, TOKEN_ARROW))
    {
      parser_advance (parser);

      type->d.function.to = type_decay (parser_parse_type (parser));
    }
  else
    type->d.function.to = type_create (result->location, TYPE_VOID);

  result->d.fdefinition.name = name;

  result->d.fdefinition.body = parser_parse_statement_compound (parser);

  result->d.fdefinition.type = type;

  return result;
}


static struct tree *
parser_parse_top_struct (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_STRUCT);

  parser_expect_advance (parser, TOKEN_STRUCT);

  struct type *type;

  type = type_create (result->location, TYPE_STRUCT);

  parser_expect (parser, TOKEN_IDENTIFIER);

  char *name = parser->current->d.s;

  parser_advance (parser);

  parser_expect_advance (parser, TOKEN_LBRACE);

  do
    {
      struct tree *field;

      field = parser_parse_statement_vdeclaration (parser);

      tree_append (&result->d.struct_s.field1, field);

      if (parser_match (parser, TOKEN_SEMICOLON))
        parser_advance (parser);
      else
        parser_expect (parser, TOKEN_RBRACE);
    }
  while (!parser_match (parser, TOKEN_RBRACE));

  parser_expect_advance (parser, TOKEN_RBRACE);

  result->d.struct_s.name = name;

  result->d.struct_s.type = type;

  return result;
}


static struct tree *
parser_parse_statement (struct parser *parser)
{
  if (parser_match (parser, TOKEN_SEMICOLON))
    return tree_create (parser->location, TREE_EMPTY);

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

      return parser_parse_statement_vdeclaration (parser);
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
  struct tree *result;

  result = tree_create (parser->location, TREE_IF);

  parser_expect_advance (parser, TOKEN_IF);

  result->d.if_s.condition = parser_parse_expression_assignment (parser);

  result->d.if_s.branch_a = parser_parse_statement_compound (parser);

  if (!parser_match (parser, TOKEN_ELSE))
    return result;

  parser_advance (parser);

  if (parser_match (parser, TOKEN_IF))
    result->d.if_s.branch_b = parser_parse_statement_if (parser);
  else
    result->d.if_s.branch_b = parser_parse_statement_compound (parser);

  return result;
}


static struct tree *
parser_parse_statement_while (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_WHILE);

  parser_expect_advance (parser, TOKEN_WHILE);

  result->d.while_s.condition = parser_parse_expression_assignment (parser);

  result->d.while_s.body = parser_parse_statement_compound (parser);

  return result;
}


static struct tree *
parser_parse_statement_for (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_FOR);

  parser_expect_advance (parser, TOKEN_FOR);

  // Optional init
  if (parser_match (parser, TOKEN_SEMICOLON))
    result->d.for_s.init = NULL;
  else
    result->d.for_s.init = parser_parse_expression_assignment (parser);

  parser_expect_advance (parser, TOKEN_SEMICOLON);

  // Required condition
  result->d.for_s.condition = parser_parse_expression_assignment (parser);

  parser_expect_advance (parser, TOKEN_SEMICOLON);

  // Optional increment
  if (parser_match (parser, TOKEN_LBRACE))
    result->d.for_s.increment = NULL;
  else
    result->d.for_s.increment = parser_parse_expression_assignment (parser);

  result->d.for_s.body = parser_parse_statement_compound (parser);

  return result;
}


static struct tree *
parser_parse_statement_compound (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_COMPOUND);

  parser_expect_advance (parser, TOKEN_LBRACE);

  while (!parser_match (parser, TOKEN_RBRACE))
    {
      struct tree *statement;

      statement = parser_parse_statement (parser);

      tree_append (&result->d.compound.statement1, statement);

      switch (statement->kind)
        {
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

  parser_advance (parser);

  return result;
}


// TODO: Initializer values for variable declarations.
static struct tree *
parser_parse_statement_vdeclaration (struct parser *parser)
{
  parser_expect (parser, TOKEN_IDENTIFIER);

  char *name = parser->current->d.s;

  struct tree *result;

  result = tree_create (parser->location, TREE_VDECLARATION);

  parser_advance (parser);

  parser_expect_advance (parser, TOKEN_COLON);

  result->d.vdeclaration.name = name;

  result->d.vdeclaration.type = parser_parse_type (parser);

  return result;
}


static struct tree *
parser_parse_statement_return (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_RETURN);

  parser_expect_advance (parser, TOKEN_RETURN);

  if (parser_match (parser, TOKEN_SEMICOLON))
    return result;

  result->d.return_s.value = parser_parse_expression_assignment (parser);

  return result;
}


static struct tree *
parser_parse_statement_print (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_PRINT);

  parser_expect_advance (parser, TOKEN_PRINT);

  result->d.print.value = parser_parse_expression_assignment (parser);

  return result;
}


static struct tree *
parser_parse_expression_assignment (struct parser *parser)
{
  struct tree *lhs;

  lhs = parser_parse_expression_binary (parser);

  if (!parser_match (parser, TOKEN_EQ))
    return lhs;

  parser_advance (parser);

  struct tree *rhs;

  rhs = parser_parse_expression_assignment (parser);

  struct tree *result;

  result = tree_create (lhs->location, TREE_ASSIGNMENT);

  result->d.assignment.lhs = lhs;
  result->d.assignment.rhs = rhs;

  return result;
}


static struct tree *
parser_parse_expression_binary_base (struct parser *parser, int p)
{
  struct tree *lhs;

  lhs = parser_parse_expression_cast (parser);

  struct operator operator;

  if (!parser_fetch_operator (parser->current, &operator))
    return lhs;

  while ((operator.a == ASSOCIATIVITY_L && operator.p > p) ||
         (operator.a == ASSOCIATIVITY_R && operator.p >= p))
    {
      parser_advance (parser);

      struct tree *rhs;

      if (operator.a == ASSOCIATIVITY_L)
        rhs = parser_parse_expression_binary_base (parser, operator.p);
      else
        rhs = parser_parse_expression_binary_base (parser, operator.p - 1);

      struct tree *binary;

      binary = tree_create (lhs->location, TREE_BINARY);

      binary->d.binary.o = operator.o;
      binary->d.binary.lhs = lhs;
      binary->d.binary.rhs = rhs;

      lhs = binary;

      if (!parser_fetch_operator (parser->current, &operator))
        return lhs;
    }

  return lhs;
}


static struct tree *
parser_parse_expression_binary (struct parser *parser)
{
  return parser_parse_expression_binary_base (parser, 0);
}


static struct tree *
parser_parse_expression_cast (struct parser *parser)
{
  struct tree *result;

  result = parser_parse_expression_postfix (parser);

  while (parser_match (parser, TOKEN_COLON))
    {
      parser_advance (parser);

      struct type *type;

      type = parser_parse_type (parser);

      struct tree *cast;

      cast = tree_create (result->location, TREE_CAST);

      cast->d.cast.value = result;
      cast->d.cast.type = type;

      result = cast;
    }

  return result;
}


static struct tree *
parser_parse_expression_postfix (struct parser *parser)
{
  struct tree *result;

  result = parser_parse_primary (parser);

  while (1)
    {
      if (parser_match (parser, TOKEN_DOT))
        {
          parser_advance (parser);

          parser_expect (parser, TOKEN_IDENTIFIER);

          char *field = parser->current->d.s;

          parser_advance (parser);

          struct tree *access;

          access = tree_create (result->location, TREE_ACCESS);

          access->d.access.s = result;
          access->d.access.field = field;

          result = access;
        }

      // NOTE: -> syntactic sugar
      else if (parser_match (parser, TOKEN_ARROW))
        {
          parser_advance (parser);

          parser_expect (parser, TOKEN_IDENTIFIER);

          char *field = parser->current->d.s;

          parser_advance (parser);

          // a->b = (*a).b
          struct tree *deref;

          deref = tree_create (result->location, TREE_DEREFERENCE);

          deref->d.dereference.value = result;

          struct tree *access;

          access = tree_create (result->location, TREE_ACCESS);

          access->d.access.s = deref;
          access->d.access.field = field;

          result = access;
        }

      // NOTE: [] syntactic sugar
      else if (parser_match (parser, TOKEN_LBRACKET))
        {
          parser_advance (parser);

          struct tree *index;

          index = parser_parse_expression_assignment (parser);

          parser_expect_advance (parser, TOKEN_RBRACKET);

          // a[b] = *(a + b)
          struct tree *add;

          add = tree_create (result->location, TREE_BINARY);

          add->d.binary.lhs = result;
          add->d.binary.rhs = index;
          add->d.binary.o = BINARY_ADD;

          struct tree *deref;

          deref = tree_create (result->location, TREE_DEREFERENCE);

          deref->d.dereference.value = add;

          result = deref;
        }

      else if (parser_match (parser, TOKEN_LPAREN))
        {
          struct tree *call;

          call = tree_create (result->location, TREE_CALL);

          call->d.call.callee = result;

          parser_advance (parser);

          while (!parser_match (parser, TOKEN_RPAREN))
            {
              struct tree *argument;

              argument = parser_parse_expression_assignment (parser);

              tree_append (&call->d.call.argument1, argument);

              if (parser_match (parser, TOKEN_COMMA))
                parser_advance (parser);
              else
                parser_expect (parser, TOKEN_RPAREN);
            }

          parser_expect_advance (parser, TOKEN_RPAREN);

          result = call;
        }

      else
        break;
    }

  return result;
}


/*
static struct tree *
parser_parse_expression_call (struct parser *parser)
{
  struct tree *result;

  result = parser_parse_primary (parser);

  while (parser_match (parser, TOKEN_LPAREN))
    {
      struct tree *call;

      call = tree_create (result->location, TREE_CALL);

      call->d.call.callee = result;

      parser_advance (parser);

      while (!parser_match (parser, TOKEN_RPAREN))
        {
          struct tree *argument;

          argument = parser_parse_expression_assignment (parser);

          tree_append (&call->d.call.argument1, argument);

          if (parser_match (parser, TOKEN_COMMA))
            parser_advance (parser);
          else
            parser_expect (parser, TOKEN_RPAREN);
        }

      parser_expect_advance (parser, TOKEN_RPAREN);

      result = call;
    }

  return result;
}
*/


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
    case TOKEN_STRING:
      return parser_parse_primary_string (parser);
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
  struct location location = parser->location;

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
  struct tree *result;

  result = tree_create (parser->location, TREE_REFERENCE);

  parser_expect_advance (parser, TOKEN_AMPERSAND);

  struct tree *value;

  value = parser_parse_expression_postfix (parser);

  result->d.reference.value = value;

  return result;
}


static struct tree *
parser_parse_primary_dereference (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_DEREFERENCE);

  parser_expect_advance (parser, TOKEN_STAR);

  struct tree *value;

  value = parser_parse_expression_postfix (parser);

  result->d.dereference.value = value;

  return result;
}


static struct tree *
parser_parse_primary_integer (struct parser *parser)
{
  parser_expect (parser, TOKEN_INTEGER);

  struct tree *result;

  result = tree_create (parser->location, TREE_INTEGER);

  result->d.integer.value = parser->current->d.i;

  parser_advance (parser);

  return result;
}


static struct tree *
parser_parse_primary_string (struct parser *parser)
{
  parser_expect (parser, TOKEN_STRING);

  struct tree *result;

  result = tree_create (parser->location, TREE_STRING);

  result->d.string.value = parser->current->d.s;

  parser_advance (parser);

  return result;

}


static struct tree *
parser_parse_primary_identifier (struct parser *parser)
{
  parser_expect (parser, TOKEN_IDENTIFIER);

  struct tree *result;

  result = tree_create (parser->location, TREE_IDENTIFIER);

  result->d.identifier .value = parser->current->d.s;

  parser_advance (parser);

  return result;
}


static struct tree *
parser_parse_program (struct parser *parser)
{
  struct tree *result;

  result = tree_create (parser->location, TREE_PROGRAM);

  while (!parser_match (parser, TOKEN_EOF))
    {
      struct tree *top;

      top = parser_parse_top (parser);

      tree_append (&result->d.compound.statement1, top);

      switch (top->kind)
        {
        case TREE_FDEFINITION:
        case TREE_STRUCT:
          break; // No semicolon!
        default:
          parser_expect_advance (parser, TOKEN_SEMICOLON);
          break;
        }
    }

  return result;
}


static struct type *
parser_parse_type_primary (struct parser *parser)
{
  struct location location = parser->location;

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


static struct type *
parser_parse_type (struct parser *parser)
{
  struct location location = parser->location;

  switch (parser->current->kind)
    {
    case TOKEN_STAR:
      parser_advance (parser);

      return type_create_pointer (location, parser_parse_type (parser));

    case TOKEN_LBRACKET:
      {
        parser_advance (parser);

        // TODO: Implement constant expressions.
        parser_expect (parser, TOKEN_INTEGER);

        size_t size = parser->current->d.i;

        parser_advance (parser);

        parser_expect_advance (parser, TOKEN_RBRACKET);

        return type_create_array (location, size, parser_parse_type (parser));
      }
      break;

    case TOKEN_STRUCT:
      {
        parser_advance (parser);

        struct type *result;

        result = type_create (location, TYPE_STRUCT_NAME);

        parser_expect (parser, TOKEN_IDENTIFIER);

        result->d.struct_name.name = parser->current->d.s;

        parser_advance (parser);

        return result;
      }
      break;

    case TOKEN_FN:
      {
        parser_advance (parser);

        struct type *result;

        result = type_create (location, TYPE_FUNCTION);

        parser_expect_advance (parser, TOKEN_LPAREN);

        while (!parser_match (parser, TOKEN_RPAREN))
          {
            struct type *parameter_type;

            parameter_type = type_decay (parser_parse_type (parser));

            type_append (&result->d.function.from1, parameter_type);

            if (parser_match (parser, TOKEN_COMMA))
              parser_advance (parser);
            else
              parser_expect (parser, TOKEN_RPAREN);
          }

        parser_expect_advance (parser, TOKEN_RPAREN);

        if (parser_match (parser, TOKEN_ARROW))
          {
            parser_advance (parser);

            result->d.function.to = type_decay (parser_parse_type (parser));
          }
        else
          result->d.function.to = type_create (location, TYPE_VOID);

        return type_create_pointer (location, result);
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

  struct tree *result;

  result = parser_parse_program (parser);

  parser_expect (parser, TOKEN_EOF);

  return result;
}

