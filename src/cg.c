#include "alignment.h"
#include "tree.h"
#include "type.h"
#include "register.h"
#include "cg.h"
#include "memory.h"
#include "scope.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>


static const char *
cg_operand_size_string (enum type_width w)
{
  switch (w)
    {
    case WIDTH_1:
      return "byte";
    case WIDTH_2:
      return "word";
    case WIDTH_4:
      return "dword";
    case WIDTH_8:
      return "qword";
    default:
      return unreachable1 (NULL);
    }
}


typedef size_t cg_label_t;


struct cg_function
{
  struct tree_node_fdefinition *node;

  size_t stack_pointer;

  cg_label_t label_return;
};


struct cg_string
{
  struct cg_string *next;

  char *value;

  cg_label_t label;
};


static struct cg_string *
cg_string_create (char *value, cg_label_t label)
{
  struct cg_string *string;

  string = aa_malloc (sizeof (struct cg_string));

  string->value = value;
  string->label = label;

  return string;
}


struct cg_loop
{
  struct cg_loop *parent;

  cg_label_t label_break;
  cg_label_t label_continue;
};


static struct cg_loop *
cg_loop_create (struct cg_loop *parent, cg_label_t label_break, cg_label_t label_continue)
{
  struct cg_loop *loop;

  loop = aa_malloc (sizeof (struct cg_string));

  loop->parent = parent;

  loop->label_break = label_break;
  loop->label_continue = label_continue;

  return loop;
}


struct cg
{
  FILE *f;

  struct cg_function function;

  struct scope *scope;

  cg_label_t label;
  cg_label_t label_string;

  bool register_free[REGISTER_COUNT];

  size_t register_spill;

  struct cg_string *string_head;
  struct cg_string *string_tail;

  struct cg_loop *loop;
};


static void
cg_scope_push (struct cg *cg)
{
  cg->scope = scope_create (cg->scope);
}


static void
cg_scope_pop (struct cg *cg)
{
  cg->scope = cg->scope->parent;
}


static void cg_register_free_all (struct cg *);


static void
cg_string_push (struct cg *cg, struct cg_string *string)
{
  if (cg->string_tail == NULL)
    {
      cg->string_head = string;
      cg->string_tail = string;
    }
  else
    {
      cg->string_tail->next = string;
      cg->string_tail = string;
    }
}


static void
cg_loop_push (struct cg *cg, struct cg_loop *loop)
{
  cg->loop = loop;
}


static void
cg_loop_pop (struct cg *cg)
{
  cg->loop = cg->loop->parent;
}


struct cg *
cg_create (FILE *f)
{
  struct cg *cg;

  cg = aa_malloc (sizeof (struct cg));

  cg->f = f;

  cg->scope = NULL;

  cg->label = 0;

  cg->register_spill = 0;

  cg_register_free_all (cg);

  cg_scope_push (cg);

  return cg;
}


static cg_label_t
cg_label (struct cg *cg)
{
  return cg->label++;
}


static cg_label_t
cg_label_string (struct cg *cg)
{
  return cg->label_string++;
}


static void
cg_write (struct cg *cg, const char *format, ...)
{
  va_list va;

  va_start (va, format);

  vfprintf (cg->f, format, va);

  va_end (va);
}


static void
cg_write_print (struct cg *cg, struct cg_register r)
{
  cg_write (cg, "\tmov\trdi, %s\n", register_string (r));
  cg_write (cg, "\tcall\tprinti\n");
}


static void
cg_write_load (struct cg *cg, struct cg_register a, struct cg_register b)
{
  const char *s = cg_operand_size_string (a.w);

  cg_write (cg, "\tmov\t%s, %s [%s]\n", register_string (a), s, register_string (b));
}


static void
cg_write_store (struct cg *cg, struct cg_register a, struct cg_register b)
{
  const char *s = cg_operand_size_string (b.w);

  cg_write (cg, "\tmov\t%s [%s], %s\n", s, register_string (a), register_string (b));
}


static void
cg_write_load_i (struct cg *cg, struct cg_register r, long i)
{
  cg_write (cg, "\tmov\t%s, %ld\n", register_string (r), i);
}


static void
cg_write_load_s (struct cg *cg, struct cg_register r, cg_label_t label)
{
  cg_write (cg, "\tmov\t%s, LS%zu\n", register_string (r), label);
}


static void
cg_write_load_local (struct cg *cg, struct cg_register r, size_t offset)
{
  const char *s = cg_operand_size_string (r.w);

  cg_write (cg, "\tmov\t%s, %s [rbp-%zu]\n", register_string (r), s, offset);
}


static void
cg_write_load_local_address (struct cg *cg, struct cg_register r, size_t offset)
{
  cg_write (cg, "\tlea\t%s, [rbp-%zu]\n", register_string (r), offset);
}


static void
cg_write_load_global (struct cg *cg, struct cg_register r, const char *id)
{
  const char *s = cg_operand_size_string (r.w);

  cg_write (cg, "\tmov\t%s, %s [rel %s]\n", register_string (r), s, id);
}


static void
cg_write_load_global_address (struct cg *cg, struct cg_register r, const char *id)
{
  cg_write (cg, "\tlea\t%s, [rel %s]\n", register_string (r), id);
}


