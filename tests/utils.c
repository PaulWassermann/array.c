#include "utils.h"

ArrayArgs array_args(size_t *shape, size_t ndim, DType dtype, void *data) {
    return (ArrayArgs){
        .shape = shape, .ndim = ndim, .dtype = dtype, .data = data};
}

bool elementwise_cmp(size_t *a, size_t *b, size_t ndim) {
    for (size_t dim = 0; dim < ndim; dim++) {
        if (a[dim] != b[dim]) {
            return false;
        }
    }
    return true;
}

SizedList sized_list(void *data, size_t len) {
    return (SizedList){.data = data, .len = len};
}
