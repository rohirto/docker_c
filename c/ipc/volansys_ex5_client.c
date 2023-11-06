/**
 * Client Code, continuing from previous example
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>

#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT "9034"    // Replace with the server's port

#define FILE_LIST_PACKET    0x01
#define FILE_LIST_ACK       0x02


struct FileInfo
{
    char name[256];
    off_t size;
    time_t creation_time;
};

/**
 * get sockaddr, IPv4 or IPv6:
 * Used to fill up the structs needed to get sockets going
 * */ 
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/**
 * To reliably receive data over sock stream
*/
int recvall(int s, unsigned char *buf, int *len)
{
    int total = 0;        // how many bytes we've received
    int bytesleft = *len; // how many bytes we have left to receive
    int n = 0;
    while (total < *len)
    {
        n = recv(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            // return the number actually received here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

int recieve_file_list()
{

    return 0;
}

int recieve_packet(int s)
{
    unsigned char buffer[1024];
    unsigned char len;
    int status;
    unsigned char packet_type;
    static int file_no = 0;

    if((status = recv(s,&packet_type,1,0)) == -1)  //first byte of packet is packet type, this is our header
    {
        //Error
        perror("recv");
        exit(1);
    }
    else if (status == 0)
    {
        return 0;  //Server hung up you
    }

    switch (packet_type)
    {
    case FILE_LIST_PACKET:  //File list packet
        if ((status = recv(s, &len, 1, 0)) == -1)
        {
            // Error
            perror("recv");
            exit(1);
        }
        else if (status == 0)
        {
            return 0; // Server hung up you
        }
        int intPacketLen = (int)len;                   // Cast unsigned char to int
        if (recvall(s, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
        {
            // Error
            perror("recv_all");
            exit(1);
        }
        //Print buffer
        struct FileInfo fileInfo;
        unpack(buffer,"256sll",fileInfo.name,&fileInfo.size,&fileInfo.creation_time);
        printf("%d.Name: %s Size: %ld bytes, Creation Time: %s\n", file_no++,fileInfo.name,fileInfo.size,ctime(&fileInfo.creation_time));
        fflush(stdout);

        return FILE_LIST_ACK;
        break;

    default:
        return 0;
        break;
    }

    return 1;
}



int client_handle()
{
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    int fdmax;                          // maximum file descriptor number
    int fd = fileno(stdin);
    int sockfd;
    int rv;
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];
    int nbytes;

    int file_count;

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);

    //filling up the structs
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

     if ((rv = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &servinfo)) != 0) //Fill up our structs using getaddrinfo function
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,  //Get descriptor
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) //Connect to the nodes in linked list, ipv4 or ipv6
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }


    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure

    printf("Connected to the server.\n");
    /**** Successfully connected to Server till this point *********/

    //Load the file descriptors into master list
    FD_SET(sockfd, &master);  //Socket descriptor
    fdmax = sockfd;

     /********** WHILE 1 LOOP **********************/
    while (1) 
    {
        read_fds = master;  //Save a copy of master
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)   //we will need both read and write functions
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))  //If any of the read_fds is set
            {
                if (i == sockfd)  //If receive on socketfd
                {
                    // Yu have a message
                    if ((nbytes = recieve_packet(i)) <= 0) // Protocol implementation, nbytes received here will be one which we posted in packet
                    {
                        // got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);           // bye!
                        FD_CLR(i, &master); // remove from master set
                    }
                    else
                    {
                        //Handle response to sucessful packet
                        switch (nbytes)
                        {
                        case FILE_LIST_ACK:
                            file_count++;
                            break;
                        
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int main() 
{
    if(client_handle() == -1)
    {
        //error has occured
    }

    return 0;
}
