#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "linked_list.h"

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&

typedef __LL_Node Node;

static Node * create_node(void *data, size_t item_size);
static void free_node(Node *node);

LinkedList * ll_new(size_t item_size){
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));

    if(not list){
        printf("Out of Memory Exception at LinkedList::ll_new()!\n");
        return NULL;
    }

    list->item_size = item_size;
    list->size = 0;
    list->first = NULL;

    return list;
}

LinkedList * ll_new_from_array(void *array, int length, size_t item_size){
    LinkedList *list = new_linked_list(item_size);

    if(not list){
        printf("Out of Memory Exception at LinkedList::ll_new()!\n");
        return NULL;
    }

    int i;
    for(i = length - 1; i >= 0; i--){
        if(not insert_beginning(list, (char *)array + i * item_size)){
            printf("Out of Memory Exception at LinkedList::ll_new()!\n");
            ll_destroy(&list);
            return NULL;
        }
    }

    return list;
}

int ll_is_empty(LinkedList *list){
    return not list->first;
}

int ll_set(LinkedList *list, void* data, int index){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_change_at_index()!\n");
        return ERROR;
    }
    if(is_empty(list)){
        return ERROR;
    }
    if (index < 0 || index >= list->size) {
        printf("Index out of Bounds Exception at LinkedList::change_at_index()!\n");
        return ERROR;
    }

    Node *current = list->first;
    int i = 0, item_size = list->item_size;
    while(current and i <= index){
        if(i == index){
            memcpy(current->data, data, item_size);
            return SUCCESS;
        }

        current = current->next;
        i++;
    }
    return ERROR;
}

int ll_insert_beginning(LinkedList *list, void *data){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    size_t item_size = list->item_size;
    Node *new = create_node(data, item_size);

    if(not new){
        printf("Out of Memory Exception at LinkedList::insert_beginning()!\n");
        return ERROR;
    }

    Node *first_node = list->first;
    new->next = first_node;

    list->first = new;
    list->size++;

    return SUCCESS;
}

int ll_insert_end(LinkedList *list, void *data){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_end()!\n");
        return ERROR;
    }
    size_t item_size = list->item_size;
    Node *new = create_node(data, item_size);

    if(not new){
        printf("Out of Memory Exception at LinkedList::insert_end()!\n");
        return ERROR;
    }

    if(is_empty(list)){
        list->first = new;
    }else{
        Node *last = list->last;

        last->next = new;
    }
    list->last = new;
    list->size++;
    return SUCCESS;
}

int ll_insert_at_index(LinkedList *list, void *data, int index) {
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_at_index()!\n");
        return ERROR;
    }
    if (index < 0 || index > list->size) {
        printf("Index out of Bounds Exception at LinkedList::insert()!\n");
        return ERROR;
    }

    if (index == 0) {
        return insert_beginning(list, data);
    }
    if(index == list->size){
        return insert_end(list, data);
    }

    size_t item_size = list->item_size;
    Node *new_node = create_node(data, item_size);
    if (new_node == NULL) {
        printf("Out of Memory Exception at LinkedList::insert()!\n");
        return ERROR;
    }

    Node *previous = NULL;
    Node *current = list->first;
    int i = 0;

    while (current != NULL && i < index) {
        previous = current;
        current = current->next;
        i++;
    }

    previous->next = new_node;
    new_node->next = current;
    list->size++;

    return SUCCESS;
}

int ll_remove_beginning(LinkedList * list, void* dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_remove_beginning()!\n");
        return ERROR;
    }
    if(is_empty(list)){
        return ERROR;
    }
    Node *first = list->first;
    Node *next = first->next;
    list->first = next;

    memcpy(dest, first->data, list->item_size);
    free_node(first);

    list->size--;
    return SUCCESS;
}

int ll_remove_end(LinkedList *list, void *dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_remove_end()!\n");
        return ERROR;
    }
    if(is_empty(list)){
        return ERROR;
    }
    Node *previous = NULL;
    Node *last = list->first;

    while (last->next){
        previous = last;
        last = last->next;
    }
    if(previous){
        previous->next = NULL;
        list->last = previous;
    }
    memcpy(dest, last->data, list->item_size);
    free_node(last);

    list->size--;
    return SUCCESS;
}

int ll_remove_at_index(LinkedList *list, void *dest, int index) {
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_remove_at_index()!\n");
        return ERROR;
    }
    if (is_empty(list)) {
        return ERROR;
    }
    if (index < 0 || index >= list->size) {
        printf("Index out of Bounds Exception at LinkedList::remove_at_index()!\n");
        return ERROR;
    }
    if (index == 0) {
        return remove_beginning(list, dest);
    }

    Node *previous = NULL;
    Node *current = list->first;
    int i = 0;

    while (current != NULL && i < index) {
        previous = current;
        current = current->next;
        i++;
    }

    if (current != NULL) {
        previous->next = current->next;
        memcpy(dest, current->data, list->item_size);
        free_node(current);
        list->size--;
        return SUCCESS;
    }

    return ERROR;
}

int ll_peek_first(LinkedList *list, void *dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_peek_first()!\n");
        return ERROR;
    }
    if(is_empty(list)){
        return ERROR;
    }
    Node *first = list->first;
    memcpy(dest, first->data, list->item_size);

    return SUCCESS;
}

int ll_peek_last(LinkedList *list, void *dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_peek_last()!\n");
        return ERROR;
    }
    if(is_empty(list)){
        return ERROR;
    }
    Node *last = list->last;

    memcpy(dest, last->data, list->item_size);
    return SUCCESS;
}

