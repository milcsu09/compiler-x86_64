#ifndef MEMORY_H
#define MEMORY_H


#include <stddef.h>


enum
{
  // NOTE: Good enough
  ALIGN_MAX = 16,
};


void *
malloc0 (size_t size);


void *
aa_malloc (size_t n_bytes);

void
aa_free (void);


#endif // MEMORY_H

