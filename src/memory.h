#ifndef MEMORY_H
#define MEMORY_H


#include <stddef.h>


enum
{
  // NOTE: Good enough
  ALIGN_MAX = 16,
};


void *
aa_malloc (size_t n_bytes);

void
aa_free (void);


#endif // MEMORY_H

