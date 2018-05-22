#ifndef MEMALLOC_H
#define MEMALLOC_H

#include <stdint.h>

void init_malloc(void* holes_start, uint64_t size);
void *c_malloc(uint64_t size);
void c_free(void *ptr);

#endif
