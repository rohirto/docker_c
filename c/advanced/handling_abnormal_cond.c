/**
 * Dynamic error handling to handle abonormalities in code 
*/
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

char* read_from_file(const char* filename, size_t length) 
{
    char* buffer = NULL;
    int fd;
    ssize_t bytes_read;

    // Allocate the buffer.
    buffer = (char*)malloc(length);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Open the file.
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("File open failed");
        free(buffer);
        return NULL;
    }

    // Read the data.
    bytes_read = read(fd, buffer, length);
    if (bytes_read != length) {
        perror("File read failed");
        free(buffer);
        close(fd);
        return NULL;
    }

    // Everything’s fine. Close the file and return the buffer.
    close(fd);
    return buffer;
}

int main() {
    const char* filename = "example.txt"; // Change this to the file you want to read.
    size_t length = 100; // Change this to the desired length.

    char* data = read_from_file(filename, length);

    if (data != NULL) {
        printf("Data read from file: %s\n", data);
        free(data); // Don't forget to free the allocated buffer when done.
    } else {
        printf("File read failed. Check the error messages above.\n");
    }

    return 0;
}