static void
cg_write_zext (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tmovzx\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sext (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tmovsx\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_add (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tadd\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sub (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tsub\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_mul (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\timul\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_sdiv (struct cg *cg, struct cg_register a, struct cg_register b)
{
  // TODO: generalize this
  const char *ax;

  if (a.w == WIDTH_4)
    ax = "eax";
  else
    ax = "rax";

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));
  cg_write (cg, "\tcqo\n");
  cg_write (cg, "\tidiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), ax);
}


static void
cg_write_udiv (struct cg *cg, struct cg_register a, struct cg_register b)
{
  const char *ax;

  if (a.w == WIDTH_4)
    ax = "eax";
  else
    ax = "rax";

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));
  cg_write (cg, "\txor\trdx, rdx\n");
  cg_write (cg, "\tdiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), ax);
}


static void
cg_write_smod (struct cg *cg, struct cg_register a, struct cg_register b)
{
  // TODO: generalize this
  const char *ax;
  const char *dx;

  if (a.w == WIDTH_4)
    {
      ax = "eax";
      dx = "edx";
    }
  else
    {
      ax = "rax";
      dx = "rdx";
    }

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));
  cg_write (cg, "\tcqo\n");
  cg_write (cg, "\tidiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), dx);
}


static void
cg_write_umod (struct cg *cg, struct cg_register a, struct cg_register b)
{
  const char *ax;
  const char *dx;

  if (a.w == WIDTH_4)
    {
      ax = "eax";
      dx = "edx";
    }
  else
    {
      ax = "rax";
      dx = "rdx";
    }

  cg_write (cg, "\tmov\t%s, %s\n", ax, register_string (a));
  cg_write (cg, "\txor\t%s, %s\n", dx, dx);
  cg_write (cg, "\tdiv\t%s\n", register_string (b));
  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), dx);
}


static void
cg_write_band (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tand\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_bor (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tor\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_bxor (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\txor\t%s, %s\n", register_string (a), register_string (b));
}


static void
cg_write_shl (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tmov\tcl, %s\n", register_b_string (b));
  cg_write (cg, "\tshl\t%s, cl\n", register_string (a));
}


static void
cg_write_shr (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tmov\tcl, %s\n", register_b_string (b));
  cg_write (cg, "\tshr\t%s, cl\n", register_string (a));
}


static void
cg_write_sar (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tmov\tcl, %s\n", register_b_string (b));
  cg_write (cg, "\tsar\t%s, cl\n", register_string (a));
}


static void
cg_write_compare (struct cg *cg, const char *i, struct cg_register a, struct cg_register b)
{
  cg_write (cg, "\tcmp\t%s, %s\n", register_string (a), register_string (b));
  cg_write (cg, "\t%s\t%s\n", i, register_b_string (a));
}


static void
cg_write_compare_e (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "sete", a, b);
}


static void
cg_write_compare_ne (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setne", a, b);
}


static void
cg_write_scompare_l (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setl", a, b);
}


static void
cg_write_scompare_g (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setg", a, b);
}


static void
cg_write_scompare_le (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setle", a, b);
}


static void
cg_write_scompare_ge (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setge", a, b);
}


static void
cg_write_ucompare_l (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setb", a, b);
}


static void
cg_write_ucompare_g (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "seta", a, b);
}


static void
cg_write_ucompare_le (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setbe", a, b);
}


static void
cg_write_ucompare_ge (struct cg *cg, struct cg_register a, struct cg_register b)
{
  cg_write_compare (cg, "setae", a, b);
}


static void
cg_write_neg (struct cg *cg, struct cg_register a)
{
  cg_write (cg, "\tneg\t%s\n", register_string (a));
}


static void
cg_write_lnot (struct cg *cg, struct cg_register a)
{
  cg_write (cg, "\ttest\t%s, %s\n", register_string (a), register_string (a));
  cg_write (cg, "\tsete\t%s\n", register_b_string (a));
}


static void
cg_write_bnot (struct cg *cg, struct cg_register a)
{
  cg_write (cg, "\tnot\t%s\n", register_string (a));
}


static void
cg_write_label (struct cg *cg, cg_label_t label)
{
  cg_write (cg, ".L%zu:\n", label);
}


static void
cg_write_jump (struct cg *cg, cg_label_t label, const char *s)
{
  cg_write (cg, "\t%s\t.L%zu\n", s, label);
}


static void
cg_write_jmp (struct cg *cg, cg_label_t label)
{
  cg_write_jump (cg, label, "jmp");
}


static void
cg_write_jz (struct cg *cg, cg_label_t a)
{
  cg_write_jump (cg, a, "jz");
}


static void
cg_write_jnz (struct cg *cg, cg_label_t a)
{
  cg_write_jump (cg, a, "jnz");
}


static void
cg_write_test (struct cg *cg, struct cg_register a)
{
  cg_write (cg, "\ttest\t%s, %s\n", register_string (a), register_string (a));
}


static void
cg_write_begin (struct cg *cg)
{
  cg_write (cg, "section .text\n");
  cg_write (cg, "\textern\tprintf\n");

  cg_write (cg, "\n");

  cg_write (cg, "printi:\n");
  cg_write (cg, "\tmov\trsi, rdi\n");
  cg_write (cg, "\txor\teax, eax\n");
  cg_write (cg, "\tmov\trdi, printi_s\n");
  cg_write (cg, "\tjmp\tprintf\n");

  cg_write (cg, "\n");
}


