#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define SAFE_MALLOC(size) safe_malloc(size, __FILE__, __LINE__)

// Array utils
size_t reduce_mul(size_t *arr, size_t length);
char *stringify_shape(size_t *shape, size_t ndim, size_t buffer_size);

// Memory utils
void *safe_malloc(size_t size, char *file, int line);

#endif