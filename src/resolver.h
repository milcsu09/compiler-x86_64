#ifndef RESOLVER_H
#define RESOLVER_H


struct tree;


struct resolver
{
  struct scope *scope;
};


struct resolver *resolver_create (void);

struct tree *resolver_resolve (struct resolver *, struct tree *);


#endif // RESOLVER_H

