#ifndef CHECKER_H
#define CHECKER_H


struct tree;


struct checker
{
  char dummy;
};


struct checker *checker_create (void);

void checker_check (struct checker *, struct tree *);


#endif // CHECKER_H

