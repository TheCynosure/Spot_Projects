#include "memalloc.h"
#include "minqueue.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/* Super simple memory allocater that just looks for the first hole that fits the requested size.
 * Then it cuts the excess off of that hole and creates a hole farther down.
 */

//Queues to keep track of the allocated chunks and holes in between.
static MinQueue allocated;
static MinQueue holes;

void init_malloc(void *holes_start, uint64_t init_size)
{
    allocated.head = NULL;
    allocated.size = 0;
    //Initial size of the first hole should be the addr where it starts to the end of memory.
    holes.head = (QueueNode*) holes_start;
    holes.head->data = init_size - sizeof(QueueNode);
    holes.head->next = NULL;
    holes.size = 1;
}

//This requires that holes is atleast initialized before calling this!
void *malloc(uint64_t size)
{
    //Look for hole that is big enough.
    QueueNode *curr_hole;
    for(curr_hole = holes.head; curr_hole != NULL && curr_hole->data < size; curr_hole = curr_hole->next);
    if(!curr_hole)
        return NULL;
    //Now we need to check if we need to make another hole.
    //(Size of this allocated block) - (Header size for next hole)
    uint64_t next_hole_size = (curr_hole->data - size) - sizeof(QueueNode);
    if(next_hole_size > 0) {
        //This means that we should plan to create a new hole.
        void* addr_of_next_hole = (void*)(curr_hole) + sizeof(QueueNode) + size;
        mq_add(&holes, addr_of_next_hole, next_hole_size);
    }
    //We should swap this hole node into the allocated list.
    //We need to 'remove' it from the hole node list, this WILL NOT modify any data within this node.
    mq_remove(&holes, curr_hole);
    //Then we need to use the original address on the heap.
    mq_addn(&allocated, curr_hole);
    //Then finally return the pointer.
    return curr_hole;
}


void free(void *ptr)
{
    //If we get a null ptr then this malloc failed, just quit
    if(!ptr)
        return;
    //Remove from the allocated list
    mq_remove(&allocated, (QueueNode*)ptr);
    //Add the pointer to the hole list.
    mq_addn(&holes, (QueueNode*) ptr);
    //Check if the following hole can be combined with this one repeatedly.
    QueueNode *curr = ptr;
    while(curr->next != NULL && (curr + curr->data) == curr->next) {
        curr->data += curr->next->data;
        mq_remove(&holes, curr->next);
    }
}
