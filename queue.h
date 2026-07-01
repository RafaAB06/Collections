#ifndef QUEUE_H
#define QUEUE_H

typedef struct __q_node{
    void *data;
    struct __q_node* next;
}__Q_Node;

typedef struct{
    size_t item_size;
    size_t size;
    __Q_Node *first;
    __Q_Node *last;
}Queue;

/**
 * @brief Cria uma fila.
 * @param item_size Tamanho em bytes dos itens.
 * @return Ponteiro para a fila criada.
 */
Queue * q_new(size_t item_size);

/**
 * @brief Cria uma fila a partir de um array.
 * @param item_size Tamanho em bytes dos itens.
 * @param array O array em questão.
 * @param length O comprimento do array.
 * @return Ponteiro para a fila criada.
 */
Queue * q_new_from_array(size_t item_size, void *array, size_t length);

/**
 * @brief Adiciona um item ao final da fila.
 * @param queue A fila em questão.
 * @param data Ponteiro para o valor a ser inserido.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int q_push(Queue *queue, void *data);

/**
 * @brief Remove um item ao início da fila.
 * @param queue A fila em questão.
 * @param dest Ponteiro para onde o valor será copiado.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int q_pop(Queue *queue, void *dest);

/**
 * @brief Lê um item ao início da fila.
 * @param queue A fila em questão.
 * @param dest Ponteiro para onde o valor será copiado.
 * @return 1 em caso de sucesso, 0 caso cointrário.
 */
int q_peek(Queue *queue, void *dest);

/**
 * @brief Verifica se um valor está na fila.
 * @param queue A fila em questão.
 * @param data Ponteiro para o valor a ser buscado.
 * @return 1 caso encontre, 0 caso contrário.
 */
int q_contains(Queue *queue, void *data);

/**
 * @brief Concatena duas filas de forma não destrutiva.
 * @param queue_1 A primeira fila.
 * @param queue_2 A segunda fila.
 * @return Ponteiro para a fila resultante.
 */
Queue * q_concat(Queue *queue_1, Queue * queue_2);

/**
 * @brief Clona uma fila.
 * @param queue A fila em questão.
 * @return Ponteiro para a nova fila.
 */
Queue * q_clone(Queue *queue);

/**
 * @brief Inverte uma fila.
 * @param queue A fila em questão.
 */
void q_reverse(Queue *queue);

/**
 * @brief Verifica se uma fila está vazia.
 * @param queue A fila em questão.
 * @return 1 em caso afirmativo, 0 caso contrário.
 */
int q_is_empty(Queue *queue);

/**
 * @brief Preferível do que utilizar a propriedade direta da fila, questão de encapsulamento.
 * @param queue A fila em questão.
 * @return O tamanho da fila.
 */
int q_get_size(Queue *queue);

/**
 * @brief Remove todos os itens de uma fila.
 * @param queue A fila em questão.
 */
void q_clear(Queue *queue);

/**
 * @brief Remove toda a memória relacionada a uma fila.
 * @param stack Ponteiro para o ponteiro da fila em questão.
 */
void q_destroy(Queue **queue);

/**
 * @brief Converte uma fila em array, não destrutivo.
 * @param stack A fila em questão.
 * @param size Ponteiro para o tamanho do array resultante.
 * @return Array com todos os itens da fila.
 */
void * q_to_array(Queue *queue, int *length);

/**
 * @brief Mostra todos os itens de uma fila usando uma função char * to_string(void *).
 * @param list A fila em questão.
 * @param to_string Ponteiro para uma função que retorna um ponteiro para caracteres.
 */
void q_print(Queue *queue, char *(*to_string)(void *));

#endif