#ifndef TREE_H
#define TREE_H


#include "error.h"

#include <stdbool.h>


enum unary_operator
{
  UNARY_NEG,
  UNARY_BNOT,
  UNARY_LNOT,
};


const char *unary_operator_string (enum unary_operator);


enum binary_operator
{
  BINARY_ADD,
  BINARY_SUB,
  BINARY_MUL,
  BINARY_DIV,
  BINARY_MOD,

  BINARY_SHL,
  BINARY_SHR,
  BINARY_BOR,
  BINARY_BAND,
  BINARY_BXOR,

  BINARY_CMP_EQ,
  BINARY_CMP_NE,
  BINARY_CMP_L,
  BINARY_CMP_G,
  BINARY_CMP_LE,
  BINARY_CMP_GE,
};


const char *binary_operator_string (enum binary_operator);


enum tree_kind
{
  // Top-level
  TREE_FDECLARATION,
  TREE_FDEFINITION,

  TREE_STRUCT,

  // Statement
  TREE_IF,
  TREE_WHILE,
  TREE_FOR,
  TREE_COMPOUND,
  TREE_VDECLARATION,
  TREE_RETURN,
  TREE_BREAK,
  TREE_CONTINUE,
  TREE_PRINT,

  // Expression
  TREE_IMPLICIT_SCALE,

  TREE_CAST,
  TREE_CALL,
  TREE_ASSIGNMENT,
  TREE_ACCESS,
  TREE_OR,
  TREE_AND,
  TREE_UNARY,
  TREE_BINARY,
  TREE_REFERENCE,
  TREE_DEREFERENCE,
  TREE_INTEGER,
  TREE_STRING,
  TREE_IDENTIFIER,

  // Miscellaneous
  TREE_PROGRAM,
};


const char *tree_kind_string (enum tree_kind);


struct tree;
struct type;


struct tree_node_fdeclaration
{
  char *name;

  struct type *function_type;
};


struct tree_node_fdefinition
{
  char *name;

  struct tree *parameter1;
  struct tree *body;

  struct type *function_type;
};


struct tree_node_struct
{
  char *name;

  struct tree *field1;

  struct type *struct_type;
};


struct tree_node_if
{
  struct tree *condition;
  struct tree *branch_a;
  struct tree *branch_b;
};


struct tree_node_while
{
  struct tree *condition;
  struct tree *body;
};


struct tree_node_for
{
  struct tree *init;
  struct tree *condition;
  struct tree *increment;
  struct tree *body;
};


struct tree_node_compound
{
  struct tree *statement1;
};


struct tree_node_vdeclaration
{
  char *name;

  struct type *variable_type;
};


struct tree_node_return
{
  struct tree *value;
};


struct tree_node_print
{
  struct tree *value;
};


struct tree_node_scale
{
  struct tree *value;

  struct type *expression_type;
  struct type *base_type;
};


struct tree_node_cast
{
  struct tree *value;

  struct type *expression_type;
};


struct tree_node_call
{
  struct tree *callee;
  struct tree *argument1;

  struct type *expression_type;
};


struct tree_node_assignment
{
  struct tree *lhs;
  struct tree *rhs;

  struct type *expression_type;
};


struct tree_node_access
{
  struct tree *value;

  char *field;

  struct type *expression_type;
};


struct tree_node_or
{
  struct tree *lhs;
  struct tree *rhs;

  struct type *expression_type;
};


struct tree_node_and
{
  struct tree *lhs;
  struct tree *rhs;

  struct type *expression_type;
};


struct tree_node_unary
{
  enum unary_operator operator;

  struct tree *value;

  struct type *expression_type;
};


struct tree_node_binary
{
  enum binary_operator operator;

  struct tree *lhs;
  struct tree *rhs;

  struct type *expression_type;
};


struct tree_node_reference
{
  struct tree *value;

  struct type *expression_type;
};


struct tree_node_dereference
{
  struct tree *value;

  struct type *expression_type;
};


struct tree_node_integer
{
  long value;

  struct type *expression_type;
};


struct tree_node_string
{
  char *value;

  struct type *expression_type;
};


struct tree_node_identifier
{
  char *value;

  struct type *expression_type;
};


struct tree_node_program
{
  struct tree *top_level1;
};


union tree_data
{
  struct tree_node_fdeclaration fdeclaration;
  struct tree_node_fdefinition  fdefinition;

  struct tree_node_struct       struct_;

  struct tree_node_if           if_;
  struct tree_node_while        while_;
  struct tree_node_for          for_;
  struct tree_node_compound     compound;
  struct tree_node_vdeclaration vdeclaration;
  struct tree_node_return       return_;
  struct tree_node_print        print;

  struct tree_node_scale        scale;

  struct tree_node_cast         cast;
  struct tree_node_call         call;
  struct tree_node_assignment   assignment;
  struct tree_node_access       access;
  struct tree_node_or           or;
  struct tree_node_and          and;
  struct tree_node_unary        unary;
  struct tree_node_binary       binary;
  struct tree_node_reference    reference;
  struct tree_node_dereference  dereference;
  struct tree_node_integer      integer;
  struct tree_node_string       string;
  struct tree_node_identifier   identifier;

  struct tree_node_program      program;
};


struct tree
{
  struct location location;

  union tree_data d;

  struct tree *next;

  enum tree_kind kind;
};


struct tree *tree_create (struct location, enum tree_kind);


void tree_append (struct tree **, struct tree *);


struct type **tree_get_expression_type_p (struct tree *);

struct type *tree_get_expression_type (struct tree *);


bool tree_is_lvalue (struct tree *);

bool tree_is_rvalue (struct tree *);


void tree_print (struct tree *, int);


#endif // TREE_H

