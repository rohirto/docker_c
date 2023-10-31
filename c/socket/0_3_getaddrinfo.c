/**
 * getaddrinfo() -> helps to set up the structs you need later on
 * 
 * You give this function three input parameters, and it gives you a pointer to a linked-list, res, of results (ipv4, ipv6 params in the node of list)
 * 
 * Gives much more info than gethostbyname() function
 * 
 * int getaddrinfo(const char *node, // e.g. "www.example.com" or IP
 *                 const char *service, // e.g. "http" or port number
 *                  const struct addrinfo *hints,
 *                  struct addrinfo **res);
 * 
 * AI_PASSIVE: AI_PASSIVE is used when setting up a server to indicate that the resulting socket address structure is intended 
 *             for binding the server socket to listen for incoming connections. It's a way to handle the details of server socket setup 
 *             without needing to specify a specific IP address and port.
 * 
 * All ports below 1024 are RESERVED (unless you're the superuser)! You can have any port number above that,
 * right up to 65535 (provided they aren't already being used by another program.)
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;       // will point to the results
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    // servinfo now points to a linked list of 1 or more struct addrinfos
    // ... do everything until you don't need servinfo anymore ....
    freeaddrinfo(servinfo); // free the linked-list

    return 0;
}