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


typedef struct
{
  enum register_id id;

  enum type_width width;
} register_t;


static const register_t register_none = {
  .id = -1,
  .width = WIDTH_8,
};


static register_t
register_create (enum register_id id, enum type_width width)
{
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
  "al",
  "bl",
  "cl",
  "dl",
  "sil",
  "dil",
  "spl",
  "bpl",
  "r8b",
  "r9b",
  "r10b",
  "r11b",
  "r12b",
  "r13b",
  "r14b",
  "r15b",

  "ax",
  "bx",
  "cx",
  "dx",
  "si",
  "di",
  "sp",
  "bp",
  "r8w",
  "r9w",
  "r10w",
  "r11w",
  "r12w",
  "r13w",
  "r14w",
  "r15w",

  "eax",
  "ebx",
  "ecx",
  "edx",
  "esi",
  "edi",
  "esp",
  "ebp",
  "r8d",
  "r9d",
  "r10d",
  "r11d",
  "r12d",
  "r13d",
  "r14d",
  "r15d",

  "rax",
  "rbx",
  "rcx",
  "rdx",
  "rsi",
  "rdi",
  "rsp",
  "rbp",
  "r8" ,
  "r9" ,
  "r10",
  "r11",
  "r12",
  "r13",
  "r14",
  "r15",
};


static const char *
register_b_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 0];
}


static const char *
register_w_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 1];
}


static const char *
register_d_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 2];
}


static const char *
register_q_string (register_t r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 3];
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
register_id_string (enum register_id i)
{
  return REGISTER_STRING[i + REGISTER_COUNT * 3];
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

  // cg->stack_offset = 0;
  // cg->stack_head = 0;
  // cg->stack_usage = 0;

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
        size_t alignment = type_alignment (tree->type);

        cg->function.stack_offset = next_multiple (cg->function.stack_offset, alignment);
        cg->function.stack_offset = cg->function.stack_offset + size;
      }
      break;
    default:
      break;
    }

  if (tree->child)
    cg_compute (cg, tree->child);

  if (tree->next)
    cg_compute (cg, tree->next);

  // switch (tree->tree_kind)
  //   {
  //   case TREE_VARIABLE_DECLARATION:
  //     {
  //       size_t size = type_size (tree->type);

  //       cg->stack_offset = next_multiple (cg->stack_offset, size);
  //       cg->stack_offset = cg->stack_offset + size;
  //     }
  //     break;
  //   default:
  //     break;
  //   }

  // if (tree->child)
  //   cg_compute (cg, tree->child);

  // if (tree->next)
  //   cg_compute (cg, tree->next);
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
  // cg_write (cg, "\tglobal\tmain\n");
  cg_write (cg, "\textern\tprintf\n");

  cg_write (cg, "\n");

  cg_write (cg, "printi:\n");
  cg_write (cg, "\tmov\trsi, rdi\n");
  cg_write (cg, "\txor\teax, eax\n");
  cg_write (cg, "\tmov\tedi, printi_s\n");
  cg_write (cg, "\tjmp\tprintf\n");

  cg_write (cg, "\n");

  // cg_write (cg, "main:\n");

  // cg_write (cg, "\t; Enter\n");
  // cg_write (cg, "\tpush\trbp\n");
  // cg_write (cg, "\tmov\trbp, rsp\n");
  // cg_write (cg, "\tsub\trsp, %zu\n", cg->stack_usage);
  // cg_write (cg, "\t; Program\n");
}


static void
cg_write_end (struct cg *cg)
{
  // cg_write (cg, "\t; Leave\n");
  // cg_write (cg, "\tadd\trsp, %zu\n", cg->stack_usage);
  // cg_write (cg, "\tpop\trbp\n");
  // cg_write (cg, "\tmov\teax, 0\n");
  // cg_write (cg, "\tret\n");

  // cg_write (cg, "\n");

  cg_write (cg, "section .data\n");
  cg_write (cg, "\tprinti_s: db \"%%ld\", 10, 0\n");

  cg_write (cg, "\n");

  cg_write (cg, "section .note.GNU-stack\n");

  cg_write (cg, "\n");
}


// static void
// cg_write_function_begin (struct cg *cg)
// {
// 
// }


static void
cg_write_nop (struct cg *cg)
{
  cg_write (cg, "\tnop\n");
}


static void
cg_write_push_register (struct cg *cg, enum register_id r)
{
  cg_write (cg, "\tpush\t%s\n", register_id_string (r));
}


static void
cg_write_pop_register (struct cg *cg, enum register_id r)
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


// static void
// cg_write_load_local (struct cg *cg, register_t r, size_t offset)
// {
//   const char *s = operand_size_string (r.width);
// 
//   cg_write (cg, "\tmov\t%s, %s [rbp-%zu]\n", register_string (r), s, offset);
// }
// 
// 
// static void
// cg_write_load_local_address (struct cg *cg, register_t r, size_t offset)
// {
//   cg_write (cg, "\tlea\t%s, [rbp-%zu]\n", register_string (r), offset);
// }


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


