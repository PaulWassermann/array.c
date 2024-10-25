#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

typedef enum DType { float32, int32 } DType;

typedef struct Array {
    void *data;
    size_t *shape;
    size_t *strides;
    size_t ndim;
    size_t nelems;
    DType dtype;
} Array;

// INITIALIZATION FUNCTIONS
Array array(size_t *shape, size_t ndim, DType dtype, void *data);
Array empty(size_t *shape, size_t ndim, DType dtype);
void free_array(Array arr);
size_t get_element_size(DType dtype);

// LOGIC INDEXING ⇄ MEMORY INDEXING
size_t array_index_to_memory(size_t *array_index, Array *arr);
size_t *memory_index_to_array(size_t memory_index, Array *arr);
Array view(size_t *shape, size_t ndim, Array *arr);

// PRINT FUNCTIONS
void print_info(Array *arr);

#endif