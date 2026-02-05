#include "analyzer.h"
#include "alignment.h"
#include "tree.h"
#include "type.h"
#include "scope.h"
#include "memory.h"

#include <assert.h>
#include <stdlib.h>


struct analyzer
{
  struct tree_node_fdefinition *function;

  struct scope *scope;
  struct scope *scope_struct;

  size_t loop_depth; // Depth of "while"- and "for"-loops
};


static void
analyzer_scope_push (struct analyzer *analyzer, struct scope *scope)
{
  analyzer->scope = scope;
}


static void
analyzer_scope_pop (struct analyzer *analyzer)
{
  analyzer->scope = analyzer->scope->parent;
}


struct analyzer *
analyzer_create (void)
{
  struct analyzer *analyzer;

  analyzer = aa_malloc (sizeof (struct analyzer));

  analyzer->scope        = scope_create (NULL);
  analyzer->scope_struct = scope_create (NULL);

  return analyzer;
}


static struct tree *
analyzer_tree_create_cast (struct tree *value, struct type *cast_type)
{
  struct type *value_type = tree_get_expression_type (value);

  if (type_cast_required (value_type, cast_type))
    {
      struct tree *result;

      result = tree_create (value->location, TREE_CAST);

      result->d.cast.value = value;

      result->d.cast.expression_type = cast_type;

      result->next = value->next;

      value->next = NULL;

      return result;
    }

  return value;
}


static struct tree *
analyzer_tree_create_scale (struct tree *value, struct type *base_type)
{
  struct tree *result;

  result = tree_create (value->location, TREE_IMPLICIT_SCALE);

  result->d.scale.value = value;

  result->d.scale.expression_type = tree_get_expression_type (value);

  result->d.scale.base_type = base_type;

  result->next = value->next;

  value->next = NULL;

  return result;
}


static void analyzer_analyze_node_fdeclaration (struct analyzer *, struct tree *);

static void analyzer_analyze_node_fdefinition (struct analyzer *, struct tree *);

static void analyzer_analyze_node_struct (struct analyzer *, struct tree *);


static void analyzer_analyze_node_if (struct analyzer *, struct tree *);

static void analyzer_analyze_node_while (struct analyzer *, struct tree *);

static void analyzer_analyze_node_for (struct analyzer *, struct tree *);

static void analyzer_analyze_node_compound (struct analyzer *, struct tree *);

static void analyzer_analyze_node_vdeclaration_into (struct analyzer *, struct tree *,
                                                     struct scope *);

static void analyzer_analyze_node_vdeclaration (struct analyzer *, struct tree *);

static void analyzer_analyze_node_return (struct analyzer *, struct tree *);

static void analyzer_analyze_node_break (struct analyzer *, struct tree *);

static void analyzer_analyze_node_continue (struct analyzer *, struct tree *);

static void analyzer_analyze_node_print (struct analyzer *, struct tree *);


static struct tree *analyzer_analyze_node_implicit_scale (struct analyzer *, struct tree *);