static void
cg_write_load_global (struct cg *cg, register_t r, const char *id)
{
  const char *s = operand_size_string (r.width);

  cg_write (cg, "\tmov\t%s, %s [rel %s]\n", register_string (r), s, id);
}


static void
cg_write_load_global_address (struct cg *cg, register_t r, const char *id)
{
  cg_write (cg, "\tlea\t%s, [rel %s]\n", register_string (r), id);
}


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
  for (size_t i = REGISTERA_START; i <= REGISTERA_END; ++i)
    {
      if (cg->register_free[i] == 1)
        {
          cg->register_free[i] = 0;

          return register_create (i, width);
        }
    }

  enum register_id i = REGISTERA_START + cg->register_spill++ % REGISTERA_COUNT;

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
      enum register_id i = REGISTERA_START + --cg->register_spill % REGISTERA_COUNT;

      cg_write_pop_register (cg, i);

      // cg->stack_head -= 8;
    }
}


static void
cg_register_free_all (struct cg *cg)
{
  while (cg->register_spill > 0)
    {
      enum register_id i = REGISTERA_START + --cg->register_spill % REGISTERA_COUNT;

      cg_write_pop_register (cg, i);

      // cg->stack_head -= 8;
    }

  for (enum register_id i = 0; i < REGISTER_COUNT; ++i)
    cg->register_free[i] = 1;
}


/// GENERATE

static register_t cg_generate (struct cg *, struct tree *);


static void cg_generate_function_definition (struct cg *, struct tree *);


static void cg_generate_empty (struct cg *, struct tree *);

static void cg_generate_if (struct cg *, struct tree *);

static void cg_generate_while (struct cg *, struct tree *);

static void cg_generate_for (struct cg *, struct tree *);

static void cg_generate_compound (struct cg *, struct tree *);

static void cg_generate_variable_declaration (struct cg *, struct tree *);

static void cg_generate_return (struct cg *, struct tree *);

static void cg_generate_print (struct cg *, struct tree *);


static register_t cg_generate_cast (struct cg *, struct tree *);

static register_t cg_generate_call (struct cg *, struct tree *);

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

        scope_get2 (cg->scope, tree->token->data.s, &symbol, tree->location);

        register_t r = cg_register_allocate (cg, WIDTH_64);

        switch (symbol.storage)
          {
          case STORAGE_LOCAL:
            cg_write_load_local_address (cg, r, symbol.offset);
            break;

          case STORAGE_GLOBAL:
            cg_write_load_global_address (cg, r, symbol.key);
            break;
          }

        // cg_write_load_local_address (cg, r, symbol.offset);

        return r;
      }
    default:
      return register_none;
    }
}


static void
cg_generate_function_definition (struct cg *cg, struct tree *tree)
{
  cg->function.name = tree->child->token->data.s;

  cg->function.stack_offset = 0;
  cg->function.stack_usage = 0;

  cg->function.label_return = cg_label (cg);

  struct symbol symbol;

  symbol.key = cg->function.name;
  symbol.storage = STORAGE_GLOBAL;
  symbol.type = tree->type;

  scope_set2 (cg->scope, symbol, tree->location);

  struct tree *current;

  cg_scope_push (cg);

  for (current = tree->child->next; current->next; current = current->next)
    {
      size_t size = type_size (current->type);
      size_t alignment = type_alignment (current->type);

      cg->function.stack_offset = next_multiple (cg->function.stack_offset, alignment);
      cg->function.stack_offset = cg->function.stack_offset + size;
    }

  size_t stack_offset = cg->function.stack_offset;

  cg_compute (cg, current);

  cg->function.stack_usage = next_multiple (cg->function.stack_offset, 16);
  cg->function.stack_offset = 0;

  cg_write (cg, "\tglobal %s\n", cg->function.name);
  cg_write (cg, "%s:\n", cg->function.name);
  cg_write (cg, "\tpush\trbp\n");
  cg_write (cg, "\tmov\trbp, rsp\n");
  cg_write (cg, "\tsub\trsp, %zu\n", cg->function.stack_usage);

  cg_write (cg, "\t;\n");

  size_t p_count = 0;

  enum register_id p_registers[] = {
    REGISTER_RDI,
    REGISTER_RSI,
    REGISTER_RDX,
    REGISTER_RCX,
    REGISTER_R8,
    REGISTER_R9,
  };

  for (struct tree *p = tree->child->next; p->next; p = p->next)
    {
      size_t size = type_size (p->type);
      size_t alignment = type_alignment (p->type);

      cg->function.stack_offset = next_multiple (cg->function.stack_offset, alignment);

      size_t offset = cg->function.stack_offset + size;

      struct symbol symbol;

      symbol.key = p->child->token->data.s;
      symbol.storage = STORAGE_LOCAL;
      symbol.offset = offset;
      symbol.type = p->type;

      scope_set2 (cg->scope, symbol, current->location);

      cg->function.stack_offset = offset;

      enum type_width w = type_width (p->type);
      const char *s = operand_size_string (w);

      if (p_count < 6)
        {
          register_t r = register_create (p_registers[p_count], w);

          cg_write (cg, "\tmov\t%s [rbp-%zu], %s\n", s, offset, register_string (r));
        }
      else
        {
          register_t r = cg_register_allocate (cg, w);

          cg_write (cg, "\tmov\t%s, %s [rbp+%zu]\n", register_string (r), s, (p_count - 5 + 1) * 8);
          cg_write (cg, "\tmov\t%s [rbp-%zu], %s\n", s, offset, register_string (r));

          cg_register_free (cg, r);
        }

      p_count++;
    }

  cg->function.stack_offset = stack_offset;

  cg_write (cg, "\t;\n");

  // Bypass generating compound
  for (struct tree *child = current->child; child; child = child->next)
    {
      cg_generate (cg, child);

      cg_register_free_all (cg);
    }

  cg_write (cg, "\t;\n");

  cg_write_label (cg, cg->function.label_return);
  cg_write (cg, "\tadd\trsp, %zu\n", cg->function.stack_usage);
  cg_write (cg, "\tpop\trbp\n");
  cg_write (cg, "\tret\n");

  cg_write (cg, "\n");

  cg_scope_pop (cg);
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
  size_t alignment = type_alignment (type);

  cg->function.stack_offset = next_multiple (cg->function.stack_offset, alignment);

  size_t offset = cg->function.stack_offset + size;

  struct symbol symbol;

  symbol.key = node->token->data.s;

  symbol.offset = offset;

  symbol.storage = STORAGE_LOCAL;

  symbol.type = type;

  scope_set2 (cg->scope, symbol, tree->location);

  cg->function.stack_offset = offset;
}


