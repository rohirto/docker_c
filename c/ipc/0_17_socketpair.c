/**
 * The socketpair function in C is used to create a pair of connected sockets that can be used for inter-process communication (IPC) between two processes. 
 * Here's a simple example of how to use socketpair:
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>

int main() 
{
    int sockets[2];  //Like pipes file descriptor
    

    // Create a socket pair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) 
    {
        perror("Error creating socket pair");
        exit(1);
    }

    // Fork a child process
    pid_t child_pid = fork();

    if (child_pid == -1) 
    {
        perror("Error forking process");
        exit(1);
    }

    if (child_pid == 0) 
    {
        char buffer[256];
        // This is the child process
        close(sockets[0]);  // Close the parent's end of the socket

        // Child process reads from the socket
        ssize_t bytes_read = read(sockets[1], buffer, sizeof(buffer));
        if (bytes_read == -1) 
        {
            perror("Error reading from socket");
            exit(1);
        }

        buffer[bytes_read] = '\0';
        printf("Child received: %s", buffer);

        close(sockets[1]);  // Close the child's end of the socket
    } 
    else 
    {
        // This is the parent process
        close(sockets[1]);  // Close the child's end of the socket

        // Parent process writes to the socket
        const char* message = "Hello from the parent!";
        ssize_t bytes_written = write(sockets[0], message, strlen(message));
        if (bytes_written == -1) 
        {
            perror("Error writing to socket");
            exit(1);
        }

        close(sockets[0]);  // Close the parent's end of the socket

        pthread_join(child_pid, NULL); //Wait for child process to exit
    }

    return 0;
}
