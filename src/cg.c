#include "cg.h"
#include "memory.h"
#include "scope.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

static size_t
next_multiple (size_t i, size_t n)
{
  return (1 + (i - 1) / n) * n;
}


typedef size_t label_t;


typedef struct
{
  register_id id;

  enum type_width width;
} register_t;


static const register_t register_none = {
  .id = -1,
  .width = WIDTH_8,
};


static register_t
register_create (register_id id, enum type_width width)
{
  assert (id < REGISTER_N);

  register_t r;

  r.id = id;
  r.width = width;

  return r;
}


static register_t
register_modify (register_t r, enum type_width width)
{
  r.width = width;
  return r;
}


static const char *const REGISTER_STRING[] = {
  "r10b",
  "r11b",
  "r12b",
  "r13b",

  "r10w",
  "r11w",
  "r12w",
  "r13w",

  "r10d",
  "r11d",
  "r12d",
  "r13d",

  "r10",
  "r11",
  "r12",
  "r13",
};


static const char *
register_b_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_N * 0];
}


static const char *
register_w_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_N * 1];
}


static const char *
register_d_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_N * 2];
}


static const char *
register_q_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_N * 3];
}


const char *
register_string (register_t r)
{
  if (r.width <= 8)
    return register_b_string (r);

  if (r.width <= 16)
    return register_w_string (r);

  if (r.width <= 32)
    return register_d_string (r);

  if (r.width <= 64)
    return register_q_string (r);

  assert (!"invalid");
}


const char *
register_id_string (register_id i)
{
  return REGISTER_STRING[i + REGISTER_N * 3];
}


// CG


static const char *
operand_size_string (enum type_width width)
{
  switch (width)
    {
    case WIDTH_8:
      return "byte";
    case WIDTH_16:
      return "word";
    case WIDTH_32:
      return "dword";
    case WIDTH_64:
      return "qword";
    default:
      return "";
    }
}


static void
cg_scope_push (struct cg *cg)
{
  cg->scope = scope_create (cg->scope, 128);
}


static void
cg_scope_pop (struct cg *cg)
{
  cg->scope = cg->scope->parent;
}


static void cg_register_free_all (struct cg *);


struct cg *
cg_create (FILE *f)
{
  struct cg *cg;

  cg = aa_malloc (sizeof (struct cg));

  cg->f = f;

  cg->scope = NULL;

  cg_scope_push (cg);

  cg->stack_offset = 0;
  // cg->stack_head = 0;
  cg->stack_usage = 0;

  cg->label_count = 0;

  cg->register_spill = 0;

  cg_register_free_all (cg);

  return cg;
}


static label_t
cg_label (struct cg *cg)
{
  return cg->label_count++;
}


/// COMPUTE


static void
cg_compute (struct cg *cg, struct tree *tree)
{
  switch (tree->tree_kind)
    {
    case TREE_VARIABLE_DECLARATION:
      {
        size_t size = type_size (tree->type);

        cg->stack_offset = next_multiple (cg->stack_offset, size);
        cg->stack_offset = cg->stack_offset + size;
      }
      break;
    default:
      break;
    }

  if (tree->child)
    cg_compute (cg, tree->child);

  if (tree->next)
    cg_compute (cg, tree->next);
}


/// WRITE


static void
cg_write (struct cg *cg, const char *format, ...)
{
  va_list va;

  va_start (va, format);

  vfprintf (cg->f, format, va);

  va_end (va);
}


static void
cg_write_begin (struct cg *cg)
{
  cg_write (cg, "section .text\n");
  cg_write (cg, "\tglobal\tmain\n");
  cg_write (cg, "\textern\tprintf\n");

  cg_write (cg, "\n");

  cg_write (cg, "printi:\n");
  cg_write (cg, "\tmov\trsi, rdi\n");
  cg_write (cg, "\txor\teax, eax\n");
  cg_write (cg, "\tmov\tedi, printi_s\n");
  cg_write (cg, "\tjmp\tprintf\n");

  cg_write (cg, "\n");

  cg_write (cg, "main:\n");

  cg_write (cg, "\t; Enter\n");
  cg_write (cg, "\tpush\trbp\n");
  cg_write (cg, "\tmov\trbp, rsp\n");
  cg_write (cg, "\tsub\trsp, %zu\n", cg->stack_usage);
  cg_write (cg, "\t; Program\n");
}


