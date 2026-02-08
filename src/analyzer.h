#ifndef ANALYZER_H
#define ANALYZER_H


struct tree;

struct analyzer;


struct analyzer *
analyzer_create (void);

void
analyzer_analyze (struct analyzer *analyzer, struct tree *tree);


#endif // ANALYZER_H