static void
cg_generate_return (struct cg *cg, struct tree *tree)
{
  if (tree->child)
    {
      register_t a = cg_generate (cg, tree->child);
      register_t b = register_create (REGISTER_RAX, a.width);

      cg_write (cg, "\tmov\t%s, %s\n", register_string (b), register_string (a));
    }

  cg_write_jmp (cg, cg->function.label_return);
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
cg_generate_call (struct cg *cg, struct tree *tree)
{
  struct tree *current;

  size_t arity = 0;

  enum register_id p_registers[] = {
    REGISTER_RDI,
    REGISTER_RSI,
    REGISTER_RDX,
    REGISTER_RCX,
    REGISTER_R8,
    REGISTER_R9,
  };

  for (current = tree->child->next; current; current = current->next)
    {
      // TODO: Call functions with more than 6 arguments
      if (arity >= 6)
        break;

      register_t a = cg_generate (cg, current->child);
      register_t b = register_create (p_registers[arity], a.width);

      cg_write (cg, "\tmov\t%s, %s\n", register_string (b), register_string (a));

      cg_register_free (cg, a);

      arity++;
    }

  register_t a = cg_generate (cg, tree->child);

  for (size_t i = REGISTERA_START; i <= REGISTERA_END; ++i)
    cg_write_push_register (cg, i);

  // TODO: SUPPORT DIRECT CALLS
  cg_write (cg, "\tcall\t%s\n", register_string (a));

  for (size_t i = REGISTERA_END; i >= REGISTERA_START; --i)
    cg_write_pop_register (cg, i);

  register_t r = register_create (REGISTER_RAX, type_width (tree->type));

  cg_write (cg, "\tmov\t%s, %s\n", register_string (a), register_string (r));

  return a;
}


static register_t
cg_generate_assignment (struct cg *cg, struct tree *tree)
{
  struct tree *node_a = tree->child;
  struct tree *node_b = tree->child->next;

  register_t b = cg_generate (cg, node_b);
  register_t a = cg_generate_left_value (cg, node_a);

  // TODO: Handle both local and global variables.
  //       Though that requires the implementation of global variables first.

  cg_write_store (cg, a, b);

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

  scope_get2 (cg->scope, tree->token->data.s, &symbol, tree->location);

  register_t r = cg_register_allocate (cg, type_width (tree->type));

  switch (symbol.storage)
    {
    case STORAGE_LOCAL:
      cg_write_load_local (cg, r, symbol.offset);
      break;

    case STORAGE_GLOBAL:
      cg_write_load_global (cg, r, symbol.key);
      break;
    }

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
    case TREE_FUNCTION_DEFINITION:
      cg_generate_function_definition (cg, tree);
      return register_none;
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
    case TREE_RETURN:
      cg_generate_return (cg, tree);
      return register_none;
    case TREE_PRINT:
      cg_generate_print (cg, tree);
      return register_none;
    case TREE_CAST:
      return cg_generate_cast (cg, tree);
    case TREE_CALL:
      return cg_generate_call (cg, tree);
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

  // cg->stack_usage = next_multiple (cg->stack_offset, 16);

  // cg->stack_offset = 0;

  cg_write_begin (cg);

  cg_generate (cg, tree);

  cg_write_end (cg);
}