static void
cg_write_end (struct cg *cg)
{
  cg_write (cg, "\t; Leave\n");
  cg_write (cg, "\tadd\trsp, %zu\n", cg->stack_usage);
  cg_write (cg, "\tpop\trbp\n");
  cg_write (cg, "\tmov\teax, 0\n");
  cg_write (cg, "\tret\n");

  cg_write (cg, "\n");

  cg_write (cg, "section .data\n");
  cg_write (cg, "\tprinti_s: db \"%%ld\", 10, 0\n");

  cg_write (cg, "\n");

  cg_write (cg, "section .note.GNU-stack\n");

  cg_write (cg, "\n");
}



static void
cg_write_nop (struct cg *cg)
{
  cg_write (cg, "\tnop\n");
}


static void
cg_write_push_register (struct cg *cg, register_id r)
{
  cg_write (cg, "\tpush\t%s\n", register_id_string (r));
}


static void
cg_write_pop_register (struct cg *cg, register_id r)
{
  cg_write (cg, "\tpop\t%s\n", register_id_string (r));
}


static void
cg_write_load_i (struct cg *cg, register_t r, long i)
{
  cg_write (cg, "\tmov\t%s, %ld\n", register_string (r), i);
}


static void
cg_write_print (struct cg *cg, register_t a)
{
  // size_t offset = next_multiple (cg->stack_head, 16) - cg->stack_head;

  // if (offset)
  //   cg_write (cg, "\tsub\trsp, %zu\n", offset);

  cg_write (cg, "\tmov\trdi, %s\n", register_string (a));
  cg_write (cg, "\tcall\tprinti\n");

  // if (offset)
  //   cg_write (cg, "\tadd\trsp, %zu\n", offset);
}


