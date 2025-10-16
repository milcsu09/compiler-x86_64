#ifndef CG_H
#define CG_H


#include "tree.h"

#include <stdio.h>


enum
{
  REGISTER_N = 4,
};


typedef size_t register_id;


struct cg
{
  FILE *f;

  struct scope *scope;

  size_t stack_offset;
  // size_t stack_head;
  size_t stack_usage;

  size_t label_count;

  bool register_free[REGISTER_N];
  register_id register_spill;
};


struct cg *cg_create (FILE *);

void cg_emit (struct cg *, struct tree *);


#endif // CG_H

