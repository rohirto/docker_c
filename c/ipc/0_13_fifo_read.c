/**
 * Continued from previous example,
 * Read the FIFO which was written in the previous example
 * Low level file functions are used here (open, close etc)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() 
{
    int fd;
    char message[100];
    
    // Open the named pipe for reading
    if ((fd = open("myfifo", O_RDONLY)) < 0) 
    {
        perror("open");
        exit(1);
    }
    
    // Read the message from the FIFO
    read(fd, message, sizeof(message));
    
    // Display the received message
    printf("Received: %s\n", message);
    
    // Close the file descriptor
    close(fd);
    
    return 0;
}