static void
cg_write_add (struct cg *cg, register_t a, register_t b)
{
  cg_write (cg, "\tadd\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sub (struct cg *cg, register_t a, register_t b)
{
  cg_write (cg, "\tsub\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_mul (struct cg *cg, register_t a, register_t b)
{
  cg_write (cg, "\timul\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sdiv (struct cg *cg, register_t a, register_t b)
{
  // TODO: generalize this
  const char *ax;

  if (a.width == 32)
    ax = "eax";
  else
    ax = "rax";

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));
  cg_write (cg, "\tcqo\n");
  cg_write (cg, "\tidiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), ax);
}


static void
cg_write_udiv (struct cg *cg, register_t a, register_t b)
{
  const char *ax;

  if (a.width == 32)
    ax = "eax";
  else
    ax = "rax";

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));

  cg_write (cg, "\txor\trdx, rdx\n");
  cg_write (cg, "\tdiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), ax);
}


static void
cg_write_compare (struct cg *cg, const char *i, register_t a, register_t b)
{
  cg_write (cg, "\tcmp\t%s, %s\n", register_string (a), register_string (b));
  cg_write (cg, "\t%s\t%s\n", i, register_b_string (a));
  // cg_write (cg, "\tand\t%s, 255\n", register_string (a));
}


static void
cg_write_compare_e (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "sete", a, b);
}


static void
cg_write_compare_ne (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setne", a, b);
}


static void
cg_write_scompare_l (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setl", a, b);
}


static void
cg_write_scompare_g (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setg", a, b);
}


static void
cg_write_scompare_le (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setle", a, b);
}


static void
cg_write_scompare_ge (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setge", a, b);
}


static void
cg_write_ucompare_l (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setb", a, b);
}


static void
cg_write_ucompare_g (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "seta", a, b);
}


static void
cg_write_ucompare_le (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setbe", a, b);
}


static void
cg_write_ucompare_ge (struct cg *cg, register_t a, register_t b)
{
  cg_write_compare (cg, "setae", a, b);
}


// static void
// cg_write_mov (struct cg *cg, register_t a, register_t b)
// {
//   cg_write (cg, "\tmov\t%s, %s\n", register_string (a), register_string (b));
// }


static void
cg_write_load (struct cg *cg, register_t a, register_t b)
{
  const char *s = operand_size_string (a.width);

  cg_write (cg, "\tmov\t%s, %s [%s]\n", register_string (a), s, register_string (b));
}


static void
cg_write_store (struct cg *cg, register_t a, register_t b)
{
  const char *s = operand_size_string (b.width);

  cg_write (cg, "\tmov\t%s [%s], %s\n", s, register_string (a), register_string (b));
}


static void
cg_write_zext (struct cg *cg, register_t a, register_t b)
{
  cg_write (cg, "\tmovzx\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sext (struct cg *cg, register_t a, register_t b)
{
  cg_write (cg, "\tmovsx\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_load_local (struct cg *cg, register_t r, size_t offset)
{
  const char *s = operand_size_string (r.width);

  cg_write (cg, "\tmov\t%s, %s [rbp-%zu]\n", register_string (r), s, offset);
}


static void
cg_write_load_local_address (struct cg *cg, register_t r, size_t offset)
{
  cg_write (cg, "\tlea\t%s, [rbp-%zu]\n", register_string (r), offset);
}


// static void
// cg_write_store_local (struct cg *cg, register_t r, size_t offset)
// {
//   cg_write (cg, "\tmov\t[rbp-%zu], %s\n", offset, register_string (r));
// }


static void
cg_write_test (struct cg *cg, register_t a)
{
  cg_write (cg, "\ttest\t%s, %s\n", register_string (a), register_string (a));
}


static void
cg_write_jump (struct cg *cg, label_t a, const char *ins)
{
  cg_write (cg, "\t%s\t.L%zu\n", ins, a);
}


static void
cg_write_jmp (struct cg *cg, label_t a)
{
  cg_write_jump (cg, a, "jmp");
}


static void
cg_write_jz (struct cg *cg, label_t a)
{
  cg_write_jump (cg, a, "jz");
}


static void
cg_write_jnz (struct cg *cg, label_t a)
{
  cg_write_jump (cg, a, "jnz");
}


static void
cg_write_label (struct cg *cg, label_t a)
{
  cg_write (cg, ".L%zu:\n", a);
}


/// REGISTER


static register_t
cg_register_allocate (struct cg *cg, enum type_width width)
{
  for (register_id i = 0; i < REGISTER_N; ++i)
    {
      if (cg->register_free[i] == 1)
        {
          cg->register_free[i] = 0;

          return register_create (i, width);
        }
    }

  register_id i = cg->register_spill++ % REGISTER_N;

  cg_write_push_register (cg, i);

  // cg->stack_head += 8;

  return register_create (i, width);
}


static void
cg_register_free (struct cg *cg, register_t r)
{
  if (cg->register_spill == 0)
    cg->register_free[r.id] = 1;
  else
    {
      register_id i = --cg->register_spill % REGISTER_N;

      cg_write_pop_register (cg, i);

      // cg->stack_head -= 8;
    }
}


static void
cg_register_free_all (struct cg *cg)
{
  while (cg->register_spill > 0)
    {
      register_id i = --cg->register_spill % REGISTER_N;

      cg_write_pop_register (cg, i);

      // cg->stack_head -= 8;
    }

  for (register_id i = 0; i < REGISTER_N; ++i)
    cg->register_free[i] = 1;
}


/// GENERATE

static register_t cg_generate (struct cg *, struct tree *);


static void cg_generate_empty (struct cg *, struct tree *);

static void cg_generate_if (struct cg *, struct tree *);

static void cg_generate_while (struct cg *, struct tree *);

static void cg_generate_for (struct cg *, struct tree *);

static void cg_generate_compound (struct cg *, struct tree *);

static void cg_generate_variable_declaration (struct cg *, struct tree *);

static void cg_generate_print (struct cg *, struct tree *);


static register_t cg_generate_cast (struct cg *, struct tree *);

static register_t cg_generate_assignment (struct cg *, struct tree *);

static register_t cg_generate_binary (struct cg *, struct tree *);

static register_t cg_generate_reference (struct cg *, struct tree *);

static register_t cg_generate_dereference (struct cg *, struct tree *);

static register_t cg_generate_integer (struct cg *, struct tree *);

static register_t cg_generate_identifier (struct cg *, struct tree *);


static void cg_generate_program (struct cg *, struct tree *);


// LEFT VALUES
static register_t
cg_generate_left_value (struct cg *cg, struct tree *tree)
{
  switch (tree->tree_kind)
    {
    case TREE_DEREFERENCE:
      return cg_generate (cg, tree->child);
    case TREE_IDENTIFIER:
      {
        struct symbol symbol;

        scope_get (cg->scope, tree->token->data.s, &symbol);

        register_t r = cg_register_allocate (cg, WIDTH_64);

        cg_write_load_local_address (cg, r, symbol.offset);

        return r;
      }
    default:
      return register_none;
    }
}


static void
cg_generate_empty (struct cg *cg, struct tree *tree)
{
  (void)cg, (void)tree;

  cg_write_nop (cg);
}


static void
cg_generate_if (struct cg *cg, struct tree *tree)
{
  struct tree *node_cond = tree->child;
  struct tree *node_then = tree->child->next;
  struct tree *node_else = tree->child->next->next;

  label_t label_end;
  label_t label_else;

  label_else = cg_label (cg);

  if (node_else)
    label_end = cg_label (cg);

  register_t c = cg_generate (cg, node_cond);

  cg_write_test (cg, c);
  cg_write_jz (cg, label_else);

  cg_register_free_all (cg);

  cg_generate (cg, node_then);

  cg_register_free_all (cg);

  if (node_else)
    cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_else);

  if (node_else)
    {
      cg_generate (cg, node_else);

      cg_register_free_all (cg);

      cg_write_label (cg, label_end);
    }
}


static void
cg_generate_while (struct cg *cg, struct tree *tree)
{
  struct tree *node_cond = tree->child;
  struct tree *node_then = tree->child->next;

  label_t label_start = cg_label (cg);
  label_t label_end = cg_label (cg);

  cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_start);

  cg_generate (cg, node_then);

  cg_register_free_all (cg);

  cg_write_label (cg, label_end);

  register_t c = cg_generate (cg, node_cond);

  cg_write_test (cg, c);
  cg_write_jnz (cg, label_start);

  cg_register_free_all (cg);
}


static void
cg_generate_for (struct cg *cg, struct tree *tree)
{
  struct tree *node_a    = tree->child;
  struct tree *node_b    = tree->child->next;
  struct tree *node_c    = tree->child->next->next;
  struct tree *node_body = tree->child->next->next->next;

  label_t label_start = cg_label (cg);
  label_t label_end = cg_label (cg);

  cg_generate (cg, node_a);

  cg_register_free_all (cg);

  cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_start);

  cg_generate (cg, node_body);

  cg_register_free_all (cg);

  cg_generate (cg, node_c);

  cg_register_free_all (cg);

  cg_write_label (cg, label_end);

  register_t c = cg_generate (cg, node_b);

  cg_write_test (cg, c);
  cg_write_jnz (cg, label_start);

  cg_register_free_all (cg);
}


static void
cg_generate_compound (struct cg *cg, struct tree *tree)
{
  cg_scope_push (cg);

  for (struct tree *current = tree->child; current; current = current->next)
    {
      cg_generate (cg, current);

      cg_register_free_all (cg);
    }

  cg_scope_pop (cg);
}


static void
cg_generate_variable_declaration (struct cg *cg, struct tree *tree)
{
  struct tree *node = tree->child;
  struct tree *type = tree->type;

  size_t size = type_size (type);

  cg->stack_offset = next_multiple (cg->stack_offset, size);

  size_t offset = cg->stack_offset + size;

  struct symbol symbol;

  symbol.key = node->token->data.s;

  symbol.offset = offset;

  symbol.type = type;

  scope_set (cg->scope, symbol);

  cg->stack_offset = offset;
}


static void
cg_generate_print (struct cg *cg, struct tree *tree)
{
  register_t a = cg_generate (cg, tree->child);

  cg_write_print (cg, a);

  cg_register_free (cg, a);
}


static register_t
cg_generate_cast (struct cg *cg, struct tree *tree)
{
  struct tree *node_a = tree->child;

  register_t r = cg_generate (cg, node_a);

  struct tree *type_a = tree->type;
  struct tree *type_b = node_a->type;

  enum type_width wa = type_width (type_a);
  enum type_width wb = type_width (type_b);

  bool sa = type_is_signed (type_a);
  bool sb = type_is_signed (type_b);

  // Truncation
  if (wa < wb)
    {
      return register_modify (r, wa);
    }

  // Extension
  if (wa > wb)
    {
      register_t s = register_modify (r, wa);

      if (sa && sb)
        cg_write_sext (cg, s, r);
      else
        {
          if (wb < 32)
            cg_write_zext (cg, s, r);
        }

      return s;
    }

  return r;
}


static register_t
cg_generate_assignment (struct cg *cg, struct tree *tree)
{
  struct tree *node_a = tree->child;
  struct tree *node_b = tree->child->next;

  register_t b = cg_generate (cg, node_b);
  register_t a = cg_generate_left_value (cg, node_a);

  cg_write_store (cg, a, b);

  // cg_write (cg, "\tmov\t%s [%s], %s\n", operand_size_string (b.width), register_string (a),
  //          register_string (b));

  cg_register_free (cg, a);

  return b;
}


static register_t
cg_generate_binary (struct cg *cg, struct tree *tree)
{
  struct tree *node_a = tree->child;
  struct tree *node_b = tree->child->next;

  register_t a = cg_generate (cg, node_a);
  register_t b = cg_generate (cg, node_b);

  struct tree *type_a = node_a->type;
  struct tree *type_b = node_b->type;

  bool s = type_is_signed (type_a);

  bool ai = type_is_integer (type_a);
  bool bi = type_is_integer (type_b);

  bool ap = type_is_pointer (type_a);
  bool bp = type_is_pointer (type_b);

  enum token_kind o = tree->token->kind;

  // TODO: Factor out repetition.
  switch (o)
    {
    case TOKEN_DEQUAL:
      cg_write_compare_e (cg, a, b);
      a.width = WIDTH_8;
      break;
    case TOKEN_NEQUAL:
      cg_write_compare_ne (cg, a, b);
      a.width = WIDTH_8;
      break;
    case TOKEN_LT:
      if (s)
        cg_write_scompare_l (cg, a, b);
      else
        cg_write_ucompare_l (cg, a, b);
      a.width = WIDTH_8;
      break;
    case TOKEN_GT:
      if (s)
        cg_write_scompare_g (cg, a, b);
      else
        cg_write_ucompare_g (cg, a, b);
      a.width = WIDTH_8;
      break;
    case TOKEN_LTEQUAL:
      if (s)
        cg_write_scompare_le (cg, a, b);
      else
        cg_write_ucompare_le (cg, a, b);
      a.width = WIDTH_8;
      break;
    case TOKEN_GTEQUAL:
      if (s)
        cg_write_scompare_ge (cg, a, b);
      else
        cg_write_ucompare_ge (cg, a, b);
      a.width = WIDTH_8;
      break;

    case TOKEN_PLUS:
      if (ai && bp)
        {
          size_t w = type_element_size (type_b);

          cg_write (cg, "\timul\t%s, %zu\n", register_string (a), w);
        }

      if (ap && bi)
        {
          size_t w = type_element_size (type_a);

          cg_write (cg, "\timul\t%s, %zu\n", register_string (b), w);
        }

      cg_write_add (cg, a, b);
      break;
    case TOKEN_MINUS:
      if (ai && bp)
        {
          size_t w = type_element_size (type_b);

          cg_write (cg, "\timul\t%s, %zu\n", register_string (a), w);
        }

      if (ap && bi)
        {
          size_t w = type_element_size (type_a);

          cg_write (cg, "\timul\t%s, %zu\n", register_string (b), w);
        }

      cg_write_sub (cg, a, b);
      break;
    case TOKEN_STAR:
      cg_write_mul (cg, a, b);
      break;
    case TOKEN_SLASH:
      if (s)
        cg_write_sdiv (cg, a, b);
      else
        cg_write_udiv (cg, a, b);
      break;
    default:
      abort ();
    }

  cg_register_free (cg, b);

  return a;
}


static register_t
cg_generate_reference (struct cg *cg, struct tree *tree)
{
  return cg_generate_left_value (cg, tree->child);
}


static register_t
cg_generate_dereference (struct cg *cg, struct tree *tree)
{
  register_t b = cg_generate (cg, tree->child);
  register_t a = register_modify (b, type_width (tree->type));

  cg_write_load (cg, a, b);

  return a;
}


static register_t
cg_generate_integer (struct cg *cg, struct tree *tree)
{
  register_t r = cg_register_allocate (cg, WIDTH_64);

  long i = tree->token->data.i;

  cg_write_load_i (cg, r, i);

  return r;
}


static register_t
cg_generate_identifier (struct cg *cg, struct tree *tree)
{
  struct symbol symbol;

  scope_get (cg->scope, tree->token->data.s, &symbol);

  register_t r = cg_register_allocate (cg, type_width (tree->type));

  cg_write_load_local (cg, r, symbol.offset);

  return r;
}


static void
cg_generate_program (struct cg *cg, struct tree *tree)
{
  for (struct tree *current = tree->child; current; current = current->next)
    {
      cg_generate (cg, current);

      cg_register_free_all (cg);
    }
}


static register_t
cg_generate (struct cg *cg, struct tree *tree)
{
  switch (tree->tree_kind)
    {
    case TREE_EMPTY:
      cg_generate_empty (cg, tree);
      return register_none;
    case TREE_IF:
      cg_generate_if (cg, tree);
      return register_none;
    case TREE_WHILE:
      cg_generate_while (cg, tree);
      return register_none;
    case TREE_FOR:
      cg_generate_for (cg, tree);
      return register_none;
    case TREE_COMPOUND:
      cg_generate_compound (cg, tree);
      return register_none;
    case TREE_VARIABLE_DECLARATION:
      cg_generate_variable_declaration (cg, tree);
      return register_none;
    case TREE_PRINT:
      cg_generate_print (cg, tree);
      return register_none;
    case TREE_CAST:
      return cg_generate_cast (cg, tree);
    case TREE_ASSIGNMENT:
      return cg_generate_assignment (cg, tree);
    case TREE_BINARY:
      return cg_generate_binary (cg, tree);
    case TREE_REFERENCE:
      return cg_generate_reference (cg, tree);
    case TREE_DEREFERENCE:
      return cg_generate_dereference (cg, tree);
    case TREE_INTEGER:
      return cg_generate_integer (cg, tree);
    case TREE_IDENTIFIER:
      return cg_generate_identifier (cg, tree);
    case TREE_PROGRAM:
      cg_generate_program (cg, tree);
      return register_none;
    default:
      abort ();
    }
}


void
cg_emit (struct cg *cg, struct tree *tree)
{
  cg_compute (cg, tree);

  cg->stack_usage = next_multiple (cg->stack_offset, 16);

  cg->stack_offset = 0;

  cg_write_begin (cg);

  cg_generate (cg, tree);

  cg_write_end (cg);
}

