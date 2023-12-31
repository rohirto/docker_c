/**
 * Cetain functions like recv(), recvfrom(), are blocking, that is they just sit idle (sleep), until the data arrive
 * 
 * If you dont want a socket to be blocking, yo have to make call to fcntl()
 * Then we have to poll the socket for information, if no information from socket then it will return -1 and errno will set to EWOULDBLOCK
 * 
 * Generally polling is a bad idea 
 * 
 * Modifying the previous code as below: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>  //Needed to make socket non blocking

#define MYPORT "4950" // the port users will be connecting to
#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) //Fill up our structs
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,  //Get descriptor
                             p->ai_protocol)) == -1)
        {
            perror("listener: socket");
            continue;
        }

        // Make the socket NON BLOCKING
        fcntl(sockfd, F_SETFL, O_NONBLOCK);

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)  //Bind all the nodes in linked list
        {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo); //No need of this struct now

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    while ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,         //No listen and connect
                             (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        // We are polling here
        //But this loop consumes CPU Time and is therefore non efficent
    }
    printf("listener: got packet from %s\n",
           inet_ntop(their_addr.ss_family,
                     get_in_addr((struct sockaddr *)&their_addr),
                     s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    close(sockfd);
    return 0;
}