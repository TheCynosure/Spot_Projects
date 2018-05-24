#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "memalloc.h"
#include "minqueue.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define MEM_SIZE 1024

int main(int argc, char** argv)
{
	void* buffer = malloc(MEM_SIZE);
    init_malloc(buffer, MEM_SIZE);

    //Try to allocate something larger than the buffer.
    //This should fail and return a null ptr.
    void *overfull_ptr = c_malloc(MEM_SIZE);
    assert(overfull_ptr == NULL);
    c_free(overfull_ptr);

    //Try to allocate two blocks.
    //This should work on both accounts.
    void *first_half = c_malloc((MEM_SIZE / 2) - sizeof(QueueNode));
    assert(first_half);
    void *second_half = c_malloc((MEM_SIZE / 2) - sizeof(QueueNode));
    assert(second_half);
    //Should fail as you can't have three halves! :/
    void *third_half = c_malloc((MEM_SIZE / 2) - sizeof(QueueNode));
    assert(!third_half);

    c_free(first_half);
    c_free(second_half);
    c_free(third_half);

    //Now we should check if these holes were combined properly.
    //Allocate a block that is as large as usable memory.
    void *full_ptr = c_malloc(MEM_SIZE - sizeof(QueueNode));
    assert(full_ptr);
    c_free(full_ptr);

    //Should fail as you can't allocate negative space.
    void *neg_ptr = c_malloc(-MEM_SIZE);
    assert(!neg_ptr);
    c_free(neg_ptr);

    free(buffer);
    return 0;
}
