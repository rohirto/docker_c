/**
 * FIFOs are IPC mechanism like Pipes, but even unrelated processes can use them to communicate data
 * FIFOs are also called as named pipes
 * Fifos can have multiple reader and multipe writers
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() 
{
    // Create a named pipe (FIFO) with a specific name, e.g., "myfifo"
    if (mkfifo("myfifo", 0666) == -1) 
    {
        perror("mkfifo");
        exit(1);
    }
    return 0;
}

