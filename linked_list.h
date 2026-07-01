#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct __ll_node
{
    void *data;
    struct __ll_node *next;
}__LL_Node;

typedef struct
{
    size_t item_size;
    size_t size;
    __LL_Node * first;
    __LL_Node * last;
}LinkedList;

/**
 * @brief Cria uma lista encadeada.
 * @param item_size Tamanho em bytes dos itens individuais.
 * @return Retorna um ponteiro para a lista.
 */
LinkedList * ll_new(size_t item_size);

/**
 * @brief Cria uma lista encadeada a partir dum array, não destrutivo.
 * @param array O array a ser copiado para a lista
 * @param length O comprimento do array.
 * @param item_size Tamanho em bytes dos itens individuais.
 * @return Retorna um ponteiro para a lista.
 */
LinkedList *ll_new_from_array(void *array, int length, size_t item_size);

/**
 * @brief Verifica se uma lista está vazia.
 * @param list A lista em questão.
 * @return 1 se estiver vazia, 0 caso contrário.
 */
int ll_is_empty(LinkedList *list);

/**
 * @brief Insere um item no começo de uma lista
 * @param list A lista em questão.
 * @param data Ponteiro para os dados a serem inseridos.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_insert_beginning(LinkedList *list, void *data);

/**
 * @brief Insere um item no final de uma lista
 * @param list A lista em questão.
 * @param data Ponteiro para os dados a serem inseridos.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_insert_end(LinkedList *list, void *data);

/**
 * @brief Insere um item em uma lista em um índice.
 * @param list A lista em questão.
 * @param data Ponteiro para os dados a serem inseridos.
 * @param index O índice em questão.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_insert_at_index(LinkedList *list, void *data, int index);

/**
 * @brief Remove um item no incício de uma lista.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_remove_beginning(LinkedList *list, void *dest);

/**
 * @brief Remove um item no final de uma lista.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_remove_end(LinkedList *list, void *dest);

/**
 * @brief Remove um item de uma lista em um índice.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @param index O índice em questão.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_remove_at_index(LinkedList *list, void *dest, int index);

/**
 * @brief Lê um item no incício de uma lista.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_peek_first(LinkedList *list, void *dest);

/**
 * @brief Lê um item no final de uma lista.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_peek_last(LinkedList *list, void *dest);

/**
 * @brief Lê um item de uma lista em um índice.
 * @param list A lista em questão.
 * @param dest Ponteiro para onde os dados serão copiados.
 * @param index O índice em questão.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_peek_at_index(LinkedList *list, void *dest, int index);

/**
 * @brief Localiza um item numa lista.
 * @param list A lista em questão.
 * @param data Ponteiro para os dados a serem buscados.
 * @return O indíce do item, -1 caso não o encontre.
 */
int ll_locate(LinkedList *list, const void *data);

/**
 * @brief Altera um item da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para os dados a serem buscados.
 * @param index O índice do item
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int ll_set(LinkedList *list, void* data, int index);

/**
 * @brief Inverte a lista.
 * @param list A lista em questão. 
 */
void ll_reverse(LinkedList *list);

/**
 * @brief Concatena duas listas, não destrutivo.
 * @param list_1 A primeira lista.
 * @param list_2 A segunda lista.
 * @return Ponteiro para a lista concatenada final.
 */
LinkedList * ll_concat(LinkedList *list_1, LinkedList *list_2);

/**
 * @brief Clona uma lista
 * @param list A lista em questão
 * @return Ponteiro para o clone da lista original
 */
LinkedList * ll_clone(LinkedList *list);

/**
 * @brief Transform a lista em array, não destrutivo.
 * @param list A lista em questão.
 * @param size Ponteiro para uma variável para o tamanho do array gerado
 * @return Os itens da lista em um array.
 */
void * ll_to_array(LinkedList *list, int *size);

/**
 * @brief Limpa os items de uma lista.
 * @param list A lista em questão.
 */
void ll_clear(LinkedList *list);

/**
 * @brief Libera toda a memória relacionada à lista.
 * @param list Ponteiro para a lista em questão.
 */
void ll_destroy(LinkedList **list);

/**
 * @brief Preferível do que usar a propridade direta da lista, questão de encapsulamento.
 * @param list A lista em questão.
 * @return Tamanho da lista.
 */
int ll_get_size(LinkedList *list);

/**
 * @brief Mostra a lista, baseado num método de assinatura "char * to_string(void *data)" específico.
 * @param list A lista em questão.
 * @param to_string Ponteiro para uma função que retorna um ponteiro para caracteres.
 */
void ll_print(LinkedList *list, char *(*to_string)(void *));

#endif