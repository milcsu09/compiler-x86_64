#ifndef MEMORY_H
#define MEMORY_H


#include <stddef.h>


enum
{
  // NOTE: Good enough
  ALIGN_MAX = 16,
};


#define aa_malloc_t(T) aa_malloc (sizeof (T))


void *aa_malloc (size_t);

void aa_free (void);


#endif // MEMORY_H

