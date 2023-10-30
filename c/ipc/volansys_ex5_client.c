/**
 * Client Code, continuing from previous example
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

int main() 
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    // Implement client functionality here

    close(clientSocket);

    return 0;
}
