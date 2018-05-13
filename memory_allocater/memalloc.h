#ifndef MEMALLOC_H
#define MEMALLOC_H

#include <stdint.h>

void *malloc(uint64_t size);
void free(void *ptr);

#endif
