#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "array_list.h"

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&
#define VOID_SIZE sizeof(void *)

static void* create_item(size_t item_size, void *data);
static void free_item(void *item);

ArrayList * al_new(size_t item_size, size_t step_size){
    if(step_size <= 0){
        printf("Invalid Value Exception at ArrayList::al_new()!\n");
        return NULL;
    }
    ArrayList *new_al = (ArrayList *)malloc(sizeof(ArrayList));

    new_al->item_size = item_size;
    new_al->step_size = step_size;
    new_al->capacity = step_size;
    new_al->length = 0;
    new_al->array = malloc(VOID_SIZE * step_size);

    return new_al;
}
ArrayList * al_new_from_array(void *array, size_t length, size_t item_size, size_t step_size){
    ArrayList *new_al = (ArrayList *)malloc(sizeof(ArrayList));

    size_t min_capacity = ((length + step_size - 1) / step_size) * step_size;
    new_al->item_size = item_size;
    new_al->step_size = step_size;
    new_al->capacity = min_capacity;
    new_al->length = length;
    new_al->array = malloc(VOID_SIZE * min_capacity);

    int i;
    for(i = 0; i<length; i++){
        void *new_item = malloc(item_size);
        memcpy(new_item, (char *)array + i * item_size, item_size);

        new_al->array[i] = new_item;
    }
    return new_al;
}

int al_set(ArrayList *list, void *data, int index){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if(index < 0 or index >= list->length){
        printf("Index out of Bounds Exception at ArrayList::al_insert_at_index()!\n");
        return ERROR;
    }
    void *new_item = create_item(list->item_size, data);  
    if (not new_item) {
        printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
        return ERROR;
    }
    free_item(list->array[index]);
    list->array[index] = new_item;
    return SUCCESS;
}

int al_insert_beggining(ArrayList *list, void *data) {
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if (list->length >= list->capacity) {
        void **new_array = realloc(list->array, (list->capacity + list->step_size) * VOID_SIZE);
        if (not new_array) {
            printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
            return ERROR;
        }
        list->array = new_array;
        list->capacity += list->step_size;
    }

    void *new_item = create_item(list->item_size, data);  
    if (not new_item) {
        printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
        return ERROR;
    }

    int i;
    for (i = list->length; i > 0; i--) {
        list->array[i] = list->array[i - 1]; 
    }

    list->array[0] = new_item;
    list->length++;
    return SUCCESS;
}

int al_insert_end(ArrayList *list, void *data){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if (list->length >= list->capacity) {
        void **new_array = realloc(list->array, (list->capacity + list->step_size) * VOID_SIZE);
        if (not new_array) {
            printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
            return ERROR;
        }
        list->array = new_array;
        list->capacity += list->step_size;
    }
    void *new_item = create_item(list->item_size, data);  
    if (not new_item) {
        printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
        return ERROR;
    }

    list->array[list->length] = new_item;
    list->length++;
    return SUCCESS;
}

