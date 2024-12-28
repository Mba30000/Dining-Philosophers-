#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 3

typedef struct {
    int data[QUEUE_SIZE];
    int head;
    int tail;
} Queue_t;

void init_queue(Queue_t *queue);
void queue(Queue_t *queue, int num);
int special_dequeue(Queue_t *queue, int num);

#endif
