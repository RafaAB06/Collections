#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>

typedef struct
{
    size_t item_size;
    size_t length;
    size_t step_size;
    size_t capacity;
    void **array;
}ArrayList;

/**
 * @brief Cria uma lista nova.
 * @param item_size Tamanho em bytes dos itens.
 * @param step_size Tamanho do incremento e tamanho inicial da lista, em items(NÃO BYTES!).
 * @return Ponteiro para a lista criada.
 */
ArrayList * al_new(size_t item_size, size_t step_size);

/**
 * @brief Cria uma lista nova a partir de um array.
 * @param array Ponteiro para o array
 * @param length Quantidade de itens no array.
 * @param item_size Tamanho em bytes dos itens.
 * @param step_size Tamanho do incremento e tamanho inicial da lista, em items(NÃO BYTES!).
 * @return Ponteiro para a lista criada.
 */
ArrayList * al_new_from_array(void *array, size_t length, size_t item_size, size_t step_size);

/**
 * @brief Muda um valor na lista.
 * @param list A lista em questão.
 * @param data Ponteiro para o dado a ser inserido.
 * @param index índice da mudança.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_set(ArrayList *list, void *data, int index);

/**
 * @brief Insere um item ao início da lista. Equivalente a array[index] = valor num array comum.
 * @param list A lista em questão.
 * @param data Ponteiro para o dado a ser inserido.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_insert_beggining(ArrayList *list, void *data);

/**
 * @brief Insere um item ao final da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para o dado a ser inserido.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_insert_end(ArrayList *list, void *data);

/**
 * @brief Insere um item em uma posição da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para o dado a ser inserido.
 * @param index Posição da inserção.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_insert_at_index(ArrayList *list, void *data, int index);

/**
 * @brief Remove o primeiro item da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_remove_first(ArrayList *list, void *dest);

/**
 * @brief Remove o último item da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_remove_last(ArrayList *list, void *dest);

/**
 * @brief Remove um item da lista numa posição específica.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_remove_at_index(ArrayList *list, void *dest, int index);

/**
 * @brief Lê o valor do primeiro item da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @param index Posição da remoção.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_peek_first(ArrayList *list, void *dest);

/**
 * @brief Lê o valor do último item da lista.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_peek_last(ArrayList *list, void *dest);

/**
 * @brief Lê o valor dum item da lista numa posição específica.
 * @param list A lista em questão.
 * @param data Ponteiro para onde os dados serão copiados.
 * @param index Posição da leitura.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_peek_at_index(ArrayList *list, void *dest, int index);

/**
 * @brief Inverte a lista.
 * @param list A lista em questão.
 */
void al_reverse(ArrayList *list);

/**
 * @brief Busca sequencialmente um item na lista.
 * @param list A lista em questão.
 * @param data ponteiro para o dado a ser buscado.
 * @return Posição do item, caso encontrado. -1 caso contrário.
 */
int al_locate(ArrayList *list, void *data);

/**
 * @brief Clone uma lista.
 * @param list A lista em questão.
 * @return Ponteiro para a nova lista.
 */
ArrayList * al_clone(ArrayList *list);

/**
 * @brief Concatena duas listas, não destrutivo.
 * @param list_1 A primeira lista.
 * @param list_2 A segunda lista.
 * @param step_size O tamanho do incremento da lista concatenada
 * @return Ponteiro para a lista concatenada.
 */
ArrayList * al_concat(ArrayList *list_1, ArrayList *list_2, size_t step_size);

/**
 * @brief Copia os itens da lista num array.
 * @param list A lista em questão.
 * @param length Ponteiro para o tamanho do array.
 * @return Ponteiro para o array criado.
 */
void * al_to_array(ArrayList *list, size_t *length);

/**
 * @brief Verifica se a lista está vazia.
 * @param list A lista em questão.
 * @return 1 em caso afirmativo, 0 caso contrário.
 */
int al_is_empty(ArrayList *list);

/**
 * @brief Preferível do que usar a propridade direta da lista, questão de encapsulamento.
 * @param list A lista em questão.
 * @return Tamanho da lista.
 */
int al_get_size(ArrayList *list);

/**
 * @brief Preferível do que usar a propridade direta da lista, questão de encapsulamento.
 * @param list A lista em questão.
 * @return Capacidade da lista.
 */
int al_get_capacity(ArrayList *list);

/**
 * @brief Muda o incremento de lista. 
 * Preferível do que usar a propridade direta da lista, questão de encapsulamento.
 * @param list A lista em questão.
 */
void al_set_step_size(ArrayList *list, size_t step_size);

/**
 * @brief Altera a capacidade mínima da lista. Pode "cortar" a lista.
 * @param list A lista em questão.
 * @param min_cap Capacidade desejada
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_ensure_capacity(ArrayList *list, size_t cap);

/**
 * @brief Compacta a lista, liberando memória.
 * @param list A lista em questão.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */
int al_compact(ArrayList *list);

/**
 * @brief Remove todos os itens da lista.
 * @param list A lista em questão.
 */
void al_clear(ArrayList *list);

/**
 * @brief Destrói a lista e todos os seus itens.
 * @param list A lista em questão.
 */
void al_destroy(ArrayList **list);

/**
 * @brief Ordena a lista via quick sort usando uma função de comparação int comparator(void *, void *).
 * @param list A lista em questão.
 * @param comparator Ponteiro para função que deve retornar 1 se o primeiro item for menor que o segundo,
 * 0 caso contrário.
 * @param ascending 1 para ordem crescente, 0 para ordem descrescente.
 */
void al_sort(ArrayList *list, int(*comparator)(void *, void *), int ascending);

/**
 * @brief Mostra todos os itens da lista usando uma função char * to_string(void *).
 * @param list A lista em questão.
 * @param to_string Ponteiro para uma função que retorna um ponteiro para caracteres.
 */
void al_print(ArrayList *list, char *(*to_string)(void *));

#endif