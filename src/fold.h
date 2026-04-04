#ifndef FOLD_H
#define FOLD_H


#include <stdbool.h>


struct tree;


bool
fold_tree (struct tree **r, struct tree *tree);


struct tree *
fold_tree_strict (struct tree *tree);


#endif // FOLD_H
