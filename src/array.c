#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "log.h"
#include "utils.h"

#define CAST_AND_ITER(arr, ctype, expr)                                        \
    ctype *data_ptr_##ctype = arr->data;                                       \
    for (size_t cai_i = 0; cai_i < arr->nelems; cai_i++) {                     \
        expr;                                                                  \
    }

// INITIALIZATION FUNCTIONS

// To avoid undefined behaviors, it is strongly recommended to only pass
// malloc'ed pointers as the data argument
Array array(size_t *shape, size_t ndim, DType dtype, void *data) {
    CHECK_NULL_PTR(shape);

    Array out = {0};

    size_t nelems = reduce_mul(shape, ndim);

    void *data_ptr;

    if (data == NULL) {
        data_ptr = SAFE_MALLOC(get_element_size(dtype) * nelems);
    } else {
        data_ptr = data;
    }

    // Since we don't know how the shape array was defined, it is
    // safer to allocate memory for a new array and copy the content of
    // the user's array; that way we can be sure we must free its memory and
    // avoid potential memory leaks in the `free_array` function
    size_t *shape_copy = SAFE_MALLOC(sizeof(*shape_copy) * ndim);
    memcpy(shape_copy, shape, sizeof(*shape_copy) * ndim);

    size_t *stride_ptr = SAFE_MALLOC(sizeof(*stride_ptr) * ndim);

    out.data = data_ptr;
    out.shape = shape_copy;
    out.ndim = ndim;
    out.nelems = nelems;
    out.strides = stride_ptr;
    out.dtype = dtype;
    out.device = cpu;

    compute_strides(&out);

    return out;
}

Array constant(void *value, size_t *shape, size_t ndim, DType dtype) {
    CHECK_NULL_PTR(shape);

    Array out = empty(shape, ndim, dtype);

    set_values(&out, value);

    return out;
}

void set_values(Array *arr, void *value) {
    switch (arr->dtype) {
    case float32:
        CAST_AND_ITER(arr, float, data_ptr_float[cai_i] = *(float *)value);
        break;
    case int32:
        CAST_AND_ITER(arr, int, data_ptr_int[cai_i] = *(int *)value);
        break;
    default:
        LOG_ERROR("Unkonwn dtype %d\n", arr->dtype);
        exit(EXIT_FAILURE);
    }
}

Array empty(size_t *shape, size_t ndim, DType dtype) {
    CHECK_NULL_PTR(shape);

    return array(shape, ndim, dtype, NULL);
}

void free_array(Array arr, bool free_data) {
    CHECK_NULL_SHAPE_PTR(&arr);
    CHECK_NULL_STRIDES_PTR(&arr);

    if (free_data) {
        CHECK_NULL_DATA_PTR(&arr);
        free(arr.data);
    }

    free(arr.shape);
    free(arr.strides);

    // For extra safety, set those pointers to NULL
    arr.data = NULL;
    arr.shape = NULL;
    arr.strides = NULL;
}

size_t get_element_size(DType dtype) {
    switch (dtype) {
    case float32:
        return sizeof(float);
    case int32:
        return sizeof(int);
    default:
        LOG_ERROR("Unsupported DType %d\n", dtype);
        exit(EXIT_FAILURE);
    }
}

// LOGIC INDEXING ⇄ MEMORY INDEXING
size_t array_index_to_memory(size_t *array_index, Array *arr) {
    CHECK_NULL_PTR(array_index);
    CHECK_NULL_PTR(arr);

    CHECK_NULL_STRIDES_PTR(arr);

    size_t memory_index = 0;
    for (int dim = 0; dim < arr->ndim; dim++) {
        memory_index += array_index[dim] * arr->strides[dim];
    }
    return memory_index;
}

size_t *memory_index_to_array(size_t memory_index, Array *arr) {
    CHECK_NULL_PTR(arr);
    CHECK_NULL_STRIDES_PTR(arr);

    size_t *array_index = SAFE_MALLOC(sizeof(*array_index) * arr->ndim);

    for (size_t dim = 0; dim < arr->ndim; dim++) {
        array_index[dim] = memory_index / arr->strides[dim];
        memory_index -= array_index[dim] * arr->strides[dim];
    }

    return array_index;
}

Array view(size_t *shape, size_t ndim, Array *arr) {
    CHECK_NULL_PTR(shape);
    CHECK_NULL_PTR(arr);

    size_t nelems = reduce_mul(shape, ndim);

    if (nelems != arr->nelems) {
        LOG_ERROR("Cannot view array with %zu elements as shape %s\n",
                  arr->nelems, get_iter_str(shape, ndim, 1024));
        exit(EXIT_FAILURE);
    }

    return array(shape, ndim, arr->dtype, arr->data);
}

// PRINT FUNCTIONS
void print_info(Array *arr) {
    CHECK_NULL_PTR(arr);

    printf("Size of Array: %zu bytes\n",
           get_element_size(arr->dtype) * arr->nelems);
    printf("The array has %zu dimensions and %zu elements\n", arr->ndim,
           arr->nelems);
    printf("Array has dtype %s\n", get_dtype_str(arr->dtype));
    printf("Array is on device %s\n", get_device_str(arr->device));

    if (arr->shape != NULL) {
        char *shape_str = get_iter_str(arr->shape, arr->ndim, 1024);
        printf("The shape of the array is: %s\n", shape_str);
        free(shape_str);
    } else {
        printf("The shape of the array is: NULL\n");
    }

    if (arr->strides != NULL) {
        printf("The stride for each dimension of the array are:\n");
        for (size_t dim = 0; dim < arr->ndim; dim++) {
            printf("\tDim %zu: %zu\n", dim, arr->strides[dim]);
        }
    } else {
        printf("The stride for each dimension of the array are: NULL\n");
    }
}
