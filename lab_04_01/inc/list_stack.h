#ifndef _LIST_STACK_H_
#define _LIST_STACK_H_

#include <stdio.h>

// Узел списка для стека
typedef struct node
{
    int data;
    struct node *next;
} node_t;

// Узел списка для освобожденных адрессов
typedef struct free_node
{
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
    NODE_ALLOC_ERROR,
    FREE_NODE_ALLOC_ERROR
};

void list_stack_init(list_stack_t *stack, int max_size);

int is_list_stack_empty(list_stack_t *stack);

int is_list_stack_full(list_stack_t *stack);

int list_stack_push(list_stack_t *stack, int value);

int list_stack_pop(list_stack_t *stack, int *value);

int list_stack_peek(list_stack_t *stack);

void print_list_stack(FILE *file, list_stack_t *stack);

void print_free_list(list_stack_t *stack);

void free_list_stack(list_stack_t *stack);

#endif //_LIST_STACK_H_
