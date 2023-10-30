/**
 * Continued From the Server Code, see the server code for Problem statement
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() 
{
    
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
     while (1) 
    {
        int client_socket;
        // Create a socket
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Socket creation failed");
            exit(1);
        }

        // Initialize server address structure
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        // Connect to the server
        if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("Connection to server failed");
            close(client_socket);
            exit(1);
        }

        printf("Enter a message (type 'quit' to exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Remove the trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "quit") == 0)
        {
            printf("Client has requested to quit. Closing the client...\n");
            break;
        }

        // Receive and display the server response
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0)
        {
            perror("Receiving data from server failed");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Server Response: %s\n", buffer);

        // Close the client socket
        close(client_socket);
    }
    
    
    
    return 0;
}
