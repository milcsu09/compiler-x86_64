#ifndef CG_H
#define CG_H


#include <stdio.h>


struct tree;

struct cg;


struct cg *cg_create (FILE *);

void cg_generate (struct cg *, struct tree *);


#endif // CG_H

