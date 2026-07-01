#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "hashmap.h"

typedef struct{
    int int_item;
    float float_item;
}Struct;

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&

#define DEFAULT_STEP 16u
#define FAC 1f/4

typedef __D_NODE Node;

static size_t minimum_capacity(size_t items, size_t step) {
    size_t capacity = step;
    while (capacity <= 2 * items) {
        capacity *= 2;
    }
    return capacity;
}

static unsigned int hash_function(char *str){
    unsigned int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

static Node * create_node(char *key, void *value, size_t item_size){
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node){;
        char *new_key = (char *)malloc(strlen(key) + 1);
        if(new_key){
            void *new_value = calloc(1, item_size);
            if(new_value){
                strcpy(new_key, key);
                memcpy(new_value, value, item_size);

                Entry new_entry = {new_key, new_value};

                new_node->entry = new_entry;
                new_node->next = NULL;

                return new_node;
            }
            free(new_key);
        }
        free(new_node);
    }
}

static int free_node(Node *node){
    if(node){
        Entry entry = node->entry;
        free(entry.key);
        free(entry.value);
        free(node);
    }
}

HashMap * hm_new(size_t item_size, size_t step_size){
    if(item_size <= 0){
        printf("Invalid Value Exception at HashMap::hm_new()!\n");
        return NULL;
    }
    HashMap *new = (HashMap *)malloc(sizeof(HashMap));
    if(not new){
        printf("Out of Memory Exception at HashMap::hm_new()!\n");
        return NULL;
    }
    if(step_size <= 0) step_size = DEFAULT_STEP;
    Node **new_entries = (Node **)calloc(step_size, sizeof(Node *));
    if(not new_entries){
        printf("Out of Memory Exception at HashMap::hm_new()!\n");
        free(new);
        return NULL;
    }

    new->item_size = item_size;
    new->entries = new_entries;
    new->size = 0;
    new->step_size = step_size;
    new->capacity = step_size;

    return new;
}

HashMap * hm_new_from_arrays(size_t item_size, size_t step_size, size_t length, char **key_array, void *value_array){
    if(item_size <= 0 or length <= 0){
        printf("Invalid Value Exception at HashMap::hm_new_from_arrays()!\n");
        return NULL;
    }
    HashMap *new = (HashMap *)malloc(sizeof(HashMap));
    if(not new){
        printf("Out of Memory Exception at HashMap::hm_new_from_arrays()!\n");
        return NULL;
    }
    if(step_size <= 0) step_size = DEFAULT_STEP;

    size_t target_capacity = minimum_capacity(length, step_size);

    Node **new_entries = (Node **)calloc(target_capacity, sizeof(Node *));
    if(not new_entries){
        printf("Out of Memory Exception at HashMap::hm_new_from_arrays()!\n");
        free(new);
        return NULL;
    }
    int i;

    new->size = 0;
    new->item_size = item_size;
    new->entries = new_entries;
    new->step_size = step_size;
    new->capacity = target_capacity;

    for(i = 0; i<length; i++){
        char *current_key = key_array[i];
        void *current_value = value_array + i * item_size;
        Node *new_node = create_node(current_key, current_value, item_size);
        if(not new_node){
            printf("Out of Memory Exception at HashMap::hm_new_from_arrays()!\n");
            hm_destroy(&new);
            return NULL;
        }
        unsigned int hash = hash_function(current_key)%new->capacity;

        Node *current_bucket = new_entries[hash];
        if(not current_bucket){
            new_entries[hash] = new_node;
        }
        else{
            while (current_bucket->next)
            {
                current_bucket = current_bucket->next;
            }
            current_bucket->next = new_node;
        }
        new->size++;
    }

    return new;
}

