#include <string.h>

#include "..\src\utils.h"
#include "utils.h"

void test_stringify_shape() {
    SizedList shapes[] = {
        sized_list((size_t[]){1}, 1),
        sized_list((size_t[]){1024}, 1),
        sized_list((size_t[]){1, 1}, 2),
        sized_list((size_t[]){4096, 784}, 2),
        sized_list((size_t[]){1, 1, 1, 1}, 4),
        sized_list((size_t[]){128, 3, 512, 512}, 4)
    };

    char *expected_shapes[] = {
        "(1,)",
        "(1024,)",
        "(1, 1)",
        "(4096, 784)",
        "(1, 1, 1, 1)",
        "(128, 3, 512, 512)",
    };

    for (size_t i = 0; i < LENGTH(shapes); i++) {
        SizedList shape = shapes[i];
        char *computed_shape = stringify_shape(shape.data, shape.len, (size_t)128);
        assert(!strcmp(expected_shapes[i], computed_shape));
    }
}
