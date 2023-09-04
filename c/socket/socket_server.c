#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345); // Port number
    server_address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for connections...\n");

    // Accept incoming connections
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
    if (client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Send a message to the client
    const char *message = "Hello, Client!";
    send(client_socket, message, strlen(message), 0);

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}