static int resize_map(HashMap *map){
    size_t old_capacity = map->capacity;
    size_t new_capacity = old_capacity * 2;
    map->capacity = new_capacity;

    Node **new_entries = (Node **)calloc(new_capacity, sizeof(Node *));
    if(not new_entries){
        return ERROR;
    }
    int i;

    for(i = 0; i<old_capacity; i++){
        Node *current = map->entries[i];

        if(current){
            while(current){
                unsigned int new_hash = hash_function(current->entry.key)%map->capacity;

                Node *new_current = new_entries[new_hash];
                if(not new_current){
                    new_entries[new_hash] = current;
                }else{
                    while (new_current->next)
                    {
                        new_current = new_current->next;
                    }
                    new_current->next = current;
                }
                Node* temp = current->next;
                current->next = NULL;
                current = temp;
            }
        }
    }
    free(map->entries);
    map->entries = new_entries;

    return SUCCESS;
}

int hm_set(HashMap *map, char *key, void *value){
    if(not map or not key or not value){
        printf("Null Reference Exception at HashMap::hm_put()!\n");
        return ERROR;
    }
    Node *new_node = create_node(key, value, map->item_size);
    if(not new_node){
        printf("Out of Memory Exception at HashMap::hm_put()!\n");
        return ERROR;
    }
    size_t capacity_factor = map->capacity * FAC;
    if(capacity_factor <= map->size){
        if(not resize_map(map)){
            printf("Out of Memory Exception at HashMap::hm_put()!\n");
            return ERROR;
        }
    }

    unsigned int hash_code = hash_function(key)%map->capacity;

    Node *previous = NULL;
    Node *node_at_position = map->entries[hash_code];
    if(not node_at_position){
        map->entries[hash_code] = new_node;
    }else{
        int override = 0;
        while (node_at_position)
        {
            if(not strcmp(new_node->entry.key, node_at_position->entry.key)){
                memcpy(node_at_position->entry.value, new_node->entry.value, map->item_size);
                free_node(new_node);

                override = 1;
                break;
            }
            previous = node_at_position;
            node_at_position = node_at_position->next;
        }
        if(not override){
            previous->next = new_node;
        }
    }
    map->size++;

    return SUCCESS;
}

int hm_get(HashMap *map, char *key, void *dest){
    if(not map or not key or not dest){
        printf("Null Reference Exception at HashMap::hm_get()!\n");
        return ERROR;
    }
    if(hm_is_empty(map)){
        return ERROR;
    }
    unsigned int hash = hash_function(key)%map->capacity;

    Node *node = map->entries[hash];
    if(node){
        while(node){
            if(not strcmp(key, node->entry.key)){
                memcpy(dest, node->entry.value, map->item_size);

                return SUCCESS;
            }
            node = node->next;
        }
    }
    return ERROR;
}

int hm_remove_entry(HashMap *map, char *key){
    if(not map or not key){
        printf("Null Reference Exception at HashMap::hm_remove_entry");
        return ERROR;
    }
    if(hm_is_empty(map)){
        return ERROR;
    }
    unsigned int hash = hash_function(key)%map->capacity;

    Node *previous = NULL;
    Node *current = map->entries[hash];
    if(current){
        while (current)
        {
            if(not strcmp(current->entry.value, key)){
                if(previous){
                    previous->next = current->next;
                }else map->entries[hash] = current->next;

                map->size--;
                free_node(current);

                return SUCCESS;
            }  
            previous = current;
            current = current->next; 
        }
    }
    return ERROR;
}

static int contains_key(HashMap *map, char *key){
    unsigned int hash = hash_function(key)%map->capacity;

    Node *current = map->entries[hash];
    if(current){
        while(current){
            if(not strcmp(current->entry.key, key)){
                return 1;
            }

            current = current->next;
        }
    }
    return 0;
}

static int contains_value(HashMap *map, void *value){
    Node **entries = map->entries;
    int i;
    for(i = 0; i<map->capacity; i++){
        Node *current = entries[i];
        if(current){
            while (current)
            {
                if(not memcmp(value, current->entry.value, map->item_size)){
                    return 1;
                }
                current = current->next;
            }
        }
    }
    return 0;
}

int hm_contains_key(HashMap *map, char *key){
    if(not map or not key){
        printf("Null Reference Exception at HashMap::hm_contains_key()!\n");
    }

    return contains_key(map, key);
}

