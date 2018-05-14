#include <assert.h>
#include "memalloc.h"
#include "minqueue.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define MEM_SIZE 1024

int main(int argc, char** argv)
{
    char buffer[MEM_SIZE];
    init_malloc(buffer, MEM_SIZE);

    //Try to allocate something larger than the buffer.
    //This should fail and return a null ptr.
    void *overfull_ptr = malloc(MEM_SIZE);
    assert(overfull_ptr == NULL);
    free(overfull_ptr);

    //Try to allocate two blocks.
    //This should work on both accounts.
    void *first_half = malloc((MEM_SIZE / 2) - sizeof(QueueNode));
    assert(first_half);
    void *second_half = malloc((MEM_SIZE / 2) - sizeof(QueueNode));
    assert(second_half);
    free(first_half);
    free(second_half);
    
    //Now we should check if these holes were combined properly.
    //Allocate a block that is as large as usable memory.
    void *full_ptr = malloc(MEM_SIZE - sizeof(QueueNode));
    assert(full_ptr);
    free(full_ptr);

    return 0;   
}
