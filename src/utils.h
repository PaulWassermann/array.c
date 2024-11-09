#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "array.h"
#include "log.h"

#define CHECK_EQUAL_DTYPES(a, b)                                               \
    if ((a)->dtype != (b)->dtype) {                                            \
        LOG_ERROR("Arrays " #a " and " #b                                      \
                  " have different dtypes: %s and %s\n",                       \
                  get_dtype_str((a)->dtype), get_dtype_str((b)->dtype));       \
        exit(EXIT_FAILURE);                                                    \
    }
#define CHECK_EQUAL_SHAPES(a, b)                                               \
    if (!equal_shapes(a, b)) {                                                 \
        LOG_ERROR("Arrays " #a " and " #b                                      \
                  " have different shapes: %s and %s\n",                       \
                  get_iter_str((a)->shape, (a)->ndim, 1024),                   \
                  get_iter_str((b)->shape, (b)->ndim, 1024));                  \
        exit(EXIT_FAILURE);                                                    \
    }
#define CHECK_NULL_PTR(ptr) \
    if ((ptr) == NULL) { \
        LOG_ERROR("Pointer %s is NULL\n", #ptr); \
        exit(EXIT_FAILURE); \
    }
#define CHECK_NULL_DATA_PTR(arr) CHECK_NULL_PTR((arr)->data)
#define CHECK_NULL_SHAPE_PTR(arr) CHECK_NULL_PTR((arr)->shape)
#define CHECK_NULL_STRIDES_PTR(arr) CHECK_NULL_PTR((arr)->strides)

#define SAFE_MALLOC(size) safe_malloc(size, __FILE__, __LINE__, __FUNCTION__)

/*Compute the strides of the given array, with respect to its `shape` and `ndim`
members.

For a given dimension, its stride is the product of the number of elements in 
each of its inner dimensions (the rightmost dimensions).

### Parameters

#### InOut :
- arr: the array for which the strides are to be computed

### Panics
If the `shape` member is a NULL pointer.
*/
void compute_strides(Array *arr);

bool equal_shapes(Array *a, Array *b);
char *get_dtype_str(DType dtype);
char *get_iter_str(size_t *iter, size_t length, size_t buffer_size);
size_t reduce_mul(size_t *arr, size_t length);

void *safe_malloc(size_t size, char *file, int line, const char *function);

#endif