static int contains_value_key(HashMap *map, void *key, void *value){
    Node **entries = map->entries;
    int i;
    for(i = 0; i<map->capacity; i++){
        Node *current = entries[i];
        if(current){
            while (current)
            {
                int has_key = not strcmp(key, current->entry.key);
                int has_value = not memcmp(value, current->entry.value, map->item_size);
                if(has_key and has_value){
                    return 1;
                }
                current = current->next;
            }
        }
    }
    return 0;
}

int hm_contains_value(HashMap *map, void *value){
    if(not map or not value){
        printf("Null Reference Exception at HashMap::hm_contains_key()!\n");
    }
    return contains_value(map, value);
}

int hm_contains_entry(HashMap *map, char *key, void *value){
    if(not map or not key or not value){
        printf("Null Reference Exception at HashMap::hm_contains_key()!\n");
    }
    return contains_value_key(map, key, value);
}

static void destroy_key_array(void *ptr){
    char **array = (char **)ptr;
    int i;
    for(i = 0; array[i]; i++){
        free(array[i]);
    }
    free(array);
}

static void array_set(HashMap *map, int is_key, void *array){
    int i, j = 0;
    for(i = 0; i<map->capacity; i++){
        Node *current = map->entries[i];

        if(current){
            while (current)
            {
                if(is_key){
                    char *current_key = current->entry.key;
                    char *new_str = (char *)malloc(strlen(current_key) + 1);
                    if(not new_str){
                        destroy_key_array(array);
                    }
                    strcpy(new_str, current_key);
                    void *current_pos = array + j * sizeof(char *);
                    current_pos = new_str;
                }else{
                    void *current_value = current->entry.value;
                    void *current_pos = array + j * map->item_size;
                    memcpy(current_pos, current_value, map->item_size);
                }
                j++;
                current = current->next;
            }
        }
    }
}

void *hm_key_set(HashMap *map, int *length){
    if(not map or not length){
        printf("Null Refenrence Exception at HashMap::hm_key_set()!\n");
        return NULL;
    } 
    if(hm_is_empty(map)){
        return NULL;
    }
    char **array = (char **)calloc(map->size, sizeof(char *));
    if(not array){
        printf("Out of Memory Exception at HashMap::hm_key_set()!\n");
        return NULL;
    }
    array_set(map, 1, array);

    *length = map->size;
    return array;
}

void *hm_value_set(HashMap *map, int *length){
    if(not map or not length){
        printf("Null Refenrence Exception at HashMap::hm_key_set()!\n");
        return NULL;
    } 
    if(hm_is_empty(map)){
        return NULL;
    }
    void *array = calloc(map->size, map->item_size);
    if(not array){
        printf("Out of Memory Exception at HashMap::hm_key_set()!\n");
        return NULL;
    }
    array_set(map, 0, array);
    *length = map->size;
    return array;
}

static void destroy_entry_array(Entry *array, int length){
    if(length > 0){
        int i;
        for(i = 0; i<length; i++){
            Entry current_entry = array[i];
            free(current_entry.key);
            free(current_entry.value);
        }
        free(array);
    }
}

Entry *hm_entry_set(HashMap *map, int *length){
    if(not map or not length){
        printf("Null Refenrence Exception at HashMap::hm_entry_set()!\n");
        return NULL;
    } 
    if(hm_is_empty(map)){
        return NULL;
    }
    Entry *array = malloc(map->size * sizeof(Entry));
    if(not array){
        printf("Out of Memory Exception at HashMap::hm_entry_set()!\n");
        return NULL;
    }
    int i, j = 0;
    for(i = 0; i<map->capacity; i++){
        Node *current = map->entries[i];

        while (current)
        {
            char *key = current->entry.key;
            void *value = current->entry.value;

            char *new_key = (char *)malloc(strlen(key) + 1);
            if(not new_key){
                destroy_entry_array(array, i);
                return NULL;
            }
            void *new_value = malloc(map->item_size);
            if(not new_value){
                free(new_key);
                destroy_entry_array(array, i);
                return NULL;
            }
            strcpy(new_key, key);
            memcpy(new_value, value, map->item_size);

            Entry new_entry;
            new_entry.key = new_key;
            new_entry.value = new_value;
            array[j] = new_entry;

            j++;
            current = current->next;
        }
    }
    *length = map->size;
    return array;
}

