#include "utils.h"

#include "test_array.h"
#include "test_utils.h"

int main(int argc, char **argv) {
    time_t start = time(NULL);
    clock_t tic = clock();

    TEST(test_array_index_to_memory);
    TEST(test_memory_index_to_array);
    TEST(test_stringify_shape);

    clock_t toc = clock();
    time_t end = time(NULL);
    printf("\nGlobal wall time: %.2fs, CPU time: %.2fs\n", 
           difftime(end, start), (double)(toc - tic) / CLOCKS_PER_SEC);
    
    return 0;
}
