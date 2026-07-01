#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>

typedef struct __pq_node{
    void *data;
    int priority;
    struct __pq_node *next;
}__PQ_NODE;

typedef struct{
    __PQ_NODE *first;
    size_t item_size;
    size_t size;
    int current_priority;
}PriorityQueue;

PriorityQueue * pq_new(size_t item_size);
int pq_push(PriorityQueue *queue, void *data, int priority);
int pq_pop(PriorityQueue *queue, void *dest);
int pq_peek(PriorityQueue *queue, void *dest);
int pq_contains(PriorityQueue *queue, void *data);
PriorityQueue * pq_concat(PriorityQueue *queue_1, PriorityQueue *queue_2);
PriorityQueue * pq_clone(PriorityQueue *queue);
int pq_is_empty(PriorityQueue *queue);
int pq_get_size(PriorityQueue *queue);
int pq_get_priority(PriorityQueue *queue);
void pq_clear(PriorityQueue *queue);
void pq_destroy(PriorityQueue **queue);
void * pq_to_array(PriorityQueue *queue, int *length);
void pq_print(PriorityQueue *queue, char *(*to_string)(void *));

#endif