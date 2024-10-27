#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int main(int argc, char **argv) {

    Array arr = empty((size_t[]){100000000, 10000000000, 100000000}, 3, float32);

    print_info(&arr);
    
    return EXIT_SUCCESS;
}