int ll_peek_at_index(LinkedList *list, void *dest, int index) {
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_peek_at_index()!\n");
        return ERROR;
    }
    if (is_empty(list)) {
        return ERROR;
    }
    if (index < 0 || index >= list->size) {
        printf("Index out of Bounds Exception at LinkedList::peek_at_index()!\n");
        return ERROR;
    }
    if (index == 0) {
        return peek_first(list, dest);
    }

    Node *current = list->first;
    int i = 0;

    while (current && i < index) {
        current = current->next;
        i++;
    }

    if (current) {
        memcpy(dest, current->data, list->item_size);
        return SUCCESS;
    }

    return ERROR;
}

int ll_locate(LinkedList *list, const void *data){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_locate()!\n");
        return -1;
    }
    if (is_empty(list)) {
        return -1; 
    }
    Node *current = list->first;
    int index = 0;

    while (current) {
        if (memcmp(current->data, data, list->item_size) == 0) {
            return index;
        }
        current = current->next;
        index++; 
    }
    return -1;
}

void ll_reverse(LinkedList *list){
    if(not list){
        printf("Null Reference Exception at LinkedList::ll_reverse()!\n");
        return;
    }
    if(not is_empty(list)){
        Node *previous = NULL;
        Node *current = list->first;
        Node *next = next = current->next;

        current->next = NULL;
        while (next)
        {
            previous = current;
            current = next;
            next = next->next;
            current->next = previous;
        }

        list->first = current;
    }
}

LinkedList * ll_concat(LinkedList *list_1, LinkedList *list_2){
    if(not list_1 or not list_2){
        printf("Null Reference Exception at LinkedList::ll_concat()!\n");
        return NULL;
    }
    list_1 = clone(list_1);
    list_2 = clone(list_2);
    LinkedList *list_3 = new_linked_list(list_1->item_size);
    if(not list_1 or not list_2 or not list_3){
        printf("Out of Memory Exception at LinkedList::ll_concat()!\n");
        ll_destroy(&list_1);
        ll_destroy(&list_2);
        ll_destroy(&list_3);
        return NULL;
    }

    Node *last_first = list_1->last;
    Node *first_last = list_2->first;
    
    if(last_first){
        list_3->first = list_1->first;
        last_first->next = first_last;
    }else{
        list_3->first = list_2->first;
    }
    
    if(list_2->last){
        list_3->last = list_2->last;
    }else{
        list_3->last = list_1->last;
    }
    
    list_3->size = list_1->size + list_2->size;
    
    ll_destroy(&list_1);
    ll_destroy(&list_2);
    return list_3;
}

LinkedList * ll_clone(LinkedList *list){
    if(not list){
        printf("Null Reference Exception at LinkedList::clone()!\n");
        return NULL;
    }
    LinkedList *new_list = new_linked_list(list->item_size);

    if(is_empty(list)){
        return new_list;
    }
    Node *current = list->first;
    size_t item_size = list->item_size;
    while (current)
    {
        void *new_data = malloc(item_size);
        if(not new_data){
            printf("Out of Memory Exception at LinkedList::clone()!\n");
            ll_destroy(&new_list);
            return NULL;
        }
        memcpy(new_data, current->data, item_size);

        insert_end(new_list, new_data);
        free(new_data);

        current = current->next;
    }
    return new_list;
}

void * ll_to_array(LinkedList *list, int *length){
    if(not list or not length){
        printf("Null Reference Exception at LinkedList::ll_to_array()!\n");
        return NULL;
    }
    if(is_empty(list)){
        *length = 0;
        return NULL;
    }
    char *array = (char *) malloc(list->size * list->item_size);//para avançar de byte em byte
    if(not array){
        printf("Out of Memory Exception at LinkedList::to_array()!\n");
        return NULL;
    }
    Node *current = list->first;
    int i = 0;
    while(current){
        int array_pos = i*list->item_size;
        memcpy(array + array_pos, current->data, list->item_size);

        current = current->next;
        i++;
    }
    *length = i;
    return (void *)array;
}

void ll_clear(LinkedList *list){
    if(not list){
        printf("Null Reference Exception at LinkedList::ll_clear()!\n");
        return;
    }
    Node *previous = NULL;
    Node *current = list->first;

    while (current)
    {
        previous = current;
        current = current->next;

        free_node(previous);
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
}

void ll_destroy(LinkedList **list){
    if(list and *list){
        clear(list);
        free((*list)->first);
        free((*list)->last);
        free(*list);
        *list = NULL;
    }
}

int ll_get_size(LinkedList *list){
    return list ? list->size: 0;
}

void ll_print(LinkedList *list, char *(*to_string)(void *)){
    if(not list){
        printf("Null Reference Exception at LinkedList::ll_print()!\n");
        return;
    }
    printf("Linked List:\n");
    if(is_empty(list)){
        printf("Empty\n");
        return;
    }
    Node *current = list->first;
    int i = 0;
    while(current){
        char *str = to_string(current->data);
        printf("%d - %s\n", i, str);
        free(str);

        current = current->next;
        i++;
    }
}

static Node * create_node(void *data, size_t item_size){
    Node * new = (Node *) malloc(sizeof(Node));

    if(new){
        new->data = malloc(item_size);
        new->next = NULL;
        memcpy(new->data, data, item_size);
    }
    return new;
}

static void free_node(Node *node){
    free(node->data);
    free(node);
}
