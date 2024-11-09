#include <stdlib.h>

#include "..\log.h"
#include "..\utils.h"
#include "float32_ops.h"
#include "ops.h"

// IMPL_ADD(float, float32);

Array add(Array *a, Array *b) {
    CHECK_NULL_DATA_PTR(a);
    CHECK_NULL_DATA_PTR(b);

    CHECK_EQUAL_DTYPES(a, b);
    CHECK_EQUAL_SHAPES(a, b);

    switch (a->dtype) {
    case float32:
        return add_float32(a, b);
    default:
        LOG_ERROR("Function 'add' is not implemented for dtype %s",
                  get_dtype_str(a->dtype));
        exit(EXIT_FAILURE);
    }
}