int al_insert_at_index(ArrayList *list, void *data, int index){
    if(not list or not data){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if(index < 0 or index > list->length){
        printf("Index out of Bounds Exception at ArrayList::al_insert_at_index()!\n");
        return ERROR;
    }
    if (list->length >= list->capacity) {
        void **new_array = realloc(list->array, (list->capacity + list->step_size) * VOID_SIZE);
        if (not new_array) {
            printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
            return ERROR;
        }
        list->array = new_array;
        list->capacity += list->step_size;
    }
    void *new_item = create_item(list->item_size, data);  
    if (not new_item) {
        printf("Out of Memory Exception at ArrayList::al_insert_beginning()!\n");
        return ERROR;
    }
    int i;
    for (i = list->length; i > index; i--) {
        list->array[i] = list->array[i - 1]; 
    }
    list->array[index] = new_item;
    list->length++;
    return SUCCESS;
}

int al_remove_first(ArrayList *list, void *dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if(al_is_empty(list)){
        return ERROR;
    }
    return al_remove_at_index(list, dest, 0);
}

int al_remove_last(ArrayList *list, void *dest){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if(al_is_empty(list)){
        return ERROR;
    }
    void *last = list->array[list->length-1];
    memcpy(dest, last, list->item_size);
    free_item(last);
    list->length--;

    return SUCCESS;
}

int al_remove_at_index(ArrayList *list, void *dest, int index){
    if(not list or not dest){
        printf("Null Reference Exception at LinkedList::ll_insert_beginning()!\n");
        return ERROR;
    }
    if(index < 0 or index >= list->length){
        printf("Index out of Bounds Exception at ArrayList::al_peek_at_index()!\n");
        return ERROR;
    }
    if(al_is_empty(list)){
        return ERROR;
    }

    void *target = list->array[index];
    memcpy(dest, target, list->item_size);
    free_item(target);
    list->length--;

    int i;
    for(i = index; i<list->length; i++){
        void *next = list->array[i + 1];
        list->array[i] = next;
    }
}

int al_peek_first(ArrayList *list, void *dest){
    if(al_is_empty(list)){
        return ERROR;
    }
    void *last = list->array[0];
    memcpy(dest, last, list->item_size);
    return SUCCESS;
}

int al_peek_last(ArrayList *list, void *dest){
    if(al_is_empty(list)){
        return ERROR;
    }
    void *last = list->array[list->length - 1];
    memcpy(dest, last, list->item_size);
    return SUCCESS;
}

int al_peek_at_index(ArrayList *list, void *dest, int index){
    if(not list or not dest){
        printf("Null Reference Exception at ArrayList::al_peek_first()!\n");
        return ERROR;
    }
    if(index < 0 or index >= list->length){
        printf("Index out of Bounds Exception at ArrayList::al_peek_at_index()!\n");
        return ERROR;
    }
    if(al_is_empty(list)){
        return ERROR;
    }

    void *target_item = list->array[index];
    memcpy(dest, target_item, list->item_size);
    return SUCCESS;
}

int al_compact(ArrayList *list){
    if(not list){
        printf("Null Reference Exception at ArrayList::al_print()!\n");
        return ERROR;
    }
    size_t length = list->length;
    size_t capacity = list->capacity;
    size_t step_size = list->step_size;
    size_t empty_space = capacity - length;
    if(empty_space >= step_size){
        size_t steps = empty_space/step_size;
        size_t new_capacity = list->capacity - steps*list->step_size;

        void **new_array = realloc(list->array, (new_capacity) * VOID_SIZE);
        if(not list->array)
            return ERROR;
        
        list->array = new_array;
        list->capacity = new_capacity;
    }
    return SUCCESS;
}

void al_reverse(ArrayList *list){
    if(not list){
        printf("Null Reference Exception at ArrayList::reverse()!\n");
        return;
    }
    size_t half = list->length/2;
    int i;
    for(i = 0; i<half; i++){
        size_t last_index = list->length - i - 1;
        void *temp = list->array[i];

        list->array[i] = list->array[last_index];
        list->array[last_index] = temp;
    }
}

int al_locate(ArrayList *list, void *data){
    if(not list or not data){
        printf("Null Reference Exception ar ArrayList::al_locate()!\n");
    }else{
        size_t len = list->length;
        int i;
        for(i = 0; i<len; i++){
            void *current = list->array[i];

            if(not memcmp(current, data, list->item_size)){
                return i;
            }
        }
    }

    return -1;
}

void * al_to_array(ArrayList *list, size_t *length){
    if(not list or not length){
        printf("Null Reference Exception at ArrayList::al_to_array()!\n");
        return NULL;
    }
    if(al_is_empty(list)){
        *length = 0;
        return NULL;
    }
    size_t len = list->length;
    size_t item_size = list->item_size;

    void *array = malloc(len * item_size);
    int i;
    for(i = 0; i<len; i++){
        memcpy((char *)array + i * item_size, list->array[i], item_size);
    }
    *length = len;
    return array;
}

ArrayList * al_clone(ArrayList *list){
    ArrayList *clone = (ArrayList *)malloc(sizeof(ArrayList));

    if(not clone){
        printf("Out of Memory Exception at ArrayList::al_clone()!\n");
        return NULL;
    }
    void **array = malloc(list->capacity * VOID_SIZE);
    if(not array){
        printf("Out of Memory Exception at ArrayList::al_clone()!\n");
        return NULL;
    }
    clone->array = array;
    int i;
    for(i = 0; i<list->length; i++){
        void *new_item = malloc(list->item_size);
        memcpy(new_item, list->array[i], list->item_size);

        clone->array[i] = new_item;
    }

    clone->capacity = list->capacity;
    clone->item_size = list->item_size;
    clone->length = list->length;
    clone->step_size = list->step_size;

    return clone;
}

ArrayList * al_concat(ArrayList *list_1, ArrayList *list_2, size_t step_size){
    if(not list_1 or not list_2){
        printf("Null Reference Exception at ArrayList::al_concat()!\n");
        return NULL;
    }
    if(list_1->item_size != list_2->item_size){
        printf("Invalid Value Exception at ArrayList::al_concat()\n");
        return NULL;
    }
    ArrayList *new_list = malloc(sizeof(ArrayList));
    if(not new_list){
        printf("Out of Memory Exception at ArrayList::al_clone()!\n");
        return NULL;
    }

    size_t new_length = list_1->length + list_2->length;
    size_t new_capacity = ((new_length + step_size - 1) / step_size)*step_size;

    new_list->array = malloc(new_capacity * VOID_SIZE);
    if(not new_list->array){
        printf("Out of Memory Exception at ArrayList::al_clone()!\n");
        free(new_list);
        return NULL;
    }
    new_list->capacity = new_capacity;
    new_list->item_size = list_1->item_size;
    new_list->step_size = step_size;

    int i;
    for(i = 0; i<list_1->length; i++){
        void *new_item = malloc(list_1->item_size);
        if(not new_item){
            printf("Out of Memory Exception at ArrayList::al_clone()!\n");
            al_destroy(new_list);
            return NULL;
        }
        memcpy(new_item, list_1->array[i], list_1->item_size); 

        new_list->array[i] = new_item;
        new_list->length++;
    }
    int j;
    for(j = 0; j<list_2->length; j++, i++){
        void *new_item = malloc(list_2->item_size);
        if(not new_item){
            printf("Out of Memory Exception at ArrayList::al_clone()!\n");
            al_destroy(new_list);
            return NULL;
        }
        memcpy(new_item, list_2->array[j], list_1->item_size); 

        new_list->array[i] = new_item;
        new_list->length++;
    }
    return new_list;
}

int al_is_empty(ArrayList *list){
    return list and list->length == 0;
}

int al_get_size(ArrayList *list){
    return list ? list->length : 0;
}

int al_get_capacity(ArrayList *list){
    return list ? list->capacity : 0;
}

int al_get_step_size(ArrayList *list){
    return list ? list->step_size : 0;
}

void al_set_step_size(ArrayList *list, size_t step_size){
    if(list){
        list->step_size = step_size;
    }
}

int al_ensure_capacity(ArrayList *list, size_t min_size) {
    if (not list) {
        printf("Null Reference Exception at ArrayList::ensure_capacity()!\n");
        return ERROR;
    }

    if (min_size > list->capacity) {
        size_t min_steps = (min_size + list->step_size - 1) / list->step_size;
        size_t target_capacity = min_steps * list->step_size;

        void **new_array = realloc(list->array, target_capacity * VOID_SIZE);
        if (!new_array) {
            printf("Out of Memory Exception at ArrayList::ensure_capacity()!\n");
            return ERROR;
        }

        list->array = new_array;
        list->capacity = target_capacity;
    }

    if (list->length > list->capacity) {
        list->length = list->capacity;
    }

    return SUCCESS;
}

void al_clear(ArrayList *list){
    if (!list) {
        printf("Null Reference Exception at ArrayList::destroy()!\n");
    }
    int i;
    for(i = 0; i<list->length; i++){
        free(list->array[i]);
    }
    list->array = realloc(list->array, list->step_size * VOID_SIZE);

    list->length = 0;
    list->capacity = list->step_size;
}

void al_destroy(ArrayList **list){
    if(list and *list){
        int i;
        for(i = 0; i<(*list)->length; i++){
            free((*list)->array[i]);
        }
        free((*list)->array);
        free(*list);
        *list = NULL;
    }
}

static void quick_sort(void **array, int min, int max, int(*comparator)(void *, void *), int ascending, size_t item_size)
{   
    if (max > min)
    {
        void *pivot = array[max];
        int i, j;
        for (i = 0, j = -1; i < max; i++)
        {
            if ((comparator(array[i], pivot) and ascending) or (not comparator(array[i], pivot) and not ascending))
            {
                j++;
                void *temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        j++;
        void *temp = array[i];//poderia ser melhor, mas eu nao tenho como explicar o porquê em uma linha de comentário..
        array[i] = array[j];
        array[j] = temp;

        quick_sort(array, min, j - 1, comparator, ascending, item_size);
        quick_sort(array, j + 1, max, comparator, ascending, item_size);
    }
}

void al_sort(ArrayList *list, int(*comparator)(void *, void *), int ascending){
    if(not list or not comparator){
        printf("Null Reference Exception at ArrayList::al_sort()!\n");
        return;
    }
    
    quick_sort(list->array, 0, list->length - 1, comparator, ascending, list->item_size);
}

void al_print(ArrayList *list, char *(*to_string)(void *)){
    printf("ArrayList:\n");
    if(not list){
        printf("NULL\n");
        return;
    }
    printf("Size -> %d, Capacity -> %d\n", list->length, list->capacity);
    if(al_is_empty(list)){
        printf("Empty\n");
        return;
    }
    int i;
    for(i = 0; i<list->length; i++){
        char *str = to_string(list->array[i]);

        printf("%d - %s\n", i, str);
        free(str);
    }
}

static void* create_item(size_t item_size, void *data){
    void *new_item = malloc(item_size);

    memcpy(new_item, data, item_size);

    return new_item;
}

static void free_item(void *item){
    free(item);
}