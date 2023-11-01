/**
 * send() returns the no of bytes actually sent, suppose you intended to send 512 bytes and send() returned 412 bytes. Remaining 100 bytes still in the 
 * buffer there to be sent.
 * 
 * Below code is method to handle such scenarios
 * 
 * 
 * Now you have sent a packet/packets which is/are not limited by length 
 * 
 * But then how to handle such big and variable packets from the server/reciever side??
 * See in next code 0_17_data_encapsulation -> Have your data encapsulated in a header mentioning the length of data, 
 * In short create a protocol
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

#define SERVERPORT "4950" // the port users will be connecting to


int sendall(int s, char *buf, int *len, const struct addrinfo *p)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n =0;
    while (total < *len)
    {
        n = sendto(s, buf + total, bytesleft, 0,        //Can use send or sendto
                        p->ai_addr, p->ai_addrlen);  
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            // return number actually sent here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    //int numbytes;

    if (argc != 3)
    {
        fprintf(stderr, "usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) //Fill up our struct
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)  //Get the descriptor
        {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    // if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,  //Just need this
    //                        p->ai_addr, p->ai_addrlen)) == -1)
    // {
    //     perror("talker: sendto");
    //     exit(1);
    // }

    // New method of sendall to handle bigger buffers as well
    int len = strlen(argv[2]);
    if(sendall(sockfd, argv[2], &len, p) != 0 )
    {
        perror("talker: sendall");
        exit(1);
        
    }

    freeaddrinfo(servinfo);
    printf("talker: All the bytes: %zu",strlen(argv[2]));
    close(sockfd);
    return 0;
}