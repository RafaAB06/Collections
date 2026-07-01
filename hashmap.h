#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct{
    char *key;
    void *value;
}Entry;

typedef struct __d_node{
    Entry entry;
    struct __d_node *next;
}__D_NODE;

typedef struct{
    size_t item_size;
    size_t size;
    size_t capacity;
    size_t step_size;
    __D_NODE **entries;
}HashMap;

HashMap * hm_new(size_t item_size, size_t step_size);
HashMap * hm_new_from_arrays(size_t item_size, size_t step_size, size_t length, char **key_array, void *value_array);
int hm_set(HashMap *map, char *key, void *value);
int hm_get(HashMap *map, char *key, void *dest);
int hm_remove_entry(HashMap *map, char *key);
int hm_contains_key(HashMap *map, char *key);
int hm_contains_value(HashMap *map, void *value);
int hm_contains_entry(HashMap *map, char *key, void *value);
void *hm_key_set(HashMap *map, int *length);
void *hm_value_set(HashMap *map, int *length);
Entry * hm_entry_set(HashMap *map, int *length);
HashMap * hm_clone(HashMap *map);
void hm_shrink(HashMap *map);
int hm_get_size(HashMap *map);
int hm_get_capacity(HashMap *map);
int hm_is_empty(HashMap *map);
void hm_clear(HashMap *map);
void hm_destroy(HashMap **map);
void hm_print(HashMap *map, char *(value_to_string)(void *));

#endif