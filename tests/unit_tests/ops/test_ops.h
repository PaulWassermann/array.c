#include "..\..\..\src\array.h"
#include "..\..\..\src\ops\ops.h"
#include "..\..\utils.h"

void test_add() {
    SizedList operands[] = {sized_list(
        (Array[]){array((size_t[]){2, 2}, 2, float32, (float[]){1, 2, 3, 4}),
                  array((size_t[]){2, 2}, 2, float32, (float[]){4, 3, 1, 2})},
        2)};

    SizedList expected_results[] = {sized_list((float[]){5, 5, 4, 6}, 4)};

    for (size_t i = 0; i < LENGTH(operands); i++) {
        Array array1 = ((Array *)operands[i].data)[0];
        Array array2 = ((Array *)operands[i].data)[1];

        Array result = add(&array1, &array2);
        float *expected_result = expected_results[i].data;
        size_t length = expected_results[i].len;

        ASSERT_ALL_EQUAL(expected_result, (float *)result.data, length);
    }
}