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
    printf("| %-15s| %-16s| %-16s| %-16s| %-16s|\n", "Seq length", "Arr stack time", "List stack time",
           "Arr stack memory", "List stack memory");
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|\n");
}

static double arr_stack_time(array_stack_t *stack, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (size_t i = 0; i < count; i++)
    {
        beg = microseconds_now();
        arr_stack_print_sequence(stack);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static double list_stack_time(list_stack_t *stack, int count)
{
    unsigned long long beg, end;
    double sum = 0;
    for (size_t i = 0; i < count; i++)
    {
        beg = microseconds_now();
        list_stack_print_sequence(stack);
        end = microseconds_now();
        sum += end - beg;
    }
    return sum / count;
}

static void print_time(size_t seq_len, int seq[])
{
    list_stack_t list_stack;
    list_stack_init(&list_stack, seq_len);

    array_stack_t array_stack;
    arr_stack_init(&array_stack, seq_len);

    for (size_t i = 0; i < seq_len; i++)
    {
        arr_stack_push(&array_stack, seq[i]);
        list_stack_push(&list_stack, seq[i]);
    }

    double time_arr_stack = arr_stack_time(&array_stack, NUM_OF_ITERATIONS);
    double time_list_stack = list_stack_time(&list_stack, NUM_OF_ITERATIONS);

    size_t arr_stack_memory = sizeof(int) * seq_len;
    size_t list_stack_memory = sizeof(node_t) * list_stack.size;

    printf("| %-15zu| %-16lf| %-16lf| %-16zu| %-16zu|\n", seq_len, time_arr_stack, time_list_stack, arr_stack_memory, list_stack_memory);
    printf("|----------------|-----------------|-----------------|-----------------|-----------------|\n");

    free_list_stack(&list_stack);
}

static void fill_arr(int arr[], size_t len)
{
    srand(time(NULL));
    for (size_t i = 0; i < len; i++)
        arr[i] = rand() % 100;
}

void print_measurements(void)
{
    int seq_lenghts[] = { 1, 10, 50, 100, 500 };

    print_table_header();
    for (size_t i = 0; i < 5; i++)
    {
        int seq[seq_lenghts[i]];
        fill_arr(seq, seq_lenghts[i]);
        print_time(seq_lenghts[i], seq);
    }
    printf("\n");
}
