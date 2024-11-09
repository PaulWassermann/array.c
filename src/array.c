#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "log.h"
#include "utils.h"

// INITIALIZATION FUNCTIONS
Array array(size_t *shape, size_t ndim, DType dtype, void *data) {
    CHECK_NULL_PTR(shape);
    
    Array out = {0};

    size_t nelems = reduce_mul(shape, ndim);

    void *data_ptr;

    // Since we don't know how the data and shape arrays were defined, it is
    // safer to allocate memory for new arrays and copy the content of
    // the users's arrays; that way we can be sure we must free their memory and
    // avoid potential memory leaks in the `free_array` function
    if (data == NULL) {
        data_ptr = SAFE_MALLOC(get_element_size(dtype) * nelems);
    } else {
        data_ptr = data;
    }

    size_t *shape_copy = SAFE_MALLOC(sizeof(*shape_copy) * ndim);

    for (size_t dim = 0; dim < ndim; dim++) {
        shape_copy[dim] = shape[dim];
    }

    size_t *stride_ptr = SAFE_MALLOC(sizeof(*stride_ptr) * ndim);

    out.data = data_ptr;
    out.shape = shape_copy;
    out.ndim = ndim;
    out.nelems = nelems;
    out.strides = stride_ptr;
    out.dtype = dtype;

    compute_strides(&out);

    return out;
}

Array empty(size_t *shape, size_t ndim, DType dtype) {
    CHECK_NULL_PTR(shape);

    return array(shape, ndim, dtype, NULL);
}

void free_array(Array arr) {
    CHECK_NULL_DATA_PTR(&arr);
    CHECK_NULL_SHAPE_PTR(&arr);
    CHECK_NULL_STRIDES_PTR(&arr);

    free(arr.data);
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
