#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "..\utils.h"

#include "..\..\src\array.h"
#include "..\..\src\utils.h"

void test_compute_strides() {
    size_t strides[8] = {1};

    Array arrays[] = {
        (Array){.shape = (size_t[]){1}, .ndim = 1, .strides = strides},
        (Array){.shape = (size_t[]){3, 3}, .ndim = 2, .strides = strides},
        (Array){.shape = (size_t[]){8, 16, 16}, .ndim = 3, .strides = strides},
        (Array){
            .shape = (size_t[]){4, 128, 128, 3}, .ndim = 4, .strides = strides},
        (Array){.shape = (size_t[]){8, 7, 6, 5, 4, 3, 2, 1},
                .ndim = 8,
                .strides = strides}};

    SizedList expected_strides[] = {
        sized_list((size_t[]){1}, 1), sized_list((size_t[]){3, 1}, 2),
        sized_list((size_t[]){16 * 16, 16, 1}, 3),
        sized_list((size_t[]){128 * 128 * 3, 128 * 3, 3, 1}, 4),
        sized_list((size_t[]){7 * 720, 720, 120, 24, 6, 2, 1, 1}, 8)};

    for (size_t i = 0; i < LENGTH(arrays); i++) {
        compute_strides(&arrays[i]);
        size_t *expected = expected_strides[i].data;

        ASSERT_ALL_EQUAL(expected, strides, expected_strides[i].len);
    }
}

void test_equal_shapes() {
    SizedList pairs[] = {
        sized_list((Array[]){(Array){.shape = (size_t[]){1}, .ndim = 1},
                             (Array){.shape = (size_t[]){1}, .ndim = 1}},
                   2),
        sized_list((Array[]){(Array){.shape = (size_t[]){1}, .ndim = 1},
                             (Array){.shape = (size_t[]){2}, .ndim = 1}},
                   2),
        sized_list((Array[]){(Array){.shape = (size_t[]){2, 2}, .ndim = 2},
                             (Array){.shape = (size_t[]){2, 2}, .ndim = 2}},
                   2),
        sized_list((Array[]){(Array){.shape = (size_t[]){16, 256}, .ndim = 2},
                             (Array){.shape = (size_t[]){16, 128}, .ndim = 2}},
                   2),
        sized_list(
            (Array[]){(Array){.shape = (size_t[]){16, 128, 128}, .ndim = 3},
                      (Array){.shape = (size_t[]){16, 128, 128}, .ndim = 3}},
            2),
        sized_list(
            (Array[]){(Array){.shape = (size_t[]){128, 128}, .ndim = 2},
                      (Array){.shape = (size_t[]){1, 128, 128}, .ndim = 3}},
            2),
    };

    bool expected[] = {true, false, true, false, true, false};

    for (size_t i = 0; i < LENGTH(expected); i++) {
        Array arr1 = ((Array *)pairs[i].data)[0];
        Array arr2 = ((Array *)pairs[i].data)[1];

        assert(expected[i] == equal_shapes(&arr1, &arr2));
    }
}

void test_get_device_str() {
    assert(strcmp(get_device_str(cpu), "cpu") == 0);
    assert(strcmp(get_device_str(cuda), "cuda") == 0);
}

void test_get_dtype_str() {
    assert(strcmp(get_dtype_str(float32), "float32") == 0);
    assert(strcmp(get_dtype_str(int32), "int32") == 0);
}

void test_get_iter_str() {
    SizedList shapes[] = {sized_list((size_t[]){1}, 1),
                          sized_list((size_t[]){1024}, 1),
                          sized_list((size_t[]){1, 1}, 2),
                          sized_list((size_t[]){4096, 784}, 2),
                          sized_list((size_t[]){1, 1, 1, 1}, 4),
                          sized_list((size_t[]){128, 3, 512, 512}, 4)};

    char *expected_shapes[] = {
        "(1,)",        "(1024,)",      "(1, 1)",
        "(4096, 784)", "(1, 1, 1, 1)", "(128, 3, 512, 512)",
    };

    for (size_t i = 0; i < LENGTH(shapes); i++) {
        SizedList shape = shapes[i];
        char *computed_shape = get_iter_str(shape.data, shape.len, 128);
        assert(strcmp(expected_shapes[i], computed_shape) == 0);

        free(computed_shape);
    }

    SizedList shape = shapes[5];
    char *overflowing_shape = get_iter_str(shape.data, shape.len, 16);
    assert(strcmp("(128, 3, 51...)", overflowing_shape) == 0);

    free(overflowing_shape);
}

void test_reduce_mul() {
    SizedList arrays[] = {
        sized_list((size_t[]){1}, 1),
        sized_list((size_t[]){10, 5, 2, 1, 1, 1}, 6),
        sized_list((size_t[]){100, 100, 100, 100, 100, 100}, 6)};

    size_t products[] = {1, 100, 1000000000000};

    for (size_t i = 0; i < LENGTH(products); i++) {
        SizedList array = arrays[i];
        assert(reduce_mul(array.data, array.len) == products[i]);
    }
}
