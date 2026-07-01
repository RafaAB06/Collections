#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define SUCCESS 1
#define ERROR 0
#define not !
#define or ||
#define and &&

typedef __LL_Node Node;

static void free_node(Node *node);
static Node *create_node(size_t item_size, void *data);

Stack * s_new(size_t item_size){
    Stack *new_stack = (Stack *)malloc(sizeof(Stack));
    if(not new_stack){
        printf("Out of Memory Exception at Stack::s_new()!\n");
        return NULL;
    }

    new_stack->item_size = item_size;
    new_stack->size = 0;
    new_stack->top = NULL;
    return new_stack;
}

Stack * s_new_from_array(size_t item_size, void *array, size_t length){
    Stack *new_stack = s_new(item_size);

    if(not new_stack){
        printf("Out of Memory Exception at Stack::s_new()!\n");
        return NULL;
    }
    char *byte_array = (char *)array;
    int i;
    for(i = 0; i<length; i++){
        if(not s_push(new_stack, byte_array + i * item_size)){
            printf("Out of Memory Exception at Stack::s_new()!\n");
            s_destroy(&new_stack);
            return NULL;
        }
    }
    return new_stack;
}

int s_push(Stack *stack, void *data){
    if(not stack or not data){
        printf("Null Reference Exception at Stack::s_push()!\n");
        return ERROR;
    }
    Node *new_node = create_node(stack->item_size, data);

    if(not new_node){
        printf("Out of Memory Exception at Stack::s_push()!\n");
        return ERROR;
    }
    new_node->previous = stack->top;
    stack->top = new_node;
    stack->size++;
}

int s_pop(Stack *stack, void *dest){
    if(not stack or not dest){
        printf("Null Reference Exception at Stack::s_pop()!\n");
        return ERROR;
    }
    Node *top_node = stack->top;
    if(top_node){
        stack->top = top_node->previous;
        memcpy(dest, top_node->data, stack->item_size);

        free_node(top_node);
        stack->size--;
        return SUCCESS;
    }else return ERROR;
}

int s_peek(Stack *stack, void *dest){
    if(not stack or not dest){
        printf("Null Reference Exception at Stack::s_peek()!\n");
        return ERROR;
    }
    Node *top_node = stack->top;
    if(top_node){
        memcpy(dest, top_node->data, stack->item_size);

        return SUCCESS;
    }else return ERROR;
}

int s_contains(Stack *stack, void *data){
    if(not stack){
        printf("Null Reference Exception at Stack::s_contains()!\n");
        return ERROR;
    }
    Node *current = stack->top;
    while(current){
        if(not memcmp(current->data, data, stack->item_size)){
            return 1;
        }
        current = current->previous;
    }

    return 0;
}

static int concat_recursive(Stack *new, Node *node){
    if(node){
        void *new_data = malloc(new->item_size);
        if(not new_data){
            return ERROR;
        }
        memcpy(new_data, node->data, new->item_size);

        if(not concat_recursive(new, node->previous)){
            free(new_data);
            return ERROR;
        }
        if(not s_push(new, new_data)){
            free(new_data);
            return ERROR;
        }
        free(new_data);
    }
    return SUCCESS;
}

Stack * s_concat(Stack *stack_1, Stack *stack_2){
    if(not stack_1 or not stack_2){
        printf("Null Reference Exception at Stack::s_concat()!\n");
        return NULL;
    }
    if(stack_1->item_size != stack_2->item_size){
        printf("Invalid Value Exception at Stack::s_concat()!\n");
        return NULL;
    }
    Stack *new_stack = malloc(sizeof(Stack));
    if(not new_stack){
        printf("Out of Memory Exception at Stack::s_new()!\n");
        return NULL;
    }
    new_stack->item_size = stack_1->item_size;
    new_stack->size = 0;
    new_stack->top = NULL;

    if(not concat_recursive(new_stack, stack_1->top) or not concat_recursive(new_stack, stack_2->top)){
        s_destroy(&new_stack);
    }

    return new_stack;
}

static int recursive_clone(Node *node, Stack *clone){
    if(node){
        void *new_data = malloc(clone->item_size);
        if(not new_data){
            return ERROR;
        }
        memcpy(new_data, node->data, clone->item_size);

        if(not recursive_clone(node->previous, clone)){
            free(new_data);
            return ERROR;
        }
        if(not s_push(clone, new_data)){
            free(new_data);
            return ERROR;
        }
        free(new_data);
    }
    return SUCCESS;
}

Stack * s_clone(Stack *stack){
    Stack *clone = s_new(stack->item_size);

    if(not stack or not clone){
        printf("Null Reference Exception at Stack::s_clone()!\n");
        return NULL;
    }
    if(not recursive_clone(stack->top, clone)){
        printf("Out of Memory Exception at Stack::s_clone()!\n");
        s_destroy(&clone);
    }
    return clone;
}

void s_reverse(Stack *stack){
    if(s_is_empty(stack)) return;

    Node *next = NULL;
    Node *current = stack->top;

    while (current)
    {
        Node *previous = current->previous;
        current->previous = next;
        next = current;
        current = previous;
    }
    stack->top = next;
}

int s_is_empty(Stack *stack){
    return stack and not stack->top;
}

int s_get_size(Stack *stack){
    return stack ? stack->size : 0;
}

void s_clear(Stack *stack){
    if(not stack){
        printf("Null Reference Exception at Stack::s_clear()!\n");
        return;
    }
    Node *top_node = stack->top;
    while (top_node)
    {
        free_node(top_node);
        top_node = top_node->previous;
    }
    stack->top = NULL;
    stack->size = 0;
}

void s_destroy(Stack **stack){
    if(stack and *stack){
        s_clear(*stack);
        free(*stack);
        *stack = NULL;
    }
}

void * s_to_array(Stack *stack, int *length){
    if(not stack or not length){
        printf("Null Reference Exception at Stack::s_clear()!\n");
        return NULL;
    }
    if(s_is_empty(stack)){
        *length = 0;
        return NULL;
    }
    size_t len = stack->size, item_size = stack->item_size;
    void *array = malloc(len * item_size);
    if(not array){
        printf("Null Reference Exception at Stack::s_clear()!\n");
        return NULL;
    }
    
    Node *last = stack->top;
    int i = len-1;
    while(last){
        memcpy(array + i * item_size, last->data, item_size);

        last = last->previous;
        i--;
    }
    *length = len;
    return array;
}

void s_print(Stack *stack, char *(*to_string)(void *)){
    printf("Stack:\n");
    if(not stack){
        printf("NULL\n");
        return;
    }
    if(s_is_empty(stack)){
        printf("Empty\n");
        return;
    }
    printf("Size -> %d\n", stack->size);
    printf("Top\n");
    
    Node* current = stack->top;
    int i = 0;
    while (current)
    {
        char *str = to_string(current->data);
        printf("%d - %s\n", stack->size - i - 1, str);

        free(str);
        i++;
        current = current->previous;
    }
    
    printf("Base\n");
}

static void free_node(Node *node){
    free(node->data);
    free(node);
}

static Node *create_node(size_t item_size, void *data){
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node){
        new_node->data = malloc(item_size);
        if(new_node->data){
            memcpy(new_node->data, data, item_size);
        }else free(new_node);
    }
    return new_node;
}