static void
cg_write_end (struct cg *cg)
{
  cg_write (cg, "section .rodata\n");
  cg_write (cg, "\tprinti_s: db \"%%ld\", 10, 0\n");

  for (struct cg_string *s = cg->string_head; s; s = s->next)
    {
      cg_write (cg, "\tLS%zu: db ", s->label);

      for (char *c = s->value; *c; c++)
        cg_write (cg, "%d, ", *c);

      cg_write (cg, "0\n");
    }

  cg_write (cg, "\n");

  cg_write (cg, "section .bss\n");

  for (struct symbol *s = cg->scope->head; s; s = s->next)
  {
    // TODO: Better logic for this

    if (s->type->kind == TYPE_FUNCTION || s->external)
      continue;

    cg_write (cg, "\t%s: resb %ld\n", s->name, type_size (s->type));
  }

  cg_write (cg, "\n");

  cg_write (cg, "section .note.GNU-stack\n");

  cg_write (cg, "\n");
}


static void
cg_write_push_register_id (struct cg *cg, enum cg_register_id r)
{
  cg_write (cg, "\tpush\t%s\n", register_id_string (r));
}


static void
cg_write_pop_register_id (struct cg *cg, enum cg_register_id r)
{
  cg_write (cg, "\tpop\t%s\n", register_id_string (r));
}


// TODO: Better register allocation (and spilling) strategy.

// Register allocation boundaries
enum
{
  CG_RA_S = REGISTER_R10,
  CG_RA_E = REGISTER_R12,
  CG_RA_COUNT = CG_RA_E - CG_RA_S + 1,
};


static struct cg_register
cg_register_allocate (struct cg *cg, enum type_width width)
{
  for (size_t i = CG_RA_S; i <= CG_RA_E; ++i)
    {
      if (cg->register_free[i] == 1)
        {
          cg->register_free[i] = 0;

          return register_create (i, width);
        }
    }

  enum cg_register_id i = CG_RA_S + cg->register_spill++ % (CG_RA_COUNT - 1);

  cg_write_push_register_id (cg, i);

  return register_create (i, width);
}


static void
cg_register_free (struct cg *cg, struct cg_register r)
{
  if (cg->register_spill == 0)
    cg->register_free[r.id] = 1;
  else
    {
      enum cg_register_id i = CG_RA_S + --cg->register_spill % (CG_RA_COUNT - 1);

      cg_write_pop_register_id (cg, i);
    }
}


static void
cg_register_free_all (struct cg *cg)
{
  while (cg->register_spill > 0)
    {
      enum cg_register_id i = CG_RA_S + --cg->register_spill % (CG_RA_COUNT - 1);

      cg_write_pop_register_id (cg, i);
    }

  for (enum cg_register_id i = 0; i < REGISTER_COUNT; ++i)
    cg->register_free[i] = 1;
}


static void
cg_count_stack_usage (struct cg *cg, struct tree *tree)
{
  if (!tree)
    return;

  switch (tree->kind)
    {
    case TREE_FDEFINITION:
      {
        struct tree_node_fdefinition node = tree->d.fdefinition;

        for (struct tree *t = node.parameter1; t; t = t->next)
          cg_count_stack_usage (cg, t);

        cg_count_stack_usage (cg, node.body);
      }
      break;
    case TREE_IF:
      {
        struct tree_node_if node = tree->d.if_;

        cg_count_stack_usage (cg, node.branch_a);
        cg_count_stack_usage (cg, node.branch_b);
      }
      break;
    case TREE_WHILE:
      {
        struct tree_node_while node = tree->d.while_;

        cg_count_stack_usage (cg, node.body);
      }
      break;
    case TREE_FOR:
      {
        struct tree_node_for node = tree->d.for_;

        cg_count_stack_usage (cg, node.body);
      }
      break;
    case TREE_COMPOUND:
      {
        struct tree_node_compound node = tree->d.compound;

        for (struct tree *t = node.statement1; t; t = t->next)
          cg_count_stack_usage (cg, t);
      }
      break;
    case TREE_VDECLARATION:
      {
        struct tree_node_vdeclaration node = tree->d.vdeclaration;

        size_t s = type_size (node.variable_type);
        size_t a = type_alignment (node.variable_type);

        cg->function.stack_pointer = next_multiple (cg->function.stack_pointer, a) + s;
      }
      break;
    case TREE_PROGRAM:
      {
        struct tree_node_program node = tree->d.program;

        for (struct tree *t = node.top_level1; t; t = t->next)
          cg_count_stack_usage (cg, t);
      }
      break;
    default:
      break;
    }


}


static void cg_generate_node_extern (struct cg *, struct tree *);

static void cg_generate_node_fdeclaration (struct cg *, struct tree *);

static void cg_generate_node_fdefinition (struct cg *, struct tree *);


static void cg_generate_node_if (struct cg *, struct tree *);

static void cg_generate_node_while (struct cg *, struct tree *);

static void cg_generate_node_for (struct cg *, struct tree *);

static void cg_generate_node_compound (struct cg *, struct tree *);

