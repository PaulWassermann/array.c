#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "..\src\array.h"

#define LENGTH(static_array) sizeof(static_array) / sizeof(static_array[0])

#define TEST(func)                                                             \
    printf("=> Testing %s... ", #func);                                        \
    time_t func##_start = time(NULL);                                          \
    clock_t func##_tic = clock();                                              \
    func();                                                                    \
    clock_t func##_toc = clock();                                              \
    time_t func##_end = time(NULL);                                            \
    printf("OK\n");                                                            \
    printf("Wall time: %fs, CPU time: %fs\n\n",                                \
           difftime(func##_end, func##_start),                                 \
           (double)(func##_toc - func##_tic) / CLOCKS_PER_SEC);

typedef struct ArrayArgs {
    size_t *shape;
    size_t ndim;
    DType dtype;
    void *data;
} ArrayArgs;

typedef struct SizedList {
    void *data;
    size_t len;
} SizedList;

ArrayArgs array_args(size_t *shape, size_t ndim, DType dtype, void *data);
bool elementwise_cmp(size_t *a, size_t *b, size_t ndim);
SizedList sized_list(void *data, size_t len);

#endif
