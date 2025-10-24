#include "register.h"

#include <assert.h>


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
  "r8",
  "r9",
  "r10",
  "r11",
  "r12",
  "r13",
  "r14",
  "r15",
};


const char *
register_id_string (enum cg_register_id id)
{
  return REGISTER_STRING[id + REGISTER_COUNT * 3];
}


const char *
register_b_string (struct cg_register r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 0];
}


const char *
register_w_string (struct cg_register r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 1];
}


const char *
register_d_string (struct cg_register r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 2];
}


const char *
register_q_string (struct cg_register r)
{
  return REGISTER_STRING[r.id + REGISTER_COUNT * 3];
}


const char *
register_string (struct cg_register r)
{
  switch (r.w)
    {
    case WIDTH_1:
      return register_b_string (r);
    case WIDTH_2:
      return register_w_string (r);
    case WIDTH_4:
      return register_d_string (r);
    case WIDTH_8:
      return register_q_string (r);
    default:
      assert (!"Invalid");
    }
}


const struct cg_register register_none = { 0 };


struct cg_register
register_create (enum cg_register_id id, enum type_width w)
{
  struct cg_register r;

  r.id = id;
  r.w = w;

  return r;
}


struct cg_register
register_modify (struct cg_register r, enum type_width w)
{
  r.w = w;

  return r;
}

