#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "..\src\array.h"

// INITIALIZATION FUNCTIONS
void test_array() {

    ArrayArgs args_array[] = {
        array_args((size_t[]){1}, 1, float32, malloc(sizeof(float))),
        array_args((size_t[]){64, 10}, 2, float32, NULL),
        array_args((size_t[]){256, 64, 64}, 3, float32, NULL),
        array_args((size_t[]){8, 4, 4, 1}, 4, float32, NULL),
        array_args((size_t[]){8, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, float32,
                   NULL)};

    Array expected_arrays[] = {
        (Array){.data = NULL,
                .shape = (size_t[]){1},
                .strides = (size_t[]){1},
                .ndim = 1,
                .nelems = 1,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){64, 10},
                .strides = (size_t[]){10, 1},
                .ndim = 2,
                .nelems = 640,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){256, 64, 64},
                .strides = (size_t[]){64 * 64, 64, 1},
                .ndim = 3,
                .nelems = (size_t)256 * 64 * 64,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){8, 4, 4, 1},
                .strides = (size_t[]){16, 4, 1, 1},
                .ndim = 4,
                .nelems = 8 * 4 * 4,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){8, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                .strides = (size_t[]){1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                .ndim = 10,
                .nelems = 8,
                .dtype = float32},
    };

    for (size_t i = 0; i < LENGTH(args_array); i++) {
        ArrayArgs args = args_array[i];
        Array arr = array(args.shape, args.ndim, args.dtype, args.data);
        Array expected_arr = expected_arrays[i];

        if (args.data == NULL) {
            assert(arr.data != NULL);
        } else {
            assert(arr.data == args.data);
        }

        assert(
            elementwise_cmp(arr.shape, expected_arr.shape, expected_arr.ndim));
        assert(elementwise_cmp(arr.strides, expected_arr.strides,
                               expected_arr.ndim));
        assert(arr.ndim == expected_arr.ndim);
        assert(arr.nelems == expected_arr.nelems);
        assert(arr.dtype == expected_arr.dtype);

        free_array(arr);
    }
}

void test_empty() {
    ArrayArgs args_array[] = {
        array_args((size_t[]){1}, 1, float32, NULL),
        array_args((size_t[]){64, 10}, 2, float32, NULL),
        array_args((size_t[]){256, 64, 64}, 3, float32, NULL),
        array_args((size_t[]){8, 4, 4, 1}, 4, float32, NULL),
        array_args((size_t[]){8, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, float32,
                   NULL)};

    Array expected_arrays[] = {
        (Array){.data = NULL,
                .shape = (size_t[]){1},
                .strides = (size_t[]){1},
                .ndim = 1,
                .nelems = 1,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){64, 10},
                .strides = (size_t[]){10, 1},
                .ndim = 2,
                .nelems = 640,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){256, 64, 64},
                .strides = (size_t[]){64 * 64, 64, 1},
                .ndim = 3,
                .nelems = (size_t)256 * 64 * 64,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){8, 4, 4, 1},
                .strides = (size_t[]){16, 4, 1, 1},
                .ndim = 4,
                .nelems = 8 * 4 * 4,
                .dtype = float32},
        (Array){.data = NULL,
                .shape = (size_t[]){8, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                .strides = (size_t[]){1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                .ndim = 10,
                .nelems = 8,
                .dtype = float32},
    };

    for (size_t i = 0; i < LENGTH(args_array); i++) {
        ArrayArgs args = args_array[i];
        Array arr = empty(args.shape, args.ndim, args.dtype);
        Array expected_arr = expected_arrays[i];

        assert(arr.data != NULL);
        assert(
            elementwise_cmp(arr.shape, expected_arr.shape, expected_arr.ndim));
        assert(elementwise_cmp(arr.strides, expected_arr.strides,
                               expected_arr.ndim));
        assert(arr.ndim == expected_arr.ndim);
        assert(arr.nelems == expected_arr.nelems);
        assert(arr.dtype == expected_arr.dtype);

        free_array(arr);
    }
}

void test_get_element_size() {
    assert(get_element_size(float32) == sizeof(float));
}

void test_array_index_to_memory() {
    Array arr = empty((size_t[4]){4, 3, 16, 16}, 4, float32);

    size_t array_indices[][4] = {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {3, 2, 15, 15},
    };

    size_t memory_indices[] = {0, 1041, 3071};

    for (size_t i = 0; i < LENGTH(array_indices); i++) {
        assert(array_index_to_memory(array_indices[i], &arr) ==
               memory_indices[i]);
    }

    free_array(arr);
}

void test_memory_index_to_array() {
    Array arr = empty((size_t[4]){4, 3, 16, 16}, 4, float32);

    size_t memory_indices[] = {0, 678, 3071};

    size_t array_indices[][4] = {
        {0, 0, 0, 0},
        {0, 2, 10, 6},
        {3, 2, 15, 15},
    };

    for (size_t i = 0; i < LENGTH(array_indices); i++) {
        size_t *computed_indices =
            memory_index_to_array(memory_indices[i], &arr);
        assert(elementwise_cmp(array_indices[i], computed_indices,
                               LENGTH(array_indices[i])));
    }

    free_array(arr);
}

void test_view() {
    Array arr = empty((size_t[]){2, 8, 8, 1}, 4, float32);

    SizedList views[] = {sized_list((size_t[]){128}, 1),
                         sized_list((size_t[]){16, 8}, 2),
                         sized_list((size_t[]){16, 8, 1}, 3),
                         sized_list((size_t[]){2, 8, 8, 1}, 4),
                         sized_list((size_t[]){1, 8, 2, 8}, 4),
                         sized_list((size_t[]){2, 4, 8, 2, 1}, 5)};

    for (size_t i = 0; i < LENGTH(views); i++) {
        SizedList new_view = views[i];
        Array new_arr = view(new_view.data, new_view.len, &arr);

        assert(elementwise_cmp(new_arr.shape, new_view.data, new_arr.ndim));
    }

    free_array(arr);
}