static struct tree *analyzer_analyze_node_cast (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_call (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_assignment (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_access (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_or (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_and (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_unary (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_binary (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_reference (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_dereference (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_integer (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_string (struct analyzer *, struct tree *);

static struct tree *analyzer_analyze_node_identifier (struct analyzer *, struct tree *);


static void analyzer_analyze_node_program (struct analyzer *, struct tree *);


// Handles undefined types.
struct type *
analyzer_analyze_type (struct analyzer *analyzer, struct type *type)
{
  if (type == TYPE_ERROR)
    return type;

  switch (type->kind)
    {
    case TYPE_POINTER:
      {
        struct type_node_pointer *pointer = &type->d.pointer;

        if (!type_is_named (pointer->base))
          pointer->base = analyzer_analyze_type (analyzer, pointer->base);

        return type;
      }

    case TYPE_ARRAY:
      {
        struct type_node_array *array = &type->d.array;

        array->base = analyzer_analyze_type (analyzer, array->base);

        return type;
      }

    case TYPE_FUNCTION:
      {
        struct type_node_function *function = &type->d.function;

        struct type *prev = NULL;
        struct type *curr = function->from1;

        while (curr)
          {
            struct type *next = analyzer_analyze_type (analyzer, curr);

            if (next->kind == TYPE_STRUCT)
              {
                error (type->location, "WIP 'struct'-type as parameter");

                exit (1);
              }

            if (prev)
              prev->next = next;
            else
              function->from1 = next;

            prev = next;
            curr = next->next;
          }

        function->to = analyzer_analyze_type (analyzer, function->to);

        if (function->to->kind == TYPE_STRUCT)
          {
            error (type->location, "WIP 'struct'-type as return");

            exit (1);
          }

        return type;
      }

    case TYPE_STRUCT:
      {
        struct type_node_struct *struct_ = &type->d.struct_;

        struct type *prev = NULL;
        struct type *curr = struct_->field1;

        while (curr)
          {
            struct type *next = analyzer_analyze_type (analyzer, curr);

            if (prev)
              prev->next = next;
            else
              struct_->field1 = next;

            prev = next;
            curr = next->next;
          }

        return type;
      }

    case TYPE_STRUCT_NAME:
      {
        struct type_node_struct_name *struct_name = &type->d.struct_name;

        struct symbol *symbol;

        // NOTE: Ignore invalidity.
        enum scope_get_result result = scope_get (analyzer->scope_struct, struct_name->name,
                                                  &symbol);

        switch (result)
          {
          case SCOPE_GET_OK:
            return type_shallow_copy (symbol->type);

          default:
            error (type->location, "undefined type 'struct %s'", struct_name->name);

            exit (1);
          }
      }

    default:
      return type;
    }
}


struct tree *
analyzer_analyze_expression_and_type (struct analyzer *analyzer, struct tree *tree)
{
  struct type **type = tree_get_expression_type_p (tree);

  *type = analyzer_analyze_type (analyzer, *type);

  switch (tree->kind)
    {
    case TREE_IMPLICIT_SCALE:
      return analyzer_analyze_node_implicit_scale (analyzer, tree);
    case TREE_CAST:
      return analyzer_analyze_node_cast (analyzer, tree);
    case TREE_CALL:
      return analyzer_analyze_node_call (analyzer, tree);
    case TREE_ASSIGNMENT:
      return analyzer_analyze_node_assignment (analyzer, tree);
    case TREE_ACCESS:
      return analyzer_analyze_node_access (analyzer, tree);
    case TREE_OR:
      return analyzer_analyze_node_or (analyzer, tree);
    case TREE_AND:
      return analyzer_analyze_node_and (analyzer, tree);
    case TREE_UNARY:
      return analyzer_analyze_node_unary (analyzer, tree);
    case TREE_BINARY:
      return analyzer_analyze_node_binary (analyzer, tree);
    case TREE_REFERENCE:
      return analyzer_analyze_node_reference (analyzer, tree);
    case TREE_DEREFERENCE:
      return analyzer_analyze_node_dereference (analyzer, tree);
    case TREE_INTEGER:
      return analyzer_analyze_node_integer (analyzer, tree);
    case TREE_STRING:
      return analyzer_analyze_node_string (analyzer, tree);
    case TREE_IDENTIFIER:
      return analyzer_analyze_node_identifier (analyzer, tree);
    default:
      return unreachable1 (NULL);
    }
}


static struct tree *
analyzer_analyze_lvalue_and_type (struct analyzer *analyzer, struct tree *tree)
{
  struct tree *value = analyzer_analyze_expression_and_type (analyzer, tree);

  if (tree_is_rvalue (value))
    {
      error (tree->location, "right-value expression used as left-value");

      exit (1);
    }

  return value;
}


static struct tree *
analyzer_analyze_rvalue_and_type (struct analyzer *analyzer, struct tree *tree)
{
  struct tree *value = analyzer_analyze_expression_and_type (analyzer, tree);

  struct type *value_type = tree_get_expression_type (value);

  if (tree_is_lvalue (value))
    {
      if (type_is_label (value_type))
        {
          struct tree *result;

          result = tree_create (tree->location, TREE_REFERENCE);

          result->d.reference.value = value;

          struct type *decayed_type = type_decay (value_type);

          result->d.reference.expression_type = decayed_type;

          value_type = decayed_type;

          result->next = value->next;

          value->next = NULL;

          value = result;
        }
    }

  if (!type_is_scalar (value_type))
    {
      char name[512];

      type_string (value_type, name, sizeof name);

      error (tree->location, "non-scalar type '%s' used as right-value", name);

      exit (1);
    }

  return value;
}


static void
analyzer_analyze_statement (struct analyzer *analyzer, struct tree *tree)
{
  if (!tree)
    return;

  switch (tree->kind)
    {
    case TREE_FDECLARATION:
      analyzer_analyze_node_fdeclaration (analyzer, tree);
      break;
    case TREE_FDEFINITION:
      analyzer_analyze_node_fdefinition (analyzer, tree);
      break;
    case TREE_STRUCT:
      analyzer_analyze_node_struct (analyzer, tree);
      break;
    case TREE_IF:
      analyzer_analyze_node_if (analyzer, tree);
      break;
    case TREE_WHILE:
      analyzer_analyze_node_while (analyzer, tree);
      break;
    case TREE_FOR:
      analyzer_analyze_node_for (analyzer, tree);
      break;
    case TREE_COMPOUND:
      analyzer_analyze_node_compound (analyzer, tree);
      break;
    case TREE_VDECLARATION:
      analyzer_analyze_node_vdeclaration (analyzer, tree);
      break;
    case TREE_RETURN:
      analyzer_analyze_node_return (analyzer, tree);
      break;
    case TREE_BREAK:
      analyzer_analyze_node_break (analyzer, tree);
      break;
    case TREE_CONTINUE:
      analyzer_analyze_node_continue (analyzer, tree);
      break;
    case TREE_PRINT:
      analyzer_analyze_node_print (analyzer, tree);
      break;
    case TREE_PROGRAM:
      analyzer_analyze_node_program (analyzer, tree);
      break;

    default:
      analyzer_analyze_expression_and_type (analyzer, tree);
      break;
    }
}


static void
analyzer_analyze_node_fdeclaration (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_fdeclaration *fdeclaration = &tree->d.fdeclaration;

  fdeclaration->function_type = analyzer_analyze_type (analyzer, fdeclaration->function_type);

  struct symbol *symbol = symbol_create (SYMBOL_GLOBAL, fdeclaration->name,
                                         fdeclaration->function_type);

  scope_set_validate (analyzer->scope, symbol, tree->location);
}


static void
analyzer_analyze_node_fdefinition (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_fdefinition *fdefinition = &tree->d.fdefinition;

  fdefinition->function_type = analyzer_analyze_type (analyzer, fdefinition->function_type);

  analyzer->function = fdefinition;

  struct symbol *symbol = symbol_create (SYMBOL_GLOBAL, fdefinition->name,
                                         fdefinition->function_type);

  scope_set_validate (analyzer->scope, symbol, tree->location);

  analyzer_scope_push (analyzer, scope_create (analyzer->scope));

  for (struct tree *t = fdefinition->parameter1; t; t = t->next)
    {
      assert (t->kind == TREE_VDECLARATION);

      analyzer_analyze_node_vdeclaration (analyzer, t);
    }

  for (struct tree *t = fdefinition->body->d.compound.statement1; t; t = t->next)
    analyzer_analyze_statement (analyzer, t);

  analyzer_scope_pop (analyzer);
}


static void
analyzer_analyze_node_struct (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_struct *struct_ = &tree->d.struct_;

  struct type_node_struct *struct_type = &struct_->struct_type->d.struct_;

  struct_type->scope = scope_create (NULL);

  struct type *prev_type = struct_type->field1;

  size_t offset = 0;

  for (struct tree *t = struct_->field1; t; t = t->next)
    {
      assert (t->kind == TREE_VDECLARATION);

      analyzer_analyze_node_vdeclaration_into (analyzer, t, struct_type->scope);

      struct type *curr_type = tree_get_expression_type (t);

      size_t alignment = type_alignment (curr_type);
      size_t size = type_size (curr_type);

      struct symbol *symbol = struct_type->scope->head;

      symbol->kind = SYMBOL_FIELD;

      offset = align_forward (offset, alignment);

      symbol->d.field.offset = offset;

      offset = offset + size;

      if (prev_type)
        prev_type->next = curr_type;
      else
        struct_type->field1 = curr_type;

      prev_type = curr_type;
    }

  struct symbol *symbol = symbol_create (SYMBOL_GLOBAL, struct_->name, struct_->struct_type);

  scope_set_validate (analyzer->scope_struct, symbol, tree->location);

  struct_type->name = struct_->name;
}


static void
analyzer_analyze_node_if (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_if *if_ = &tree->d.if_;

  if_->condition = analyzer_analyze_rvalue_and_type (analyzer, if_->condition);

  analyzer_analyze_statement (analyzer, if_->branch_a);

  if (if_->branch_b)
    analyzer_analyze_statement (analyzer, if_->branch_b);
}


static void
analyzer_analyze_node_while (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_while *while_ = &tree->d.while_;

  while_->condition = analyzer_analyze_rvalue_and_type (analyzer, while_->condition);

  analyzer->loop_depth++;

  analyzer_analyze_statement (analyzer, while_->body);

  analyzer->loop_depth--;
}


static void
analyzer_analyze_node_for (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_for *for_ = &tree->d.for_;

  for_->init = analyzer_analyze_expression_and_type (analyzer, for_->init);

  for_->condition = analyzer_analyze_rvalue_and_type (analyzer, for_->condition);

  for_->increment = analyzer_analyze_expression_and_type (analyzer, for_->increment);

  analyzer->loop_depth++;

  analyzer_analyze_statement (analyzer, for_->body);

  analyzer->loop_depth--;
}


static void
analyzer_analyze_node_compound (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_compound *compound = &tree->d.compound;

  analyzer_scope_push (analyzer, scope_create (analyzer->scope));

  for (struct tree *t = compound->statement1; t; t = t->next)
    analyzer_analyze_statement (analyzer, t);

  analyzer_scope_pop (analyzer);
}


static void
analyzer_analyze_node_vdeclaration_into (struct analyzer *analyzer, struct tree *tree,
                                         struct scope *scope)
{
  struct tree_node_vdeclaration *vdeclaration = &tree->d.vdeclaration;

  vdeclaration->variable_type = analyzer_analyze_type (analyzer, vdeclaration->variable_type);

  struct symbol *symbol
      = symbol_create (SYMBOL_LOCAL, vdeclaration->name, vdeclaration->variable_type);

  scope_set_validate (scope, symbol, tree->location);

}


static void
analyzer_analyze_node_vdeclaration (struct analyzer *analyzer, struct tree *tree)
{
  analyzer_analyze_node_vdeclaration_into (analyzer, tree, analyzer->scope);
}


static void
analyzer_analyze_node_return (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_return *return_ = &tree->d.return_;

  struct type_node_function type_function = analyzer->function->function_type->d.function;

  if (return_->value)
    {
      return_->value = analyzer_analyze_rvalue_and_type (analyzer, return_->value);

      if (type_function.to->kind != TYPE_VOID)
        return_->value = analyzer_tree_create_cast (return_->value, type_function.to);
      else
        {
          error (tree->location, "'return' used with expression inside a 'void' function");

          exit (1);
        }
    }
  else
    if (type_function.to->kind != TYPE_VOID)
      {
        error (tree->location, "'return' used without expression inside a non-'void' function");

        exit (1);
      }
}


static void
analyzer_analyze_node_break (struct analyzer *analyzer, struct tree *tree)
{
  if (analyzer->loop_depth == 0)
    {
      error (tree->location, "'break' used outside of a loop");

      exit (1);
    }
}


static void
analyzer_analyze_node_continue (struct analyzer *analyzer, struct tree *tree)
{
  if (analyzer->loop_depth == 0)
    {
      error (tree->location, "'continue' used outside of a loop");

      exit (1);
    }
}


static void
analyzer_analyze_node_print (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_print *print = &tree->d.print;

  print->value = analyzer_analyze_rvalue_and_type (analyzer, print->value);

  print->value = analyzer_tree_create_cast (print->value, type_create (tree->location, TYPE_I64));
}


static struct tree *
analyzer_analyze_node_implicit_scale (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_scale *scale = &tree->d.scale;

  analyzer_analyze_rvalue_and_type (analyzer, scale->value);

  return tree;
}


static struct tree *
analyzer_analyze_node_cast (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_cast *cast = &tree->d.cast;

  cast->value = analyzer_analyze_rvalue_and_type (analyzer, cast->value);

  cast->expression_type = analyzer_analyze_type (analyzer, cast->expression_type);

  if (!type_is_scalar (cast->expression_type))
    {
      char name[512];

      type_string (cast->expression_type, name, sizeof name);

      error (tree->location, "cast to non-scalar type '%s'", name);

      exit (1);
    }

  if (!type_cast_required (cast->expression_type, tree_get_expression_type (cast->value)))
    return cast->value;

  return tree;
}


static struct tree *
analyzer_analyze_node_call (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_call *call = &tree->d.call;

  call->callee = analyzer_analyze_rvalue_and_type (analyzer, call->callee);

  struct type *callee_type = tree_get_expression_type (call->callee);

  // NOTE: For now, only function pointers can be called.
  if (!type_is_pointer_to_k (callee_type, TYPE_FUNCTION))
    {
      char name[512];

      type_string (callee_type, name, sizeof name);

      error (tree->location, "call to non-callable type '%s'", name);

      exit (1);
    }

  struct type_node_function type_function = callee_type->d.pointer.base->d.function;

  struct tree *curr = NULL;

  struct tree *curr_argument = call->argument1;
  struct type *curr_type = type_function.from1;

  size_t count = 0;

  while (curr_argument && curr_type)
    {
      struct tree *argument;

      argument = analyzer_analyze_rvalue_and_type (analyzer, curr_argument);

      // First 6 arguments are casted to their expected type
      if (count < 6)
        argument = analyzer_tree_create_cast (argument, curr_type);

      // Rest of the arguments must be 64-bit, so they're correctly placed on the stack
      else
        argument = analyzer_tree_create_cast (argument, type_create (tree->location, TYPE_I64));

      if (curr)
        curr->next = argument;
      else
        call->argument1 = argument;

      curr = argument;

      curr_argument = argument->next;
      curr_type = curr_type->next;

      count++;
    }

  if (curr_argument)
    {
      error (tree->location, "too many arguments to function");
      exit (1);
    }

  if (curr_type)
    {
      error (tree->location, "too few arguments to function");
      exit (1);
    }

  call->expression_type = type_function.to;

  return tree;
}


static struct tree *
analyzer_analyze_node_assignment (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_assignment *assignment = &tree->d.assignment;

  assignment->lhs = analyzer_analyze_lvalue_and_type (analyzer, assignment->lhs);
  assignment->rhs = analyzer_analyze_rvalue_and_type (analyzer, assignment->rhs);

  struct type *lhs_type = tree_get_expression_type (assignment->lhs);

  if (type_is_assignable (lhs_type))
    {
      assignment->rhs = analyzer_tree_create_cast (assignment->rhs, lhs_type);

      assignment->expression_type = lhs_type;
    }
  else
    {
      char name[512];

      type_string (lhs_type, name, sizeof name);

      error (tree->location, "assignment to non-assignable type '%s'", name);

      exit (1);
    }

  return tree;
}


static struct tree *
analyzer_analyze_node_access (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_access *access = &tree->d.access;

  access->value = analyzer_analyze_lvalue_and_type (analyzer, access->value);

  struct type *type = tree_get_expression_type (access->value);

  if (type_is_composite (type))
    {
      assert (type->kind == TYPE_STRUCT);

      struct type_node_struct type_node = type->d.struct_;

      struct symbol *symbol;

      scope_get_validate (type_node.scope, access->field, &symbol, tree->location);

      access->expression_type = analyzer_analyze_type (analyzer, symbol->type);
    }
  else
    {
      char name[512];

      type_string (type, name, sizeof name);

      error (tree->location, "field access of non-composite type '%s'", name);

      exit (1);
    }

  return tree;
}


static struct tree *
analyzer_analyze_node_or (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_or *or = &tree->d.or;

  or->lhs = analyzer_analyze_rvalue_and_type (analyzer, or->lhs);
  or->rhs = analyzer_analyze_rvalue_and_type (analyzer, or->rhs);

  struct type *lhs_type = tree_get_expression_type (or->lhs);
  struct type *rhs_type = tree_get_expression_type (or->rhs);

  struct type *common = type_find_common (lhs_type, rhs_type);

  or->lhs = analyzer_tree_create_cast (or->lhs, common);
  or->rhs = analyzer_tree_create_cast (or->rhs, common);

  or->expression_type = common;

  return tree;
}


static struct tree *
analyzer_analyze_node_and (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_and *and = &tree->d.and;

  and->lhs = analyzer_analyze_rvalue_and_type (analyzer, and->lhs);
  and->rhs = analyzer_analyze_rvalue_and_type (analyzer, and->rhs);

  struct type *lhs_type = tree_get_expression_type (and->lhs);
  struct type *rhs_type = tree_get_expression_type (and->rhs);

  struct type *common = type_find_common (lhs_type, rhs_type);

  and->lhs = analyzer_tree_create_cast (and->lhs, common);
  and->rhs = analyzer_tree_create_cast (and->rhs, common);

  and->expression_type = common;

  return tree;
}


static struct tree *
analyzer_analyze_node_unary (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_unary *unary = &tree->d.unary;

  unary->value = analyzer_analyze_rvalue_and_type (analyzer, unary->value);

  struct type *type = tree_get_expression_type (unary->value);

  enum unary_operator operator = unary->operator;

  bool is_integer = type_is_integer (type);
  bool is_pointer = type_is_pointer (type);

  switch (operator)
    {
    case UNARY_NEG:
    case UNARY_BNOT:
      if (is_integer)
        {
          unary->value = analyzer_tree_create_cast (unary->value, type);

          unary->expression_type = type;

          return tree;
        }

      break;

    case UNARY_LNOT:
      if (is_integer || is_pointer)
        {
          unary->value = analyzer_tree_create_cast (unary->value, type);

          unary->expression_type = type_create (type->location, TYPE_U8);

          return tree;
        }

      break;

    default:
      unreachable ();
      break;
    }

  const char *operator_name = unary_operator_string (operator);

  char name[512];

  type_string (type, name, sizeof name);

  error (tree->location, "operator '%s' before '%s' is not allowed", operator_name, name);

  exit (1);
}


static struct tree *
analyzer_analyze_node_binary (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_binary *binary = &tree->d.binary;

  binary->lhs = analyzer_analyze_rvalue_and_type (analyzer, binary->lhs);
  binary->rhs = analyzer_analyze_rvalue_and_type (analyzer, binary->rhs);

  struct type *lhs_type = tree_get_expression_type (binary->lhs);
  struct type *rhs_type = tree_get_expression_type (binary->rhs);

  enum binary_operator operator = binary->operator;

  bool lhs_is_integer = type_is_integer (lhs_type);
  bool rhs_is_integer = type_is_integer (rhs_type);

  bool lhs_is_pointer = type_is_pointer (lhs_type);
  bool rhs_is_pointer = type_is_pointer (rhs_type);

  switch (operator)
    {
    case BINARY_ADD:
    case BINARY_SUB:
      if (lhs_is_integer && rhs_is_integer)
        {
          struct type *common = type_find_common (lhs_type, rhs_type);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = common;

          return tree;
        }

      if (lhs_is_integer && rhs_is_pointer)
        {
          binary->lhs
              = analyzer_tree_create_cast (binary->lhs, type_create (tree->location, TYPE_I64));

          binary->expression_type = rhs_type;

          struct type *type_base
              = analyzer_analyze_type (analyzer, type_element (binary->expression_type));

          binary->lhs = analyzer_tree_create_scale (binary->lhs, type_base);

          return tree;
        }

      if (lhs_is_pointer && rhs_is_integer)
        {
          binary->rhs
              = analyzer_tree_create_cast (binary->rhs, type_create (tree->location, TYPE_I64));

          binary->expression_type = lhs_type;

          struct type *type_base
              = analyzer_analyze_type (analyzer, type_element (binary->expression_type));

          binary->rhs = analyzer_tree_create_scale (binary->rhs, type_base);

          return tree;
        }

      if (operator == BINARY_SUB)
        if (lhs_is_pointer && rhs_is_pointer)
          {
            binary->expression_type = type_create (tree->location, TYPE_I64);

            return tree;
          }

      break;

    case BINARY_SHL:
    case BINARY_SHR:
    case BINARY_BOR:
    case BINARY_BAND:
    case BINARY_BXOR:
      if (lhs_is_integer && rhs_is_integer)
        {
          struct type *common = type_find_common (lhs_type, rhs_type);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = common;

          return tree;
        }

      break;

    case BINARY_MUL:
    case BINARY_DIV:
    case BINARY_MOD:
      if (lhs_is_integer && rhs_is_integer)
        {
          struct type *common = type_find_common (lhs_type, rhs_type);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = common;

          return tree;
        }

      break;

    case BINARY_CMP_EQ:
    case BINARY_CMP_NE:
    case BINARY_CMP_L:
    case BINARY_CMP_G:
    case BINARY_CMP_LE:
    case BINARY_CMP_GE:
      if (lhs_is_integer && rhs_is_integer)
        {
          struct type *common = type_find_common (lhs_type, rhs_type);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (lhs_is_integer && rhs_is_pointer)
        {
          struct type *common = type_create (tree->location, TYPE_I64);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (lhs_is_pointer && rhs_is_integer)
        {
          struct type *common = type_create (tree->location, TYPE_I64);

          binary->lhs = analyzer_tree_create_cast (binary->lhs, common);
          binary->rhs = analyzer_tree_create_cast (binary->rhs, common);

          binary->expression_type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      if (lhs_is_pointer && rhs_is_pointer)
        {
          binary->expression_type = type_create (tree->location, TYPE_U8);

          return tree;
        }

      break;

    default:
      unreachable ();
      break;
    }

  const char *operator_name = binary_operator_string (operator);

  char lhs_name[512];
  char rhs_name[512];

  type_string (lhs_type, lhs_name, sizeof lhs_name);
  type_string (rhs_type, rhs_name, sizeof rhs_name);

  error (tree->location, "operator '%s' between '%s' and '%s' is not allowed", operator_name,
         lhs_name, rhs_name);

  exit (1);
}


static struct tree *
analyzer_analyze_node_reference (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_reference *reference = &tree->d.reference;

  reference->value = analyzer_analyze_lvalue_and_type (analyzer, reference->value);

  reference->expression_type
      = type_create_pointer (tree->location, tree_get_expression_type (reference->value));

  return tree;
}


static struct tree *
analyzer_analyze_node_dereference (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_dereference *dereference = &tree->d.dereference;

  dereference->value = analyzer_analyze_rvalue_and_type (analyzer, dereference->value);

  struct type *value_type = tree_get_expression_type (dereference->value);

  if (type_is_pointer (value_type))
    dereference->expression_type = analyzer_analyze_type (analyzer, value_type->d.pointer.base);
  else
    {
      char name[512];

      type_string (value_type, name, sizeof name);

      error (tree->location, "dereference of non-pointer type '%s'", name);

      exit (1);
    }

  return tree;
}


static struct tree *
analyzer_analyze_node_integer (struct analyzer *analyzer, struct tree *tree)
{
  (void)analyzer;

  struct tree_node_integer *integer = &tree->d.integer;

  integer->expression_type = type_create (tree->location, TYPE_I64);

  return tree;
}


static struct tree *
analyzer_analyze_node_string (struct analyzer *analyzer, struct tree *tree)
{
  (void)analyzer;

  struct tree_node_string *string = &tree->d.string;

  string->expression_type
      = type_create_pointer (tree->location, type_create (tree->location, TYPE_U8));

  return tree;
}


static struct tree *
analyzer_analyze_node_identifier (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_identifier *identifier = &tree->d.identifier;

  struct symbol *symbol;

  scope_get_validate (analyzer->scope, identifier->value, &symbol, tree->location);

  identifier->expression_type = symbol->type;

  return tree;
}


static void
analyzer_analyze_node_program (struct analyzer *analyzer, struct tree *tree)
{
  struct tree_node_program *node = &tree->d.program;

  for (struct tree *t = node->top_level1; t; t = t->next)
    analyzer_analyze_statement (analyzer, t);
}


void
analyzer_analyze (struct analyzer *analyzer, struct tree *tree)
{
  analyzer_analyze_statement (analyzer, tree);
}

