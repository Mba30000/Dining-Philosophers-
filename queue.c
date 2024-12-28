#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue(Queue_t *queue) {               //initialize queue with -1s
        queue->head = 0;                        //save the index of the head and tail
        queue->tail = 0;
        for (int i = 0; i < QUEUE_SIZE; i++) {
                queue->data[i] = -1;
        }
}

void queue(Queue_t *queue, int num) {           // Enqueue logic; handling was omitted since 3 people share a fork
        queue->data[queue->tail] = num;         // The tail points to the last empty spot in the queue
        queue->tail = (queue->tail + 1) % QUEUE_SIZE;
}

int special_dequeue(Queue_t *queue, int num) {  // special equeue that returns true if the number is the head
    if (queue->data[queue->head] == num) {
        queue->data[queue->head] = -1;
        queue->head = (queue->head + 1) % QUEUE_SIZE;
        return 1;
    }

    return 0;
}
