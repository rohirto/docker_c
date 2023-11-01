/**
 * Telnet chat example from earlier but here we will incorporate a protocol
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


#define PORT "9034" // port we're listening on


/**
 * get sockaddr, IPv4 or IPv6:
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
int recvall(int s, char *buf, int *len)
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

int sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n =0;
    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);      //Can use send or sendto
                        
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


/**
 * Marshal and Unmarshal the data 
 * 
 * Protocol : len (1) + username(8b) + msg (128b)
 * 
 * 0A       74 6F 6D 00 00 00 00 00     48 69
 * (length) T o m    (padding)          H i
 * 
 * And the second is similar:
 * 18       42 65 6E 6A 61 6D 69 6E 48 65 79 20 67 75 79 73 20 77 ...
 * (length) B e n j a m i n          H e y g u y s w ...
*/
int recv_msg(int i, void* buffer )
{
    int packet_len; 
    if(recv(i, &
    packet_len, 1, 0) == -1)  //first byte of packet is len
    {
        //Error
        perror("recv");
        exit(1);
    }

    if(recvall(i,(char*) buffer, &packet_len) == -1)  //read the remaining packet
    {
        //Error
        perror("recv_all");
        exit(1);
    }

    return packet_len;

}

int send_msg(int j, void* buf, int len, const char* username)
{
    char buffer[137];   //Len
    //Make packet as per protocol
    //Add len to the start of packet
    buffer[0] = len;

    //Load the username
    int i = 0;
    //Username is 8 bytes field
    int user_len = strlen(username);
    for( i = 0; i < 8; i++)
    {
        if(i < user_len)
        {
            buffer[i+1] = username[i];
        }
        else
        {
            buffer[i+1] = 0x00;
        }
    }
    //Load up the 128 bytes data field
    memcpy(&buffer[9],(char*) buf, len);

    if(sendall(j, buffer, &len) != 0 )
    {
        perror("talker: sendall");
        exit(1);
        
    }

    return 0;
}

int main(void)
{
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    int fdmax;                          // maximum file descriptor number
    int listener;                       // listening socket descriptor
    int newfd;                          // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    char buf[137]; // buffer for client data   (1+8 + 128 = 137)
    int nbytes;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1; // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;
    struct addrinfo hints, *ai, *p;

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)   //fill up the structs
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);  //Create the socket descriptor
        if (listener < 0)
        {
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        break;
    }
    // if we got here, it means we didn't get bound
    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for (;;)
    {
        read_fds = master; // copy it, need to keep a safe copy of master fds
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)   //we are only interested in readfds
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            { 
                // we got one!!
                if (i == listener)  //Listener is waiting for new connections, and got a new one
                {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);
                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax)
                        { // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                    }
                }
                else
                {
                    // handle data from a client
                    //if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    if ((nbytes = recv_msg(i, (void*) buf)) <= 0)   //Protocol implementation
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
                        // we got some data from a client
                        for (j = 0; j <= fdmax; j++)
                        {
                            // send to everyone!
                            if (FD_ISSET(j, &master))
                            {
                                // except the listener and ourselves
                                if (j != listener && j != i)
                                {
                                    // if (send(j, buf, nbytes, 0) == -1)
                                    // {
                                    //     perror("send");
                                    // }

                                    //Our Protocol will come here
                                    if (send_msg(j, buf, nbytes,"tom") == -1)
                                    {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            }     // END got new incoming connection
        }         // END looping through file descriptors
    }             // END for(;;)--and you thought it would never end!
    return 0;
}