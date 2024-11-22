#include "..\..\..\src\array.h"
#include "..\..\..\src\ops\ops.h"
#include "..\..\utils.h"

void test_add() {
    Array result, expected;

    Array float32_arr1 =
        array((size_t[]){2, 2}, 2, float32, (float[]){1, 2, 3, 4});
    Array float32_arr2 =
        array((size_t[]){2, 2}, 2, float32, (float[]){4, 3, 1, 2});
    result = add(&float32_arr1, &float32_arr2);
    expected = array((size_t[]){2, 2}, 2, float32, (float[]){5, 5, 4, 6});

    assert(expected.dtype == result.dtype);
    ASSERT_ALL_EQUAL((float *)expected.data, (float *)result.data,
                     expected.nelems);

    FREE_ARRAY_PARTIAL(float32_arr1);
    FREE_ARRAY_PARTIAL(float32_arr2);
    FREE_ARRAY_PARTIAL(result);
    FREE_ARRAY_PARTIAL(expected);

    Array int32_arr1 = array((size_t[]){2, 2}, 2, int32, (int[]){1, 2, 3, 4});
    Array int32_arr2 = array((size_t[]){2, 2}, 2, int32, (int[]){4, 3, 1, 2});
    result = add(&int32_arr1, &int32_arr2);
    expected = array((size_t[]){2, 2}, 2, int32, (int[]){5, 5, 4, 6});

    assert(expected.dtype == result.dtype);
    ASSERT_ALL_EQUAL((int *)expected.data, (int *)result.data, expected.nelems);

    FREE_ARRAY_PARTIAL(int32_arr1);
    FREE_ARRAY_PARTIAL(int32_arr2);
    FREE_ARRAY_PARTIAL(result);
    FREE_ARRAY_PARTIAL(expected);
}