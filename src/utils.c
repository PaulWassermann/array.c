#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// Array utils
size_t reduce_mul(size_t *arr, size_t length) {
    size_t out = 1;
    for (size_t i = 0; i < length; i++) {
        out *= arr[i];
    }
    return out;
}

// Add input validation for buffer size (must be >= 4 because at least 4 bytes
// will be written to it)
char *stringify_shape(size_t *shape, size_t ndim, size_t buffer_size) {
    char *shape_buff = SAFE_MALLOC(sizeof(*shape_buff) * buffer_size);
    size_t bytes = 0;

    bytes += snprintf(shape_buff + bytes, buffer_size, "(%zu,", shape[0]);

    for (size_t dim = 1; dim < ndim - 1; dim++) {
        bytes += snprintf(shape_buff + bytes, buffer_size - bytes, " %zu,",
                          shape[dim]);
    }

    if (ndim > 1) {
        bytes += snprintf(shape_buff + bytes, buffer_size - bytes, " %zu",
                          shape[ndim - 1]);
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

// Memory utils
void *safe_malloc(size_t size, char *file, int line) {
    void *ptr = malloc(size);

    if (ptr == NULL) {
        fprintf(stderr, "Could not allocate %zu bytes (File: %s, line: %d)\n",
                size, file, line);
        exit(1);
    }

    return ptr;
}