static void cg_generate_node_vdeclaration (struct cg *, struct tree *);

static void cg_generate_node_return (struct cg *, struct tree *);

static void cg_generate_node_break (struct cg *, struct tree *);

static void cg_generate_node_continue (struct cg *, struct tree *);

static void cg_generate_node_print (struct cg *, struct tree *);


static struct cg_register cg_generate_node_scale (struct cg *, struct tree *);

static struct cg_register cg_generate_node_cast (struct cg *, struct tree *);

static struct cg_register cg_generate_node_call (struct cg *, struct tree *);

static struct cg_register cg_generate_node_assignment (struct cg *, struct tree *);

static struct cg_register cg_generate_node_access (struct cg *, struct tree *);

static struct cg_register cg_generate_node_or (struct cg *, struct tree *);

static struct cg_register cg_generate_node_and (struct cg *, struct tree *);

static struct cg_register cg_generate_node_unary (struct cg *, struct tree *);

static struct cg_register cg_generate_node_binary (struct cg *, struct tree *);

static struct cg_register cg_generate_node_reference (struct cg *, struct tree *);

static struct cg_register cg_generate_node_dereference (struct cg *, struct tree *);

static struct cg_register cg_generate_node_integer (struct cg *, struct tree *);

static struct cg_register cg_generate_node_string (struct cg *, struct tree *);

static struct cg_register cg_generate_node_identifier (struct cg *, struct tree *);


static void cg_generate_node_program (struct cg *, struct tree *);


static struct cg_register
cg_generate_expression (struct cg *cg, struct tree *tree)
{
  if (!tree)
    return register_none;

  switch (tree->kind)
    {
    case TREE_IMPLICIT_SCALE:
      return cg_generate_node_scale (cg, tree);
    case TREE_CAST:
      return cg_generate_node_cast (cg, tree);
    case TREE_CALL:
      return cg_generate_node_call (cg, tree);
    case TREE_ASSIGNMENT:
      return cg_generate_node_assignment (cg, tree);
    case TREE_ACCESS:
      return cg_generate_node_access (cg, tree);
    case TREE_OR:
      return cg_generate_node_or (cg, tree);
    case TREE_AND:
      return cg_generate_node_and (cg, tree);
    case TREE_UNARY:
      return cg_generate_node_unary (cg, tree);
    case TREE_BINARY:
      return cg_generate_node_binary (cg, tree);
    case TREE_REFERENCE:
      return cg_generate_node_reference (cg, tree);
    case TREE_DEREFERENCE:
      return cg_generate_node_dereference (cg, tree);
    case TREE_INTEGER:
      return cg_generate_node_integer (cg, tree);
    case TREE_STRING:
      return cg_generate_node_string (cg, tree);
    case TREE_IDENTIFIER:
      return cg_generate_node_identifier (cg, tree);
    default:
      return unreachable1 (register_none);
    }
}


static struct cg_register
cg_generate_lvalue (struct cg *cg, struct tree *tree)
{
  if (!tree)
    return register_none;

  switch (tree->kind)
    {
    case TREE_DEREFERENCE:
      return cg_generate_expression (cg, tree->d.dereference.value);

    case TREE_IDENTIFIER:
      {
        struct tree_node_identifier *node = &tree->d.identifier;

        struct symbol *symbol;

        scope_get_assert (cg->scope, node->value, &symbol);

        struct cg_register r = cg_register_allocate (cg, WIDTH_8);

        switch (symbol->kind)
          {
          case SYMBOL_LOCAL:
            cg_write_load_local_address (cg, r, symbol->d.local.offset);
            break;

          case SYMBOL_GLOBAL:
            cg_write_load_global_address (cg, r, symbol->name);
            break;

          default:
            break;
          }

        return r;
      }

    case TREE_ACCESS:
      {
        struct tree_node_access *node = &tree->d.access;

        struct cg_register s = cg_generate_lvalue (cg, node->value);

        struct symbol *symbol;

        struct type_node_struct type_node = tree_get_expression_type (node->value)->d.struct_;

        scope_get_assert (type_node.scope, node->field, &symbol);

        cg_write (cg, "\tadd\t%s, %zu\n", register_string (s), symbol->d.field.offset);

        return s;

      }

    default:
      return unreachable1 (register_none);
    }
}


static struct cg_register
cg_generate_rvalue (struct cg *cg, struct tree *tree)
{
  return cg_generate_expression (cg, tree);
}


static void
cg_generate_statement (struct cg *cg, struct tree *tree)
{
  if (!tree)
    return;

  cg_register_free_all (cg);

  switch (tree->kind)
    {
    case TREE_EXTERN:
      cg_generate_node_extern (cg, tree);
      break;
    case TREE_FDECLARATION:
      cg_generate_node_fdeclaration (cg, tree);
      break;
    case TREE_FDEFINITION:
      cg_generate_node_fdefinition (cg, tree);
      break;
    case TREE_STRUCT:
      break;
    case TREE_IF:
      cg_generate_node_if (cg, tree);
      break;
    case TREE_WHILE:
      cg_generate_node_while (cg, tree);
      break;
    case TREE_FOR:
      cg_generate_node_for (cg, tree);
      break;
    case TREE_COMPOUND:
      cg_generate_node_compound (cg, tree);
      break;
    case TREE_VDECLARATION:
      cg_generate_node_vdeclaration (cg, tree);
      break;
    case TREE_RETURN:
      cg_generate_node_return (cg, tree);
      break;
    case TREE_BREAK:
      cg_generate_node_break (cg, tree);
      break;
    case TREE_CONTINUE:
      cg_generate_node_continue (cg, tree);
      break;
    case TREE_PRINT:
      cg_generate_node_print (cg, tree);
      break;
    case TREE_PROGRAM:
      cg_generate_node_program (cg, tree);
      break;
    default:
      cg_generate_expression (cg, tree);
      break;
    }

  cg_register_free_all (cg);
}


