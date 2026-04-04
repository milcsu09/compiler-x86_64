#include "fold.h"
#include "tree.h"
#include "type.h"

#include <stdlib.h>


bool
fold_unary_i64 (int64_t *r, enum unary_operator operator, int64_t value)
{
  switch (operator)
    {
    case UNARY_NEG:
      *r = -value;
      return true;
    case UNARY_BNOT:
      *r = ~value;
      return true;
    case UNARY_LNOT:
      *r = !value;
      return true;
    default:
      return unreachable1(false);
    }
}


bool
fold_binary_i64 (int64_t *r, enum binary_operator operator, int64_t lhs, int64_t rhs)
{
  switch (operator)
    {
    case BINARY_ADD:
      *r = lhs + rhs;
      return true;
    case BINARY_SUB:
      *r = lhs - rhs;
      return true;
    case BINARY_MUL:
      *r = lhs * rhs;
      return true;
    case BINARY_DIV:
      if (rhs == 0)
        return false;
      *r = lhs / rhs;
      return true;
    case BINARY_MOD:
      if (rhs == 0)
        return false;
      *r = lhs % rhs;
      return true;
    case BINARY_SHL:
      *r = lhs << rhs;
      return true;
    case BINARY_SHR:
      *r = lhs >> rhs;
      return true;
    case BINARY_BOR:
      *r = lhs | rhs;
      return true;
    case BINARY_BAND:
      *r = lhs & rhs;
      return true;
    case BINARY_BXOR:
      *r = lhs ^ rhs;
      return true;

    // NOTE: For now, comparisons will NOT be folded
    // case BINARY_CMP_EQ:
    //   *r = lhs == rhs;
    //   return true;
    // case BINARY_CMP_NE:
    //   *r = lhs != rhs;
    //   return true;
    // case BINARY_CMP_L:
    //   *r = lhs < rhs;
    //   return true;
    // case BINARY_CMP_G:
    //   *r = lhs > rhs;
    //   return true;
    // case BINARY_CMP_LE:
    //   *r = lhs <= rhs;
    //   return true;
    // case BINARY_CMP_GE:
    //   *r = lhs >= rhs;
    //   return true;

    case BINARY_CMP_EQ:
    case BINARY_CMP_NE:
    case BINARY_CMP_L:
    case BINARY_CMP_G:
    case BINARY_CMP_LE:
    case BINARY_CMP_GE:
      return false;

    default:
      return unreachable1(false);
    }
}


bool
fold_tree (struct tree **result, struct tree *tree)
{
  switch (tree->kind)
    {
    case TREE_UNARY:
      {
        struct tree_node_unary unary = tree->d.unary;

        bool is_integer = unary.value->kind == TREE_INTEGER;

        if (is_integer)
          {
            int value = unary.value->d.integer.value;

            int64_t r;

            bool success = fold_unary_i64 (&r, unary.operator, value);

            if (success)
              {
                *result = tree_create_integer (tree->location, r);

                return true;
              }

            // fallthrough
          }

        // fallthrough
      }
      break;

    case TREE_BINARY:
      {
        struct tree_node_binary binary = tree->d.binary;

        bool lhs_is_integer = binary.lhs->kind == TREE_INTEGER;
        bool rhs_is_integer = binary.rhs->kind == TREE_INTEGER;

        if (lhs_is_integer && rhs_is_integer)
          {
            int64_t lhs = binary.lhs->d.integer.value;
            int64_t rhs = binary.rhs->d.integer.value;

            int64_t r;

            bool success = fold_binary_i64 (&r, binary.operator, lhs, rhs);

            if (success)
              {
                *result = tree_create_integer (tree->location, r);

                return true;
              }

            // fallthrough
          }

        // fallthrough
      }
      break;

    case TREE_INTEGER:
      *result = tree;
      return true;

    default:
      break;
    }

  return false;
}


struct tree *
fold_tree_strict (struct tree *tree)
{
  struct tree *result = NULL;

  bool success = fold_tree (&result, tree);

  if (!success)
    {
      error (tree->location, "expression is not a compile-time constant");

      exit (1);
    }

  return result;
}
