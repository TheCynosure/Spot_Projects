#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minqueue.h"

int main(int argc, char** argv)
{
    //Test that the queue can have all these elements added in reverse order and order them 
    //correctly.
    void* buff = malloc(500);
    MinQueue queue = {
        .head = NULL
    };

    printf("Buffer starts at %lu\n", &buff);
    for(int i = 10; i >= 0; i--) {
        printf("Placing QueueNode at offset %lu\n", i * sizeof(QueueNode));
        QueueNode *temp = (QueueNode*)(&buff + (i * sizeof(QueueNode)));
        temp->data = i;
        printf("Adding %lu\n", temp->data);
        if(queue.head)
            printf("Current Head: %lu\n", queue.head->data);
        mq_addn(&queue, temp);
        printf("New Head: %lu\n", queue.head->data);
    }

    //Remove all the elements
    while(!mq_is_empty(&queue)) {
        mq_remove(&queue, queue.head);
    }

    assert(mq_is_empty(&queue));

    printf("Freeing\n");
    free(buff);
    return 0;
}
