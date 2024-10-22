#include <string.h>

#include "..\src\array.h"
#include "utils.h"

void test_array_index_to_memory() {
    Array arr = empty((size_t[4]){4, 3, 16, 16}, 4, float32);

    size_t array_indices[][4]= {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {3, 2, 15, 15},
    };

    size_t memory_indices[] = {
        0,
        1041,
        3071
    };

    for (size_t i = 0; i < LENGTH(array_indices); i++) {
        assert(array_index_to_memory(array_indices[i], &arr) == memory_indices[i]);
    }

    free_array(arr);
}

void test_memory_index_to_array() {
    Array arr = empty((size_t[4]){4, 3, 16, 16}, 4, float32);

    size_t memory_indices[] = {
        0,
        678,
        3071
    };

    size_t array_indices[][4]= {
        {0, 0, 0, 0},
        {0, 2, 10, 6},
        {3, 2, 15, 15},
    };

    for (size_t i = 0; i < LENGTH(array_indices); i++) {
        size_t *computed_indices = memory_index_to_array(memory_indices[i], &arr);
        assert(array_indices_cmp(array_indices[i], computed_indices, LENGTH(array_indices[i])));
    }

    free_array(arr);
}
