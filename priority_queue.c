#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "priority_queue.h"

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&

typedef __PQ_NODE Node;

static void free_node(Node *node);
static Node * create_node(size_t item_size, void *data, int priority);

PriorityQueue * pq_new(size_t item_size){
    PriorityQueue *new = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if(not new){
        printf("Out of Memory Exception at PriorityQueue::pq_new()!\n");
        return NULL;
    }

    new->current_priority = 0;
    new->size = 0;
    new->item_size = item_size;
    new->first = NULL;

    return new;
}

int pq_push(PriorityQueue *queue, void *data, int priority){
    if(not queue or not data){
        printf("Null Reference Exception at PriorityQueue::pq_push()!\n");
        return ERROR;
    }
    Node *new_node = create_node(queue->item_size, data, priority);
    if(not new_node){
        printf("Out of Memory Exception at PriorityQueue::pq_pussh()!\n");
        return ERROR;
    }

    Node *current = queue->first, *previous = NULL;
    while (current and current->priority >= new_node->priority)
    {
        previous = current;
        current = current->next;
    }
    if(previous){
        previous->next = new_node;
    }else{
        queue->first = new_node;
    }
    new_node->next = current;
    queue->size++;

    if(queue->current_priority < priority){
        queue->current_priority = priority;
    }

    return SUCCESS;
}

int pq_pop(PriorityQueue *queue, void *dest){
    if(not queue or not dest){
        printf("Null Reference Exception at PriorityQueue::pq_pop()!\n");
        return ERROR;
    }
    if(pq_is_empty(queue)){
        return ERROR;
    }
    Node *first = queue->first;
    Node *next = first->next;
    
    memcpy(dest, first->data, queue->item_size);
    free_node(first);

    queue->first = next;
    queue->current_priority = next ? next->priority : 0;
    queue->size--;

    return SUCCESS;
}

int pq_peek(PriorityQueue *queue, void *dest){
    if(not queue or not dest){
        printf("Null Reference Excepition at PriorityQueue::pq_peek()!\n");
        return ERROR;
    }
    if(pq_is_empty(queue)){
        return ERROR;
    }
    Node *first = queue->first;
    memcpy(dest, first->data, queue->item_size);

    return SUCCESS;
}

int pq_contains(PriorityQueue *queue, void *data){
    if(not queue or not data){
        printf("Null Reference Exception at PriorityQueue::pq_contains()!\n");
        return 0;
    }
    Node *current = queue->first;
    while(current){
        if(not memcmp(current->data, data, queue->item_size)){
            return 1;
        }
        current = current->next;
    }

    return 0;
}

PriorityQueue * pq_concat(PriorityQueue *queue_1, PriorityQueue *queue_2){
    if(not queue_1 or not queue_2){
        printf("Null Reference Exception at PrioritQueue::pq_concat()!\n");
        return NULL;
    }
    if(queue_1->item_size != queue_2->item_size){
        printf("Invalid Value Exception at PriorityQueue::pq_concat()!\n");
        return NULL;
    }
    PriorityQueue *queue_3 = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if(not queue_3){
        printf("Out of Memory Exception at PriorityQueue::pq_concat()!\n");
        return NULL;
    }
    queue_3->item_size = queue_1->item_size;
    queue_3->size = queue_2->size + queue_1->size;
    queue_3->first = NULL;
    queue_3->current_priority = 0;

    Node *last_node = NULL;
    Node *current = queue_1->first;
    Node *other = queue_2->first;
    while (current or other)
    {
        if(current and other){
            if(current->priority < other->priority){
                Node *temp = current;
                current = other;
                other = temp;
            }
        }else if(not current){
            current = other;
            other =  NULL;
        }
        Node *new_node = create_node(queue_3->item_size, current->data, current->priority);
        if(not new_node){
            printf("Out of Memory Exception at PriorityQueue::pq_concat()!\n");
            pq_destroy(&queue_3);
            return NULL;
        }
        if(last_node){
            last_node->next = current;
        }else{
            queue_3->first = current;
        }
        last_node = current;

        current = current->next;
    }
    return queue_3;
}

