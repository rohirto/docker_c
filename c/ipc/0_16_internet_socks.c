/**
 * Internet Sockets allow for Communication between the Processes for different systems
 * 
 * Internet Namespace - PF_INET  (AF_INET)
 * 
 * Most Common Protocol - TCP/IP
 * 
 * IP - low level protocol to move packets through the internet, splitting and rejoining the packets if necessary.
 *      Only best effort delivery - so packets may be lost or reordered during transport.
 *      Every computer has a unique IP number (IP Address)
 * 
 * TCP - Transmission Control Protocol, layered on top of IP, provides an reliable connection-ordered transport
 *      It permits telephone like connections ot be established between computers and ensures that data is delivered reliably and in order
 * 
 * Internet Socket address contains 2 part - A machine and a port no
 * 
 * sockaddr_in (Internet Domain Sockets - IPv4) vs sockaddr_un (Unix Domain Socket): Former used for communication over IPv4, latter used for local Communication 
 * 
 * Below code opens a Internet Socket and does a HTTP get
 * How to run the code after building 
 * $ ./0_16_internet_socks.out "www.google.com"
*/

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#if 0
/* Print the contents of the home page for the server’s socket.
Return an indication of success. */
void get_home_page(int socket_fd)
 {
    char buffer[10000];
    ssize_t number_characters_read;
    
    /* Send the HTTP GET command for the home page. */
    sprintf(buffer, "GET /\r\n");
    write(socket_fd, buffer, strlen(buffer));
    
    /* Read from the socket. The call to read may not
       return all the data at one time, so keep trying until we run out. */
    while (1) 
    {
        number_characters_read = read(socket_fd, buffer, 10000);
        if (number_characters_read == 0)
            return;
        /* Write the data to standard output. */
        fwrite(buffer, sizeof(char), number_characters_read, stdout);
    }
}

int main(int argc, char* const argv[]) 
{
    int socket_fd;
    struct sockaddr_in name;
    struct hostent* hostinfo;

    /* Create the socket. */
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    /* Store the server’s name in the socket address. */
    name.sin_family = AF_INET;

    /* Convert from strings to numbers. */
    hostinfo = gethostbyname(argv[1]);
    if (hostinfo == NULL)
        return 1;
    else
        name.sin_addr = *((struct in_addr*)hostinfo->h_addr);

    /* Web servers use port 80. */
    name.sin_port = htons(443);

    /* Connect to the Web server */
    if (connect(socket_fd, (struct sockaddr*)&name, sizeof(struct sockaddr_in)) == -1) 
    {
        perror("connect");
        return 1;
    }

    /* Retrieve the server’s home page. */
    get_home_page(socket_fd);

    return 0;
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Function to send an HTTP GET request and receive the response
void http_get(const char *host, const char *path) {
    struct hostent *server;
    struct sockaddr_in server_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(80);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    char request[1024];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);

    if (write(sockfd, request, strlen(request)) < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    char response[8192];
    int total_bytes = 0;
    int n;

    while ((n = read(sockfd, response + total_bytes, sizeof(response) - total_bytes)) > 0) {
        total_bytes += n;
    }

    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    response[total_bytes] = '\0';
    printf("%s", response);

    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    char *url = argv[1];
    char *host, *path;

    // Separate the host and path from the URL
    if (strstr(url, "http://") != NULL) {
        url += 7; // Skip "http://"
    }

    host = strtok(url, "/");
    path = strtok(NULL, "");

    if (path == NULL) {
        path = "/";
    }

    http_get(host, path);

    return 0;
}
