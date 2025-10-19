#ifndef CG_H
#define CG_H


#include "tree.h"

#include <stdio.h>


enum register_id
{
  REGISTER_RAX,
  REGISTER_RBX,
  REGISTER_RCX,
  REGISTER_RDX,
  REGISTER_RSI,
  REGISTER_RDI,
  REGISTER_RSP,
  REGISTER_RBP,
  REGISTER_R8,
  REGISTER_R9,
  REGISTER_R10,
  REGISTER_R11,
  REGISTER_R12,
  REGISTER_R13,
  REGISTER_R14,
  REGISTER_R15,

  REGISTER_COUNT,
};


enum
{
  REGISTERA_START = REGISTER_R10,
  REGISTERA_END   = REGISTER_R15,

  REGISTERA_COUNT = REGISTERA_START - REGISTERA_END,
};


typedef size_t label_t;


struct function
{
  char *name;

  size_t stack_offset;
  size_t stack_usage;

  label_t label_return;
};


struct cg
{
  FILE *f;

  struct function function;

  struct scope *scope;

  // size_t stack_offset;
  // size_t stack_head;
  // size_t stack_usage;

  size_t label_count;

  bool register_free[REGISTER_COUNT];

  size_t register_spill;
};


struct cg *cg_create (FILE *);

void cg_emit (struct cg *, struct tree *);


#endif // CG_H

