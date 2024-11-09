#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "ops\ops.h"

int main(int argc, char **argv) {

    Array array1 = array((size_t[]){4, 2}, 2, float32, (float[]){1, 2, 3, 4});
    Array array2 = array((size_t[]){2, 2}, 2, float32, (float[]){4, 3, 1, 2});

    Array result = add(&array1, &array2);

    print_info(&result);
    
    return EXIT_SUCCESS;
}
