#ifndef ALIGNMENT_H
#define ALIGNMENT_H


#include <stddef.h>


size_t
align_forward (size_t x, size_t a);

void *
align_forward_p (void *p, size_t a);

size_t
next_multiple (size_t i, size_t n);


#endif // ALIGNMENT_H

