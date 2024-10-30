#ifndef _ARRAY_STACK_H_
#define _ARRAY_STACK_H_

#define MAX_STACK_SIZE 1000

typedef struct
{
    int data[MAX_STACK_SIZE];
    int ps;
} array_stack_t;

enum stack_errors
{
    STACK_OVERFLOW = 1,
    STACK_UNDERFLOW
};

void init(array_stack_t *stack);

int is_empty(array_stack_t *stack);

int is_full(array_stack_t *stack);

int push(array_stack_t *stack, const int value);

int pop(array_stack_t *stack, int *value);

int peek(array_stack_t *stack, int *value);

void print_stack(array_stack_t *stack);

#endif //_ARRAY_STACK_H_
