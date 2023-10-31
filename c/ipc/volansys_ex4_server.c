/**
 * For each exercise select whether you will use connection-oriented or connectionless socket with reason for your choice. 
 * By default server should serve multiple-clients at a time
 * Develop a client server application, wherein client application sends message to server application from STDIN, server echoes’ 
 * back client Message. Client should display this server response on STDOUT with server processing time. Use special keyword “quit” 
 * to close both applications gracefully. NOTE: Message processing time = Message Rx time + Server Processing time + Message Tx Time 
 * 
 * 
 * Connection less socket : SOCK_DGRAM
 * Connection Oriented Socket: SOCK_STREAM
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

#define PORT 12345
#define BUFFER_SIZE 1024

int main() 
{
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);   /* htons host to network short -convert the numbers to Network Byte Order before they go out on the wire, and
                                            convert them to Host Byte Order as they come in off the wire */
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) //Can serve 5 connections simultenously
    {
        perror("Listening failed");
        close(server_socket);
        exit(1);
    }
    
    printf("Server is listening on port %d...\n", PORT);
    
    while (1) 
    {   
        // Accept a connection from the client
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        
        if (client_socket == -1) 
        {
            perror("Accepting connection failed");
            continue;
        }
        // Calculate msg rx time from here
        time_t start_time = time(NULL);
        // Receive data from the client
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) 
        {
            perror("Receiving data failed");
            close(client_socket);
            continue;
        }
        
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        
        if (strcmp(buffer, "quit") == 0) 
        {
            printf("Client has requested to quit. Closing the server...\n");
            close(client_socket);
            break;
        }
        
        // Calculate processing time
        time_t end_time = time(NULL);

        long time_process = end_time - start_time; 
        
        
        // Send the response back to the client
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Server response: %s (Processing time: %ld)", buffer, time_process);

        //Time ends here

        send(client_socket, response, strlen(response), 0);
        
        // Close the client socket
        close(client_socket);
    }
    
    // Close the server socket
    close(server_socket);
    
    return 0;
}