static void
cg_generate_node_extern (struct cg *cg, struct tree *tree)
{
  struct tree_node_extern *node = &tree->d.extern_;

  struct symbol *symbol = symbol_create (SYMBOL_GLOBAL, node->name, node->type);

  symbol->external = true;

  scope_set_assert (cg->scope, symbol);

  cg_write (cg, "\textern\t%s\n", node->name);
}


static void
cg_generate_node_fdeclaration (struct cg *cg, struct tree *tree)
{
  struct tree_node_fdeclaration *node = &tree->d.fdeclaration;

  scope_set_assert (cg->scope, symbol_create (SYMBOL_GLOBAL, node->name, node->function_type));

  cg_write (cg, "\textern\t%s\n", node->name);
}


static void
cg_generate_node_fdefinition (struct cg *cg, struct tree *tree)
{
  struct tree_node_fdefinition *node = &tree->d.fdefinition;

  cg->function.node = node;

  cg->function.stack_pointer = 0;

  cg->function.label_return = cg_label (cg);

  scope_set_assert (cg->scope, symbol_create (SYMBOL_GLOBAL, node->name, node->function_type));

  cg_scope_push (cg);

  cg_count_stack_usage (cg, tree);

  size_t stack_usage = next_multiple (cg->function.stack_pointer, 16);

  cg->function.stack_pointer = 0;

  cg_write (cg, "\tglobal\t%s\n", cg->function.node->name);
  cg_write (cg, "%s:\n", cg->function.node->name);
  cg_write (cg, "\tpush\trbp\n");
  cg_write (cg, "\tmov\trbp, rsp\n");
  cg_write (cg, "\tsub\trsp, %zu\n", stack_usage);

  const enum cg_register_id p_register[] = {
    REGISTER_RDI,
    REGISTER_RSI,
    REGISTER_RDX,
    REGISTER_RCX,
    REGISTER_R8,
    REGISTER_R9,
  };

  size_t p_n = 0;

  for (struct tree *t = node->parameter1; t; t = t->next)
    {
      cg_generate_statement (cg, t);

      // NOTE: Last symbol pushed is the current parameters's symbol.
      struct symbol *symbol = cg->scope->head;

      enum type_width w = type_width (symbol->type);

      const char *s = cg_operand_size_string (w);

      // First N arguments are passed in registers
      if (p_n < 6)
        {
          struct cg_register r = register_create (p_register[p_n], w);

          size_t offset_s = symbol->d.local.offset;

          cg_write (cg, "\tmov\t%s [rbp-%zu], %s\n", s, offset_s, register_string (r));
        }

      // Rest of the arguments are passed on the stack
      else
        {
          struct cg_register r = cg_register_allocate (cg, w);

          size_t offset_p = (p_n - 6) * 8 + 16;
          size_t offset_s = symbol->d.local.offset;

          cg_write (cg, "\tmov\t%s, %s [rbp+%zu]\n", register_string (r), s, offset_p);
          cg_write (cg, "\tmov\t%s [rbp-%zu], %s\n", s, offset_s, register_string (r));

          cg_register_free (cg, r);
        }

      p_n++;
    }

  for (struct tree *t = node->body->d.compound.statement1; t; t = t->next)
    cg_generate_statement (cg, t);

  if (node->function_type->d.function.to->kind == TYPE_VOID)
    {
      cg_write_label (cg, cg->function.label_return);
      cg_write (cg, "\txor\trax, rax\n", stack_usage);
    }
  else
    {
      cg_write (cg, "\txor\trax, rax\n", stack_usage);
      cg_write_label (cg, cg->function.label_return);
    }

  cg_write (cg, "\tadd\trsp, %zu\n", stack_usage);
  cg_write (cg, "\tpop\trbp\n");
  cg_write (cg, "\tret\n");

  cg_write (cg, "\n");

  cg_scope_pop (cg);
}


static void
cg_generate_node_if (struct cg *cg, struct tree *tree)
{
  struct tree_node_if *node = &tree->d.if_;

  bool has_else = node->branch_b != NULL;

  cg_label_t label_end;
  cg_label_t label_else;

  label_else = cg_label (cg);

  if (has_else)
    label_end = cg_label (cg);

  struct cg_register c = cg_generate_rvalue (cg, node->condition);

  cg_write_test (cg, c);
  cg_write_jz (cg, label_else);

  cg_generate_statement (cg, node->branch_a);

  if (has_else)
    cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_else);

  if (has_else)
    {
      cg_generate_statement (cg, node->branch_b);

      cg_write_label (cg, label_end);
    }
}


