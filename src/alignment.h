#ifndef ALIGNMENT_H
#define ALIGNMENT_H


#include <stddef.h>


size_t align_forward (size_t, size_t);

void *align_forward_p (void *, size_t);

size_t next_multiple (size_t, size_t);


#endif // ALIGNMENT_H

