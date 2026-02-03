#include "alignment.h"

#include <stdint.h>


size_t
align_forward (size_t x, size_t a)
{
  size_t m = a - 1;

  return (x + m) & ~m;
}


void *
align_forward_p (void *p, size_t a)
{
  uintptr_t x = (uintptr_t)p;
  uintptr_t m = a - 1;

  return (void *)((x + m) & ~m);
}


size_t
next_multiple (size_t i, size_t n)
{
  return (1 + (i - 1) / n) * n;
}

