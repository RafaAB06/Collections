#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct __q_node Node;

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&

static void free_node(Node *node);
static Node * create_node(void *data, size_t item_size);

Queue * q_new(size_t item_size){
    Queue *new_queue = (Queue *)malloc(sizeof(Queue));
    if(not new_queue){
        printf("Out of Memory Exception at Queue::q_new()!\n");
        return NULL;
    }

    new_queue->first = NULL;
    new_queue->last = NULL;
    new_queue->size = 0;
    new_queue->item_size = item_size;
    
    return new_queue;
}

Queue * q_new_from_array(size_t item_size, void *array, size_t length){
    Queue *new_queue = (Queue *)malloc(sizeof(Queue));
    if(not new_queue){
        printf("Out of Memory Exception at Queue::q_new_from_array()!\n");
        return NULL;
    }

    int i;
    for(i = 0; i<length; i++){
        Node *new_node = create_node((char *)array + i * item_size, item_size);
        if(new_node){
            Node *first = new_queue->first;
            new_node->next = first;
            new_queue->first = new_node;
        }else{
            printf("Out of Memory Exception at Queue::q_new_from_array()!\n");
            q_destroy(&new_queue);
            return NULL;
        }
    }
    new_queue->size = length;
    new_queue->item_size = item_size;

    return new_queue;
}

int q_push(Queue *queue, void *data){
    if(not queue or not data){
        printf("Null Reference Exception at Queue::q_push!\n");
        return ERROR;
    }
    Node *new_node = create_node(data, queue->item_size);
    if(not new_node){
        printf("Out of Memory Exception at Queue::q_push()!\n");
        return ERROR;
    }

    Node* last = queue->last;
    queue->last = new_node;
    queue->size++;
    if(q_is_empty(queue)){
        queue->first = new_node;
    }else{
        last->next = new_node;
    }
    return SUCCESS;
}

int q_pop(Queue *queue, void* dest){
    if(not queue or not dest){
        printf("Null Reference Exception at Queue::q_pop()!\n");
        return ERROR;
    }
    if(q_is_empty(queue)){
        return ERROR;
    }
    Node *first = queue->first;
    Node *next = first->next;
    memcpy(dest, first->data, queue->item_size);
    free_node(first);

    queue->first = next;
    if(queue->size == 1) queue->last = NULL;
    queue->size--;

    return SUCCESS;
}

int q_peek(Queue *queue, void *dest){
    if(not queue or not dest){
        printf("Null Reference Exception at Queue::q_peek())!\n");
        return ERROR;
    }
    if(q_is_empty(queue)){
        return ERROR;
    }
    Node *first = queue->first;
    memcpy(dest, first->data, queue->item_size);

    return SUCCESS;
}

int q_contains(Queue *queue, void *data){
    if(not queue or not data){
        printf("Null Reference Exception at Queue::q_peek())!\n");
        return 0;
    }
    if(q_is_empty(queue)){
        return 0;
    }

    Node *current = queue->first;
    while (current)
    {
        void *current_data = current->data;
        if(not memcmp(current_data, data, queue->item_size)){
            return 1;
        }   
        current = current->next;
    }
    return 0;
}

static int q_concat_insert(Node *current, Queue *new_queue, size_t item_size){
    while(current){
        Node *new_node = create_node(current->data, item_size);
        if(not new_node){
            printf("Out of Memory Exception at Queue::q_new()!\n");
            q_destroy(&new_queue);
            return 0;
        }
        Node *last = new_queue->last;
        if(last){
            last->next = new_node;
        }
        new_queue->last = new_node;
        current = current->next;
        if(not new_queue->first){
            new_queue->first = new_node;
        }
    }
    return 1;
}

