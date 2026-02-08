#ifndef CG_H
#define CG_H


#include <stdio.h>


struct tree;

struct cg;


struct cg *
cg_create (FILE *f);

void
cg_generate (struct cg *cg, struct tree *tree);


#endif // CG_H