static void
cg_generate_node_while (struct cg *cg, struct tree *tree)
{
  struct tree_node_while *node = &tree->d.while_;

  cg_label_t label_start = cg_label (cg);
  cg_label_t label_end = cg_label (cg);

  cg_label_t label_break = cg_label (cg);
  cg_label_t label_continue = label_end;

  cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_start);

  cg_loop_push (cg, cg_loop_create (cg->loop, label_break, label_continue));

  cg_generate_statement (cg, node->body);

  cg_loop_pop (cg);

  cg_write_label (cg, label_end);

  struct cg_register c = cg_generate_rvalue (cg, node->condition);

  cg_write_test (cg, c);
  cg_write_jnz (cg, label_start);

  cg_write_label (cg, label_break);
}


static void
cg_generate_node_for (struct cg *cg, struct tree *tree)
{
  struct tree_node_for *node = &tree->d.for_;

  cg_label_t label_start = cg_label (cg);
  cg_label_t label_end = cg_label (cg);

  cg_label_t label_break = cg_label (cg);
  cg_label_t label_continue = cg_label (cg);

  cg_generate_statement (cg, node->init);

  cg_write_jmp (cg, label_end);

  cg_write_label (cg, label_start);

  cg_loop_push (cg, cg_loop_create (cg->loop, label_break, label_continue));

  cg_generate_statement (cg, node->body);

  cg_loop_pop (cg);

  cg_write_label (cg, label_continue);

  cg_generate_statement (cg, node->increment);

  cg_write_label (cg, label_end);

  struct cg_register c = cg_generate_rvalue (cg, node->condition);

  cg_write_test (cg, c);
  cg_write_jnz (cg, label_start);

  cg_write_label (cg, label_break);
}


static void
cg_generate_node_compound (struct cg *cg, struct tree *tree)
{
  struct tree_node_compound *node = &tree->d.compound;

  cg_scope_push (cg);

  for (struct tree *t = node->statement1; t; t = t->next)
    cg_generate_statement (cg, t);

  cg_scope_pop (cg);
}


static void
cg_generate_node_vdeclaration (struct cg *cg, struct tree *tree)
{
  struct tree_node_vdeclaration *node = &tree->d.vdeclaration;

  size_t s = type_size (node->variable_type);
  size_t a = type_alignment (node->variable_type);

  cg->function.stack_pointer = next_multiple (cg->function.stack_pointer, a);

  size_t offset = cg->function.stack_pointer + s;

  struct symbol *symbol;

  symbol = symbol_create (node->symbol_kind, node->name, node->variable_type);

  symbol->d.local.offset = offset;

  scope_set_assert (cg->scope, symbol);

  cg->function.stack_pointer = offset;
}


static void
cg_generate_node_return (struct cg *cg, struct tree *tree)
{
  struct tree_node_return *node = &tree->d.return_;

  if (node->value)
    {
      struct cg_register a = cg_generate_rvalue (cg, node->value);
      struct cg_register b = register_create (REGISTER_RAX, a.w);

      cg_write (cg, "\tmov\t%s, %s\n", register_string (b), register_string (a));
    }

  cg_write_jmp (cg, cg->function.label_return);
}


static void
cg_generate_node_break (struct cg *cg, struct tree *tree)
{
  (void)tree;

  cg_write_jmp (cg, cg->loop->label_break);
}


static void
cg_generate_node_continue (struct cg *cg, struct tree *tree)
{
  (void)tree;

  cg_write_jmp (cg, cg->loop->label_continue);
}


static void
cg_generate_node_print (struct cg *cg, struct tree *tree)
{
  struct tree_node_print *node = &tree->d.print;

  cg_write_print (cg, cg_generate_rvalue (cg, node->value));
}


static struct cg_register
cg_generate_node_scale (struct cg *cg, struct tree *tree)
{
  struct tree_node_scale *node = &tree->d.scale;

  struct cg_register r = cg_generate_rvalue (cg, node->value);

  size_t w = type_size (node->base_type);

  if (w != 1)
    cg_write (cg, "\timul\t%s, %zu\n", register_string (r), w);

  return r;
}


static struct cg_register
cg_generate_node_cast (struct cg *cg, struct tree *tree)
{
  struct tree_node_cast *node = &tree->d.cast;

  struct cg_register r = cg_generate_rvalue (cg, node->value);

  struct type *type_a = node->expression_type;
  struct type *type_b = tree_get_expression_type (node->value);

  enum type_width wa = type_width (type_a);
  enum type_width wb = type_width (type_b);

  bool sa = type_is_integer_signed (type_a);
  bool sb = type_is_integer_signed (type_b);

  // Truncation
  if (wa < wb)
    {
      return register_modify (r, wa);
    }

  // Extension
  if (wa > wb)
    {
      struct cg_register s = register_modify (r, wa);

      if (sa && sb)
        cg_write_sext (cg, s, r);
      else
        {
          if (wb < WIDTH_4)
            cg_write_zext (cg, s, r);
        }

      return s;
    }

  return r;
}


// TODO: Rewrite function calling code. It's wrong.


