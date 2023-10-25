/**
 * After running the 0_5_mapped_mem_write program (which generates a random no and stores in a file at /tmp/integer-file)
 * This code will that file and double that integer
 * 
 * How to run this code after building
 * $ ./0_6_mapped_mem_read /tmp/integer-file
 * $ cat /tmp/integer-file  <== which show the operation performed
*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main(int argc, char* const argv[]) 
{
    int fd;
    void* file_memory;
    int integer;

    /* Open the file. */
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);

    /* Create the memory mapping. */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    /* Read the integer, print it out, and double it. */
    sscanf(file_memory, "%d", &integer);
    printf("value: %d\n", integer);
    sprintf((char*)file_memory, "%d\n", 2 * integer);

    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
