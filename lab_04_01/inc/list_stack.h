#ifndef _LIST_STACK_H_
#define _LIST_STACK_H_

typedef struct node
{
    int data;
    struct node *next;
} node_t;

typedef struct free_node {
    void *address;
    struct free_node *next;
} free_node_t;

typedef struct
{
    node_t *ps; // Указатель на верхушку стека
    int size; // Размер стека
    int max_size; // Максимальный размер стека

    free_node_t *free_list; // Cписок освобжденных элементов
    int free_elems_size; // Кол-во освобожденных элементов
} list_stack_t;

enum errors
{
    STACK_OVERFLOW = 1,
    STACK_UNDERFLOW,
    NODE_ALLOC_ERROR,
    FREE_NODE_ALLOC_ERROR,

};

void init(list_stack_t *stack, int max_size);

int push(list_stack_t *stack, int value);



#endif //_LIST_STACK_H_