Queue * q_concat(Queue *queue_1, Queue *queue_2){
    if(not queue_1 or not queue_2){
        printf("Null Reference Exception at Queue::q_concat()!\n");
        return NULL;
    }
    if(queue_1->item_size != queue_2->item_size){
        printf("Invalid Value Exception at Stack::q_concat()!\n");
        return NULL;
    }
    Queue *new_queue = (Queue *)malloc(sizeof(Queue));
    if(not new_queue){
        printf("Out of Memory Exception at Queue::q_new()!\n");
        return NULL;
    }
    new_queue->first = NULL;
    new_queue->last = NULL;

    size_t item_size = queue_1->item_size;
    if(not q_concat_insert(queue_1->first, new_queue, item_size)
    or not q_concat_insert(queue_2->first, new_queue, item_size)){
        return NULL;
    }

    new_queue->size = queue_1->size + queue_2->size;
    new_queue->item_size = item_size;
    return new_queue;
}

Queue * q_clone(Queue *queue){
    if(not queue){
        printf("Null Reference Exception at Queue:q_clone()!\n");
        return NULL;
    }
    Queue *clone = (Queue *)malloc(sizeof(Queue));
    if(not clone){
        printf("Out of Memory Exception at Queue::q_clone()!\n");
        return NULL;
    }
    clone->first = NULL;
    clone->last = NULL;

    Node *current = queue->first;
    while (current)
    {
        Node *new_node = create_node(current->data, queue->item_size);
        if(not new_node){
            printf("Out of Memory Exception at Queue::q_clone()!\n");
            q_destroy(&clone);
            return NULL;
        }
        Node *last = clone->last;
        if(last){
            last->next = new_node;
        }
        if(not clone->first){
            clone->first = new_node;
        }
        clone->last = new_node;
        current = current->next;
    }
    clone->item_size = queue->item_size;
    clone->size = queue->size;
    return clone;
}

void q_reverse(Queue *queue){
    if(q_get_size(queue) <= 1) return;
    Node *current = queue->first;
    Node *last = queue->last;

    Node *previous = NULL;
    Node *next = NULL;
    while (current)
    {
        next = current->next;
        current->next = previous;

        previous = current;
        current = next;
    }

    queue->first = last;
    queue->last = current;
}

int q_is_empty(Queue *queue){
    return queue and not queue->first;
}

int q_get_size(Queue *queue){
    return queue ? queue->size : 0;
}

void q_clear(Queue *queue){
    if(queue){
        Node *current = queue->first;
        while (current)
        {
            Node *next = current->next;
            free_node(current);
            current = next;
        }
        queue->first = NULL;
        queue->size = 0;
    }
}

void q_destroy(Queue **queue){
    if(queue and *queue){
        q_clear(*queue);
        free(*queue);
        *queue = NULL;
    }
}

void * q_to_array(Queue *queue, int *length){
    if(not queue or not length){
        printf("Null Reference Exception at Queue:q_to_array()!\n");
        return NULL;
    }
    if(q_is_empty(queue)){
        *length = 0;
        return NULL;
    }
    size_t item_size = queue->item_size;
    size_t size = queue->size;
    char *array = (char *)malloc(size * item_size);
    if(not array){
        printf("Out of Memory Exception at Queue::q_to_array()!\n");
        return NULL;
    }

    Node *current = queue->first;
    int i;
    for(i = 0; i<queue->size; i++){
        char *pos = array + i*size;
        memcpy(pos, current->data, item_size);

        current = current->next;
    }
    *length = size;
    return array;
}

void q_print(Queue *queue, char *(*to_string)(void *)){
    if(not to_string){
        printf("Null Reference Exception at Queue::q_print()!\n");
        return;
    }
    printf("Queue:\n");
    if(not queue){
        printf("NULL\n");
        return;
    }
    if(q_is_empty(queue)){
        printf("Empty\n");
        return;
    }
    printf("Size -> %d\n", queue->size);
    
    Node* current = queue->first;
    int i = 0;
    while (current)
    {
        char *str = to_string(current->data);
        printf("%d - %s\n", i, str);
        free(str);

        i++;
        current = current->next;
    }
}

static void free_node(Node *node){
    free(node->data);
    free(node);
}

static Node * create_node(void *data, size_t item_size){
    if(data){
        Node *new_node = (Node *)malloc(sizeof(Node));
        if(new_node){
            void *new_data = malloc(item_size);
            if(new_data){
                memcpy(new_data, data, item_size);
                new_node->data = new_data;
                new_node->next = NULL;
                return new_node;
            }
            free(new_node);
        }
    }
    return NULL;
}