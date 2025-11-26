#include "memory.h"
#include "error.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define AA_CHUNK_SIZE (0x10000)


struct aa_chunk
{
  struct aa_chunk *next;

  uint8_t *head;
  uint8_t *tail;

  uint8_t data[];
};


static struct aa_chunk *
aa_chunk_create (size_t n_bytes)
{
  if (n_bytes < AA_CHUNK_SIZE)
    n_bytes = AA_CHUNK_SIZE;

  size_t a_bytes = sizeof (struct aa_chunk) + n_bytes + ALIGN_MAX - 1;

  struct aa_chunk *chunk;

  chunk = calloc (1, a_bytes);

  chunk->next = NULL;

  chunk->head = chunk->data;
  chunk->tail = chunk->data + n_bytes;

  return chunk;
}


static struct aa_chunk *head = NULL;
static struct aa_chunk *tail = NULL;


static void *
align_forward_p (void *p, size_t a)
{
  uintptr_t x = (uintptr_t)p;
  uintptr_t m = (a - 1);
  uintptr_t y = (x + m) & ~m;
  return (void *)y;
}


void
*aa_malloc (size_t n_bytes)
{
  struct aa_chunk *chunk = tail;

  if (chunk == NULL)
    {
      chunk = aa_chunk_create (n_bytes);

      head = chunk;
      tail = chunk;
    }

  uint8_t *head = align_forward_p (chunk->head, ALIGN_MAX);
  uint8_t *next = head + n_bytes;

  if (next > chunk->tail)
    {
      chunk = aa_chunk_create (n_bytes);

      tail = tail->next = chunk;

      head = align_forward_p (chunk->head, ALIGN_MAX);
      next = head + n_bytes;
    }

  chunk->head = next;

  return head;
}


const char *
size_to_human (size_t bytes, char *buffer, size_t buffer_size)
{
  static const char *const UNITS[] = {"B", "K", "M", "G", "T", "P"};

  size_t unit = 0;

  double size = (double)bytes;

  while (size >= 1024 && unit < sizeof (UNITS) / sizeof (char *))
    {
      size /= 1024;
      unit++;
    }

  snprintf (buffer, buffer_size, "%.1f%s", size, UNITS[unit]);

  return buffer;
}


void
aa_free (void)
{
#if 1
  {
    size_t used = 0;
    size_t size = 0;
    size_t n = 0;

    struct aa_chunk *chunk = head;

    while (chunk)
      {
        used += chunk->head - chunk->data;
        size += AA_CHUNK_SIZE;

        n++;

        chunk = chunk->next;
      }

    if (n != 0)
      {
        char buffer1[64];
        char buffer2[64];

        size_to_human (used, buffer1, sizeof buffer1);
        size_to_human (size, buffer2, sizeof buffer2);

        info (location_none, "%s / %s (%ld)", buffer1, buffer2, n);
      }
  }
#endif

  struct aa_chunk *chunk = head;

  while (chunk)
    {
      struct aa_chunk *next = chunk->next;

      free (chunk);

      chunk = next;
    }

  head = NULL;
  tail = NULL;
}

