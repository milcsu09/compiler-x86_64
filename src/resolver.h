#ifndef RESOLVER_H
#define RESOLVER_H


struct tree;

struct resolver;


struct resolver *resolver_create (void);

void resolver_resolve (struct resolver *, struct tree *);


#endif // RESOLVER_H

