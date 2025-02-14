#include <stdio.h>
#include <stdlib.h>
#include "tlsf.h"

#define POOL_SIZE (1024 * 64)

int main() {
    void *memory_pool = malloc(POOL_SIZE);
    if (!memory_pool) {
        printf("Failed to allocate memory pool!\n");
        return 1;
    }

    tlsf_t tlsf = tlsf_create_with_pool(memory_pool, POOL_SIZE);
    if (!tlsf) {
        printf("TLSF initialization failed!\n");
        free(memory_pool);
        return 1;
    }

    printf("TLSF initialized successfully!\n");

    void *ptr = tlsf_malloc(tlsf, 128);
    if (ptr) {
        printf("Memory allocated successfully!\n");
        tlsf_free(tlsf, ptr);
        printf("Memory freed successfully!\n");
    } else {
        printf("Memory allocation failed!\n");
    }

    tlsf_destroy(tlsf);
    free(memory_pool);
    printf("TLSF memory pool destroyed!\n");

    return 0;
}
