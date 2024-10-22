#include "utils.h"

bool array_indices_cmp(size_t *a, size_t *b, size_t ndim) {
    for (size_t dim = 0; dim < ndim; dim++) {
        if (a[dim] != b[dim]) {
            return false;
        }
    }
    return true;
}

SizedList sized_list(void *data, size_t len) {
    SizedList out = { .data = data, .len = len };
    return out;
}
