#include "..\array.h"

Array add_float32(Array *a, Array *b);

Array add_float32(Array *a, Array *b) {
    float *a_data = a->data;
    float *b_data = b->data;

    Array out = empty(a->shape, a->ndim, float32);
    float *out_data = out.data;

    for (size_t i = 0; i < a->nelems; i++) {
        out_data[i] = a_data[i] + b_data[i];
    }

    return out;
}