static struct cg_register
cg_generate_node_call (struct cg *cg, struct tree *tree)
{
  struct tree_node_call *node = &tree->d.call;

  size_t register_save = 0;

  for (size_t i = CG_RA_S; i <= CG_RA_E; ++i)
    if (cg->register_free[i] == 0)
      {
        cg_write_push_register_id (cg, i);

        register_save++;
      }

  const enum cg_register_id p_register[] = {
    REGISTER_RDI,
    REGISTER_RSI,
    REGISTER_RDX,
    REGISTER_RCX,
    REGISTER_R8,
    REGISTER_R9,
  };

  size_t p_n = 0;

  for (struct tree *t = node->argument1; t; t = t->next)
    p_n++;

  size_t i = 0;

  struct tree *argument = node->argument1;

  while (argument)
    {
      if (i >= 6)
        break;

      struct cg_register a = cg_generate_rvalue (cg, argument);

      struct cg_register b = register_create (p_register[i], a.w);

      cg_write (cg, "\tmov\t%s, %s\n", register_string (b), register_string (a));

      cg_register_free (cg, a);

      argument = argument->next;
      i++;
    }

  // System V bullshit

  size_t P = register_save + cg->register_spill;

  size_t stack_args = p_n - i;

  size_t pad = (P + stack_args) & 1 ? 8 : 0;

  size_t offset = stack_args * 8 + pad;

  if (offset != 0)
    cg_write (cg, "\tsub\trsp, %zu\n", offset);

  while (argument)
    {
      if (i >= p_n)
        break;

      struct cg_register a = cg_generate_rvalue (cg, argument);

      size_t offset_p = (i - 6) * 8;

      cg_write (cg, "\tmov\tqword [rsp+%zu], %s\n", offset_p, register_string (a));

      cg_register_free (cg, a);

      argument = argument->next;
      i++;
    }

  struct cg_register a = cg_generate_rvalue (cg, node->callee);

  cg_write (cg, "\tcall\t%s\n", register_string (a));

  cg_register_free (cg, a);

  if (offset != 0)
    cg_write (cg, "\tadd\trsp, %zu\n", offset);

  for (size_t i = CG_RA_E + 1; i-- > CG_RA_S; )
    if (cg->register_free[i] == 0)
      {
        cg_write_pop_register_id (cg, i);
      }

  if (node->expression_type->kind != TYPE_VOID)
    {
      size_t w = type_width (node->expression_type);

      struct cg_register r = register_create (REGISTER_RAX, w);

      struct cg_register s = cg_register_allocate (cg, w);

      cg_write (cg, "\tmov\t%s, %s\n", register_string (s), register_string (r));

      return s;
    }

  return register_none;
}


static struct cg_register
cg_generate_node_assignment (struct cg *cg, struct tree *tree)
{
  struct tree_node_assignment *node = &tree->d.assignment;

  struct cg_register rhs = cg_generate_rvalue (cg, node->rhs);
  struct cg_register lhs = cg_generate_lvalue (cg, node->lhs);

  cg_write_store (cg, lhs, rhs);

  cg_register_free (cg, lhs);

  return rhs;
}


static struct cg_register
cg_generate_node_access (struct cg *cg, struct tree *tree)
{
  struct tree_node_access *node = &tree->d.access;

  struct cg_register s = cg_generate_lvalue (cg, node->value);

  struct symbol *symbol;

  struct type_node_struct type_node = tree_get_expression_type (node->value)->d.struct_;

  scope_get_assert (type_node.scope, node->field, &symbol);

  cg_write (cg, "\tadd\t%s, %zu\n", register_string (s), symbol->d.field.offset);

  struct cg_register r = register_modify (s, type_width (node->expression_type));

  cg_write_load (cg, r, s);

  return r;
}


static struct cg_register
cg_generate_node_or (struct cg *cg, struct tree *tree)
{
  struct tree_node_or *node = &tree->d.or;

  cg_label_t label_end = cg_label (cg);

  struct cg_register a = cg_generate_rvalue (cg, node->lhs);

  cg_write_test (cg, a);
  cg_write_jnz (cg, label_end);

  cg_register_free (cg, a);

  a = cg_generate_rvalue (cg, node->rhs);

  cg_write_label (cg, label_end);

  return a;
}


static struct cg_register
cg_generate_node_and (struct cg *cg, struct tree *tree)
{
  struct tree_node_and *node = &tree->d.and;

  cg_label_t label_end = cg_label (cg);

  struct cg_register a = cg_generate_rvalue (cg, node->lhs);

  cg_write_test (cg, a);
  cg_write_jz (cg, label_end);

  cg_register_free (cg, a);

  a = cg_generate_rvalue (cg, node->rhs);

  cg_write_label (cg, label_end);

  return a;
}


static struct cg_register
cg_generate_node_unary (struct cg *cg, struct tree *tree)
{
  struct tree_node_unary *node = &tree->d.unary;

  struct cg_register a = cg_generate_rvalue (cg, node->value);

  enum unary_operator o = node->operator;

  switch (o)
    {
    case UNARY_NEG:
      cg_write_neg (cg, a);
      break;

    case UNARY_BNOT:
      cg_write_bnot (cg, a);
      break;

    case UNARY_LNOT:
      cg_write_lnot (cg, a);
      a.w = WIDTH_1;
      break;

    default:
      unreachable ();
      break;
    }

  return a;

}


