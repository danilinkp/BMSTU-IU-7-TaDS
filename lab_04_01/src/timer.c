#include "timer.h"
#include "stack.h"
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

static unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;

    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

static void print_table_header(void)
{
    printf("| %-15s| %-16s| %-16s| %-16s| %-16s| %-16s| %-16s| %-16s|%-16s|\n", "Seq length", "Arr stack time", "List stack time",
           "Arr stack push", "List stack push", "Arr stack pop", "List stack pop",
           "Arr stack memory", "List stack memory");
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");
}

static double arr_stack_time(FILE *file, array_stack_t *stack, int count)
{
    rewind(file);
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        beg = microseconds_now();
        arr_stack_print_sequence(file, stack);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_time(FILE *file, list_stack_t *stack, int count)
{
    rewind(file);
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        beg = microseconds_now();
        list_stack_print_sequence(file, stack);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double arr_stack_push_time(array_stack_t *stack, int seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        arr_stack_init(stack, seq_len);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            arr_stack_push(stack, seq[i]);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_push_time(list_stack_t *stack, int seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        list_stack_init(stack, seq_len);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            list_stack_push(stack, seq[i]);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double arr_stack_pop_time(array_stack_t *stack, int seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        arr_stack_init(stack, seq_len);
        for (int c = 0; c < seq_len; c++)
            arr_stack_push(stack, seq[i]);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            arr_stack_pop(stack);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_pop_time(list_stack_t *stack, int seq[], int seq_len, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    int tmp;
    for (int i = 0; i < count; i++)
    {
        list_stack_init(stack, seq_len);
        for (int c = 0; c < seq_len; c++)
            list_stack_push(stack, seq[i]);
        beg = microseconds_now();
        for (int c = 0; c < seq_len; c++)
            list_stack_pop(stack, &tmp);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static void print_time(int seq_len, int seq[])
{
    FILE *file = fopen("out.txt", "wt");
    list_stack_t list_stack;
    list_stack_init(&list_stack, seq_len);

    array_stack_t array_stack;
    arr_stack_init(&array_stack, seq_len);

    for (int i = 0; i < seq_len; i++)
    {
        arr_stack_push(&array_stack, seq[i]);
        list_stack_push(&list_stack, seq[i]);
    }

    double time_arr_stack = arr_stack_time(file, &array_stack, NUM_OF_ITERATIONS);
    double time_list_stack = list_stack_time(file, &list_stack, NUM_OF_ITERATIONS);
    size_t arr_stack_memory = sizeof(int) * MAX_ARR_SIZE;
    size_t list_stack_memory = sizeof(node_t) * list_stack.size;
    double time_arr_push = arr_stack_push_time(&array_stack, seq, seq_len, NUM_OF_ITERATIONS);
    double time_list_push = list_stack_push_time(&list_stack, seq, seq_len, NUM_OF_ITERATIONS);
    double time_arr_pop = arr_stack_pop_time(&array_stack, seq, seq_len, NUM_OF_ITERATIONS);
    double time_list_pop = list_stack_pop_time(&list_stack, seq, seq_len, NUM_OF_ITERATIONS);

    printf("| %-15d| %-16lf| %-16lf| %-16lf| %-16lf| %-16lf| %-16lf| %-16zu| %-16zu|\n", seq_len, time_arr_stack, time_list_stack,
           time_arr_push, time_list_push, time_arr_pop, time_list_pop, arr_stack_memory, list_stack_memory);
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|\n");

    free_list_stack(&list_stack);
    fclose(file);
}

static void fill_arr(int arr[], int len)
{
    srand(time(NULL));
    for (int i = 0; i < len; i++)
        arr[i] = rand() % 100;
}

void print_measurements(void)
{
    int seq_lenghts[] = { 1, 10, 50, 100, 130, 500 };

    print_table_header();
    for (size_t i = 0; i < 6; i++)
    {
        int seq[seq_lenghts[i]];
        fill_arr(seq, seq_lenghts[i]);
        print_time(seq_lenghts[i], seq);
    }
    printf("\n");
}
