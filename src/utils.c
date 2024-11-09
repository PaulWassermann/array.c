#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "log.h"
#include "utils.h"

void compute_strides(Array *arr) {
    CHECK_NULL_SHAPE_PTR(arr);
    CHECK_NULL_STRIDES_PTR(arr);

    size_t acc = 1;
    for (size_t dim = arr->ndim - 1; dim < arr->ndim; dim--) {
        arr->strides[dim] = acc;
        acc *= arr->shape[dim];
    }
}

bool equal_shapes(Array *a, Array *b) {
    CHECK_NULL_SHAPE_PTR(a);
    CHECK_NULL_SHAPE_PTR(b);

    if (a->ndim != b->ndim) {
        return false;
    }

    for (size_t dim = 0; dim < a->ndim; dim++) {
        if (a->shape[dim] != b->shape[dim]) {
            return false;
        }
    }

    return true;
}

char *get_dtype_str(DType dtype) {
    switch (dtype) {
    case float32:
        return "float32";
    case int32:
        return "int32";
    default:
        LOG_ERROR("DType %d does not exist\n", dtype);
        exit(EXIT_FAILURE);
    }
}

char *get_iter_str(size_t *iter, size_t length, size_t buffer_size) {
    CHECK_NULL_PTR(iter);

    if (buffer_size <= 6) {
        LOG_ERROR("Buffer must be have at least 6 bytes available (received "
                  "size %zu)\n",
                  buffer_size);
    }

    char *shape_buff = SAFE_MALLOC(sizeof(*shape_buff) * buffer_size);
    size_t bytes = 0;

    bytes += snprintf(shape_buff + bytes, buffer_size, "(%zu,", iter[0]);

    for (size_t i = 1; i < length - 1; i++) {
        bytes +=
            snprintf(shape_buff + bytes, buffer_size - bytes, " %zu,", iter[i]);
    }

    if (length > 1) {
        bytes += snprintf(shape_buff + bytes, buffer_size - bytes, " %zu",
                          iter[length - 1]);
    }

    if (bytes >= buffer_size) {
        bytes = buffer_size - 2;
        shape_buff[bytes - 3] = '.';
        shape_buff[bytes - 2] = '.';
        shape_buff[bytes - 1] = '.';
    }

    shape_buff[bytes] = ')';
    shape_buff[bytes + 1] = '\0';

    return shape_buff;
}

size_t reduce_mul(size_t *arr, size_t length) {
    CHECK_NULL_PTR(arr);

    size_t out = 1;
    for (size_t i = 0; i < length; i++) {
        out *= arr[i];
    }
    return out;
}

void *safe_malloc(size_t size, char *file, int line, const char *function) {
    void *ptr = malloc(size);

    if (ptr == NULL) {
        LOG_ERROR(
            "Could not allocate %zu bytes (File: %s, line: %d, function: %s)\n",
            size, file, line, function);
        exit(EXIT_FAILURE);
    }

    return ptr;
}
