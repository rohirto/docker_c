/**
 * Chat GPT example of mapped memory implementation
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    const char* shared_memory_name = "/my_shared_memory"; // Name for the shared memory
    const int shared_memory_size = 4096; // Size of the shared memory in bytes

    int shared_memory_fd; // File descriptor for the shared memory
    char* shared_memory_ptr; // Pointer to the shared memory

    // Create (or open if it exists) the shared memory segment
    shared_memory_fd = shm_open(shared_memory_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shared_memory_fd == -1) 
    {
        perror("shm_open");
        exit(1);
    }

    // Resize the shared memory segment to the desired size
    if (ftruncate(shared_memory_fd, shared_memory_size) == -1) 
    {
        perror("ftruncate");
        exit(1);
    }

    // Map the shared memory segment into the address space of the process
    shared_memory_ptr = mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    if (shared_memory_ptr == MAP_FAILED) 
    {
        perror("mmap");
        exit(1);
    }

    // Writing data to the shared memory
    strcpy(shared_memory_ptr, "Hello from the first process!");

    // Fork a child process
    pid_t child_pid = fork();

    if (child_pid == -1) 
    {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) 
    {
        // Child process

        // Map the shared memory segment into the address space of the child process
        shared_memory_ptr = mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
        if (shared_memory_ptr == MAP_FAILED) 
        {
            perror("mmap in child");
            exit(1);
        }

        printf("Child process reads from shared memory: %s\n", shared_memory_ptr);

        // Unmap the shared memory segment in the child process
        if (munmap(shared_memory_ptr, shared_memory_size) == -1) 
        {
            perror("munmap in child");
            exit(1);
        }
    } 
    else 
    {
        // Parent process

        // Wait for the child to finish
        wait(NULL);

        // Unmap the shared memory segment in the parent process
        if (munmap(shared_memory_ptr, shared_memory_size) == -1) 
        {
            perror("munmap in parent");
            exit(1);
        }

        // Close and unlink the shared memory segment
        if (close(shared_memory_fd) == -1) 
        {
            perror("close");
            exit(1);
        }
        if (shm_unlink(shared_memory_name) == -1) 
        {
            perror("shm_unlink");
            exit(1);
        }
    }

    return 0;
}
