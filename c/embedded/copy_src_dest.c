#include <stdio.h>
#include <string.h>

void memoryCopy(void *dest, const void *src, size_t size) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

int main() {
    int source[] = {1, 2, 3, 4, 5};
    int destination[5];

    size_t size = sizeof(source);

    // Copy the contents of source to destination
    memoryCopy(destination, source, size);

    printf("Source: ");
    for (size_t i = 0; i < size / sizeof(int); i++) {
        printf("%d ", source[i]);
    }

    printf("\nDestination: ");
    for (size_t i = 0; i < size / sizeof(int); i++) {
        printf("%d ", destination[i]);
    }

    return 0;
}