static struct cg_register
cg_generate_node_binary (struct cg *cg, struct tree *tree)
{
  struct tree_node_binary *node = &tree->d.binary;

  struct cg_register a = cg_generate_rvalue (cg, node->lhs);
  struct cg_register b = cg_generate_rvalue (cg, node->rhs);

  struct type *type_a = tree_get_expression_type (node->lhs);
  // struct type *type_b = tree_type (node->rhs);

  bool s = type_is_integer_signed (type_a);

  // bool ai = type_is_integer (type_a);
  // bool bi = type_is_integer (type_b);

  // bool ap = type_is_pointer (type_a);
  // bool bp = type_is_pointer (type_b);

  enum binary_operator o = node->operator;

  switch (o)
    {
    case BINARY_ADD:
      cg_write_add (cg, a, b);
      break;
    case BINARY_SUB:
      cg_write_sub (cg, a, b);
      break;

    case BINARY_SHL:
      cg_write_shl (cg, a, b);
      break;
    case BINARY_SHR:
      if (s)
        cg_write_sar (cg, a, b);
      else
        cg_write_shr (cg, a, b);
      break;
    case BINARY_BOR:
      cg_write_bor (cg, a, b);
      break;
    case BINARY_BAND:
      cg_write_band (cg, a, b);
      break;
    case BINARY_BXOR:
      cg_write_bxor (cg, a, b);
      break;

    case BINARY_MUL:
      cg_write_mul (cg, a, b);
      break;
    case BINARY_DIV:
      if (s)
        cg_write_sdiv (cg, a, b);
      else
        cg_write_udiv (cg, a, b);
      break;
    case BINARY_MOD:
      if (s)
        cg_write_smod (cg, a, b);
      else
        cg_write_umod (cg, a, b);
      break;

    case BINARY_CMP_EQ:
      cg_write_compare_e (cg, a, b);
      a.w = WIDTH_1;
      break;
    case BINARY_CMP_NE:
      cg_write_compare_ne (cg, a, b);
      a.w = WIDTH_1;
      break;
    case BINARY_CMP_L:
      if (s)
        cg_write_scompare_l (cg, a, b);
      else
        cg_write_ucompare_l (cg, a, b);
      a.w = WIDTH_1;
      break;
    case BINARY_CMP_G:
      if (s)
        cg_write_scompare_g (cg, a, b);
      else
        cg_write_ucompare_g (cg, a, b);
      a.w = WIDTH_1;
      break;
    case BINARY_CMP_LE:
      if (s)
        cg_write_scompare_le (cg, a, b);
      else
        cg_write_ucompare_le (cg, a, b);
      a.w = WIDTH_1;
      break;
    case BINARY_CMP_GE:
      if (s)
        cg_write_scompare_ge (cg, a, b);
      else
        cg_write_ucompare_ge (cg, a, b);
      a.w = WIDTH_1;
      break;

    default:
      unreachable ();
      break;
    }

  cg_register_free (cg, b);

  return a;
}


static struct cg_register
cg_generate_node_reference (struct cg *cg, struct tree *tree)
{
  struct tree_node_reference *node = &tree->d.reference;

  return cg_generate_lvalue (cg, node->value);
}


static struct cg_register
cg_generate_node_dereference (struct cg *cg, struct tree *tree)
{
  struct tree_node_dereference *node = &tree->d.dereference;

  struct cg_register r = cg_generate_rvalue (cg, node->value);

  struct cg_register s = register_modify (r, type_width (node->expression_type));

  cg_write_load (cg, s, r);

  return s;
}


static struct cg_register
cg_generate_node_integer (struct cg *cg, struct tree *tree)
{
  struct tree_node_integer *node = &tree->d.integer;

  struct cg_register r = cg_register_allocate (cg, WIDTH_8);

  cg_write_load_i (cg, r, node->value);

  return r;
}


static struct cg_register
cg_generate_node_string (struct cg *cg, struct tree *tree)
{
  struct tree_node_string *node = &tree->d.string;

  cg_label_t label = cg_label_string (cg);

  cg_string_push (cg, cg_string_create (node->value, label));

  struct cg_register r = cg_register_allocate (cg, type_width (node->expression_type));

  cg_write_load_s (cg, r, label);

  return r;
}


static struct cg_register
cg_generate_node_identifier (struct cg *cg, struct tree *tree)
{
  struct tree_node_identifier *node = &tree->d.identifier;

  struct symbol *symbol;

  scope_get_assert (cg->scope, node->value, &symbol);

  struct cg_register r = cg_register_allocate (cg, type_width (node->expression_type));

  switch (symbol->kind)
    {
    case SYMBOL_LOCAL:
      cg_write_load_local (cg, r, symbol->d.local.offset);
      break;

    case SYMBOL_GLOBAL:
      cg_write_load_global (cg, r, symbol->name);
      break;

    default:
      break;
    }

  return r;
}


static void
cg_generate_node_program (struct cg *cg, struct tree *tree)
{
  struct tree_node_program *node = &tree->d.program;

  for (struct tree *t = node->top_level1; t; t = t->next)
    cg_generate_statement (cg, t);
}


void
cg_generate (struct cg *cg, struct tree *tree)
{
  cg_write_begin (cg);

  cg_generate_statement (cg, tree);

  cg_write_end (cg);
}

