#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "..\src\array.h"
#include "..\src\log.h"

// Platform specific includes
#include <windows.h>

#define ASSERT_ALL_EQUAL(a, b, length)                                         \
    for (size_t i_aae = 0; i_aae < (length); i_aae++) {                        \
        assert((a)[i_aae] == (b)[i_aae]);                                      \
    }

#define LENGTH(static_array) sizeof(static_array) / sizeof(static_array[0])

#define TEST(func)                                                             \
    printf("=> Testing %s... ", #func);                                        \
    LARGE_INTEGER func##_tic, func##_toc;                                      \
                                                                               \
    time_t func##_start = time(NULL);                                          \
    QueryPerformanceCounter(&func##_tic);                                      \
    func();                                                                    \
    QueryPerformanceCounter(&func##_toc);                                      \
    time_t func##_end = time(NULL);                                            \
                                                                               \
    printf("OK\n");                                                            \
    printf("Wall time: %.2fs, CPU time: %fs\n\n",                              \
           difftime(func##_end, func##_start),                                 \
           (double)(func##_toc.QuadPart - func##_tic.QuadPart) /               \
               get_cpu_frequency());

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
SizedList sized_list(void *data, size_t len);

ArrayArgs array_args(size_t *shape, size_t ndim, DType dtype, void *data) {
    return (ArrayArgs){
        .shape = shape, .ndim = ndim, .dtype = dtype, .data = data};
}

LONGLONG get_cpu_frequency() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

SizedList sized_list(void *data, size_t len) {
    return (SizedList){.data = data, .len = len};
}

#endif
