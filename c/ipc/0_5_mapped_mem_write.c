/**
 * Mapped Memory another IPC 
 * Mapped memory, also known as memory-mapped files or memory-mapped I/O, is a technique in interprocess communication (IPC) that 
 * allows multiple processes to share memory segments, typically a file or a portion of physical memory. Memory-mapped IPC is used 
 * for sharing data between processes efficiently. It's a mechanism where processes can access the same memory region, 
 * allowing them to communicate and share data without explicitly copying it between them.
 * 
 * How to run this program after building
 * $ ./0_5_mapped_mem_write /tmp/integer-file
 * $ cat /tmp/integer-file  <== To view what the random number our code has generated and store in the File
*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

/* Return a uniformly random number in the range [low, high]. */
int random_range(unsigned const low, unsigned const high) 
{
    unsigned const range = high - low + 1;
    return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char* const argv[]) 
{
    int fd;
    void* file_memory;

    /* Seed the random number generator. */
    srand(time(NULL));

    /* Prepare a file large enough to hold an unsigned integer. */
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    /* Create the memory mapping. */
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    /* Write a random integer to the memory-mapped area. */
    sprintf((char*)file_memory, "%d\n", random_range(-100, 100));

    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
