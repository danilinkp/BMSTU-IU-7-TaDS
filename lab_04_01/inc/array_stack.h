#ifndef _ARRAY_STACK_H_
#define _ARRAY_STACK_H_

#include <stdio.h>

#define MAX_ARR_SIZE 500

typedef struct
{
    int data[MAX_ARR_SIZE];
    int max_size;
    int ps;
} array_stack_t;


void arr_stack_init(array_stack_t *stack, int max_size);

int is_arr_stack_empty(array_stack_t *stack);

int is_arr_stack_full(array_stack_t *stack);

void arr_stack_push(array_stack_t *stack, int value);

int arr_stack_pop(array_stack_t *stack);

int arr_stack_peek(array_stack_t *stack);

void print_arr_stack(FILE *file, array_stack_t *stack);

#endif //_ARRAY_STACK_H_