HashMap * hm_clone(HashMap *map){
    if(not map){
        printf("Null Reference Exception at HashMap::hm_clone()!\n");
        return NULL;
    }
    HashMap *clone = (HashMap *)malloc(sizeof(HashMap));
    if(not clone){
        printf("Out of Memory Exception at HashMap::hm_clone()!\n");
        return NULL;
    }
    Node **new_entries = (Node **)calloc(map->capacity, sizeof(Node *));
    if(not new_entries){
        printf("Out of Memory Exception at HashMap::hm_clone()!\n");
        free(clone);
        return NULL;
    }

    clone->capacity = map->capacity;
    clone->entries = new_entries;
    int i;
    for(i = 0; i< map->capacity; i++){
        Node *current_bucket = map->entries[i];

        while (current_bucket)
        {
            Node *new_node = create_node(current_bucket->entry.key, current_bucket->entry.value, map->item_size);
            if(not new_node){
                printf("Out of Memory Exception at HashMap::hm_clone()!\n");
                hm_destroy(&clone);
                return NULL;
            }

            Node *new_bucket = clone->entries[i];
            if(not new_bucket) clone->entries[i] = new_node;
            else{
                while(new_bucket->next){
                    new_bucket = new_bucket->next;
                }
                new_bucket->next = new_node;
            }
            current_bucket = current_bucket->next;
        }
    }
    clone->size = map->size;
    clone->item_size = map->item_size;
    clone->step_size = map->step_size;

    return clone;
}

void hm_shrink(HashMap *map){
    if(map){
        size_t old_capacity = map->capacity;
        size_t new_capacity = minimum_capacity(map->size, map->step_size);

        if(old_capacity == new_capacity) return;

        Node **new_entries = (Node **)calloc(new_capacity, sizeof(Node *));
        if(not new_entries){
            printf("Out of Memory Exception at HashMap::hm_shrink()!\n");
            return;
        }
        int i;
        for(i = 0; i<old_capacity; i++){
            Node *old_bucket = map->entries[i];
            while (old_bucket)
            {
                char *key = old_bucket->entry.key;
                unsigned int new_hash = hash_function(key)%new_capacity;

                Node *new_bucket = new_entries[new_hash];
                if(not new_bucket) new_entries[new_hash] = old_bucket;
                else{
                    while(new_bucket->next){
                        new_bucket = new_bucket->next;
                    }
                    new_bucket->next = old_bucket;
                }

                old_bucket = old_bucket->next;
            }
        }

        free(map->entries);
        map->entries = new_entries;
        map->capacity = new_capacity;
    }
}

int hm_get_capacity(HashMap *map){
    return map ? map->capacity : 0;
}

int hm_get_size(HashMap *map){
    return map ? map->size : 0;
}

int hm_is_empty(HashMap *map){
    return map ? map->size == 0 : 1;
}

void hm_clear(HashMap *map){
    if(not map){
        printf("Null Reference Exception at HashMap::hm_clear()!\n");
        return;
    }
    if(map){
        size_t len = map->capacity;
        int i;
        for(i = 0; i<len; i++){
            Node *current = map->entries[i];
            
            if(current){
                while(current){
                    Node *next = current->next;
                    free_node(current);

                    current = next;
                }
            }
            map->entries[i] = NULL;
        }
    }

    map->size = 0;
}

void hm_destroy(HashMap **map){
    if(map and *map){
        hm_clear(*map);
        free((*map)->entries);
        free(*map);
        *map = NULL;
    }
}

void hm_print(HashMap *map, char *(value_to_string)(void *)){
    printf("HashMap:\n");
    if(not map){
        printf("NULL\n");
        return;
    }
    if(hm_is_empty(map)){
        printf("Empty\n");
        return;
    }
    printf("Size -> %d, Capacity -> %d\n", map->size, map->capacity);
    int i;
    for(i = 0; i<map->capacity; i++){
        Node *current = map->entries[i];
        if(current){
            while (current)
            {
                char *value_string = value_to_string(current->entry.value);
                printf("%s : %s\n", current->entry.key, value_string);
                free(value_string);

                current = current->next;
            }     
        }
    }
}