#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct __ll_node
{
    void *data;
    struct __ll_node *previous;
}__LL_Node;

typedef struct
{
    size_t item_size;
    size_t size;
    __LL_Node *top;
}Stack;

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

/**
 * @brief Cria uma pilha.
 * @param item_size Tamanho em bytes dos itens.
 * @return Ponteiro para a pilha criada.
 */
Stack * s_new(size_t item_size);

/**
 * @brief Cria uma pilha a partir de um array.
 * @param item_size Tamanho em bytes dos itens.
 * @param array O array em questão.
 * @param length O comprimento do array.
 * @return Ponteiro para a pilha criada.
 */
Stack * s_new_from_array(size_t item_size, void *array, size_t length);

/**
 * @brief Adiciona um item ao topo da pilha.
 * @param stack A pilha em questão.
 * @param data Ponteiro para o valor a ser inserido.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int s_push(Stack *stack, void *data);

/**
 * @brief Remove um item do topo da pilha.
 * @param stack A pilha em questão.
 * @param dest Ponteiro para onde o valor será copiado.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int s_pop(Stack *stack, void *dest);

/**
 * @brief Lê um item do topo da pilha.
 * @param stack A pilha em questão.
 * @param dest Ponteiro para onde o valor será copiado.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int s_peek(Stack *stack, void *dest);

/**
 * @brief Verifica se um valor está na pilha.
 * @param stack A pilha em questão.
 * @param data Ponteiro para o valor a ser buscado.
 * @return 1 caso encontre, 0 caso contrário.
 */
int s_contains(Stack *stack, void *data);

/**
 * @brief Concatena duas pilhas de forma não destrutiva.
 * @param stack_1 A primeira pilha.
 * @param stack_2 A segunda pilha.
 * @return Ponteiro para a pilha resultante.
 */
Stack * s_concat(Stack *stack_1, Stack *stack_2);

/**
 * @brief Clona uma pilha.
 * @param stack A pilha em questão.
 * @return Ponteiro para a nova pilha.
 */
Stack * s_clone(Stack *stack);

/**
 * @brief Inverte uma pilha.
 * @param stack A pilha em questão.
 */
void s_reverse(Stack *stack);

/**
 * @brief Verifica se uma pilha está vazia.
 * @param stack A pilha em questão.
 * @return 1 em caso afirmativo, 0 caso contrário.
 */
int s_is_empty(Stack *stack);

/**
 * @brief Preferível do que usar a propriedade direta, questão de encapsulamento.
 * @param stack A pilha em questão.
 * @return Tamanho da pilha.
 */
int s_get_size(Stack *stack);

/**
 * @brief Remove todos os itens da pilha.
 * @param stack A pilha em questão.
 */
void s_clear(Stack *stack);

/**
 * @brief Remove toda a memória relacionada a uma pilha.
 * @param stack Ponteiro para o ponteiro da pilha em questão.
 */
void s_destroy(Stack **stack);

/**
 * @brief Converte a pilha em array, não destrutivo.
 * @param stack A pilha em questão.
 * @param size Ponteiro para o tamanho do array resultante.
 * @return Array com todos os itens da pilha.
 */
void * s_to_array(Stack *stack, int *size);

/**
 * @brief Mostra todos os itens da pilha usando uma função char * to_string(void *).
 * @param list A lista em questão.
 * @param to_string Ponteiro para uma função que retorna um ponteiro para caracteres.
 */
void s_print(Stack *stack, char *(*to_string)(void *));

#endif