#include <assert.h>
#include <string.h>

#include "..\utils.h"

#include "..\..\src\utils.h"

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

void test_stringify_shape() {
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
        char *computed_shape = stringify_shape(shape.data, shape.len, 128);
        assert(!strcmp(expected_shapes[i], computed_shape));
    }

    SizedList shape = shapes[5];
    char *overflowing_shape = stringify_shape(shape.data, shape.len, 16);
    assert(!strcmp("(128, 3, 51...)", overflowing_shape));
}