static int insert_beginning_recursive(PriorityQueue *clone, Node *node){
    if(node){
        if(not insert_beginning_recursive(clone, node->next)){
            return 0;
        }
        Node *new = create_node(clone->item_size, node->data, node->priority);
        if(not new){
            return 0;
        }
        Node *first = clone->first;
        new->next = first;
        clone->first = new;

        if(clone->current_priority < new->priority){
            clone->current_priority = new->priority;
        }
    }
    return 1;
}

PriorityQueue * pq_clone(PriorityQueue *queue){
    if(not queue){
        printf("Null Reference Exception at PriorityQueue::oq_clone()!\n");
        return NULL;
    }

    PriorityQueue *clone = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if(not clone){
        printf("Out of Memory Exception at PriorityQueue::pq_clone()!\n");
        return NULL;
    }
    clone->current_priority = 0;
    clone->first = NULL;
    clone->item_size = queue->item_size;
    clone->size = queue->size;

    if(not insert_beginning_recursive(clone, queue->first)){
        printf("Out of Memory Exception at  PriorityQueue::pq_clone()!\n");
        pq_destroy(&clone);
        return NULL;
    }

    return clone;
}

int pq_is_empty(PriorityQueue *queue){
    return queue and not queue->first;
}

int pq_get_size(PriorityQueue *queue){
    return queue ? queue->size : 0;
}

int pq_get_priority(PriorityQueue *queue){
    return queue ? queue->current_priority : 0;
}

void pq_clear(PriorityQueue *queue){
    if(queue){
        Node *current = queue->first;
        while (current)
        {
            Node *next = current->next;
            free_node(current);
            current = next;
        }
        queue->current_priority = 0;
        queue->first = NULL;
        queue->item_size = 0;
        queue->size = 0;
    }
}
void pq_destroy(PriorityQueue **queue){
    if(queue){
        pq_clear(*queue);
        free(*queue);
        *queue = NULL;
    }
}
void * pq_to_array(PriorityQueue *queue, int *length){
    if(not queue or not length){
        printf("Null Reference Exception at PriorityQueue::pq_to_array()!\n");
        return NULL;
    }
    if(pq_is_empty(queue)){
        *length = 0;
        return NULL;
    }
    void *array = malloc(queue->size * queue->item_size);
    if(not array){
        printf("Out of Memory Exception at PriorityQueue::pq_to_array()!\n");
        return NULL;
    }

    Node *current = queue->first;
    int i = 0;
    while (current)
    {
        void *pos = array + i * queue->item_size;
        memcpy(pos, current->data, queue->item_size);

        current = current->next;
        i++;
    }

    *length = i;
    return array;
}

void pq_print(PriorityQueue *queue, char *(*to_string)(void *)){
    if(not to_string){
        printf("Null Reference Exception at PriorityQueue::pq_print()!\n");
        return;
    }
    printf("Priority Queue:\n");
    if(not queue){
        printf("NULL\n");
        return;
    }
    if(pq_is_empty(queue)){
        printf("Empty\n");
        return;
    }
    printf("Size -> %d\n", queue->size);
    
    Node* current = queue->first;
    while (current)
    {
        char *str = to_string(current->data);
        printf("%d - %s\n", current->priority, str);
        free(str);

        current = current->next;
    }
}

static Node * create_node(size_t item_size, void *data, int priority){
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node){
        void *new_data = malloc(item_size);
        if(new_data){
            memcpy(new_data, data, item_size);

            new_node->data = new_data;
            new_node->priority = priority;
            new_node->next = NULL;

            return new_node;
        }
        free(new_node);
    }
}

static void free_node(Node *node){
    if(node){
        free(node->data);
        free(node);
    }
}

