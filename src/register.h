#ifndef REGISTER_H
#define REGISTER_H


#include "type.h"


enum cg_register_id
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


const char *
register_id_string (enum cg_register_id id);


struct cg_register
{
  enum cg_register_id id;

  enum type_width w;
};


extern const struct cg_register register_none;


const char *
register_b_string (struct cg_register r);

const char *
register_w_string (struct cg_register r);

const char *
register_d_string (struct cg_register r);

const char *
register_q_string (struct cg_register r);

const char *
register_string (struct cg_register r);


struct cg_register
register_create (enum cg_register_id id, enum type_width w);

struct cg_register
register_modify (struct cg_register r, enum type_width w);


#endif // REGISTER_H

