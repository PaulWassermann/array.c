#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>

#define FREE_ARRAY(arr) free_array(arr, true)
#define FREE_ARRAY_PARTIAL(arr) free_array(arr, false)

typedef enum Device { cpu, cuda } Device;
typedef enum DType { float32, int32 } DType;

typedef struct Array {
    void *data;
    size_t *shape;
    size_t *strides;
    size_t ndim;
    size_t nelems;
    DType dtype;
    Device device;
} Array;

// INITIALIZATION FUNCTIONS
Array array(size_t *shape, size_t ndim, DType dtype, void *data);
Array constant(void *value, size_t *shape, size_t ndim, DType dtype);
void set_values(Array *arr, void *value);
Array empty(size_t *shape, size_t ndim, DType dtype);
void free_array(Array arr, bool free_data);
size_t get_element_size(DType dtype);

// LOGIC INDEXING ⇄ MEMORY INDEXING
size_t array_index_to_memory(size_t *array_index, Array *arr);
size_t *memory_index_to_array(size_t memory_index, Array *arr);
Array view(size_t *shape, size_t ndim, Array *arr);

// PRINT FUNCTIONS
void print_info(Array *arr);

#endif