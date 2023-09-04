//Client code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345); // Port number
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (loopback in this example)

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Receive and print server's response
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Server says: %s\n", buffer);

    // Close the socket
    close(client_socket);

    return 0;
}