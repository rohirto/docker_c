#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT "9034"    // Replace with the server's port


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
 * To reliably send data over sock stream
*/
int sendall_stream(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n =0;
    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);  
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

int recvall_stream(int s, char *buf, int *len)
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
    if(recv(i, &packet_len, 1, 0) == -1)  //first byte of packet is len
    {
        //Error
        perror("recv");
        exit(1);
    }

    if(recvall_stream(i,(char*) buffer, &packet_len) == -1)  //read the remaining packet
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

    if(sendall_stream(j, buffer, &len) != 0 )
    {
        perror("talker: sendall");
        exit(1);
        
    }

    return 0;
}


int main(void) {
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;                          // maximum file descriptor number
    int sockfd;
    char buf[137]; // buffer for client data   (1+8 + 128 = 137)
    int nbytes;
    char message[128]; // Assuming a message length of 128 characters
    int rv;
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

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

    printf("Connected to the server. Start typing messages:\n");


    FD_SET(sockfd, &master);
    fdmax = sockfd;

    while (1) {
        read_fds = master;
        write_fds = master;
        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)   //
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                // Yu have a message
                if ((nbytes = recv_msg(i, (void *)buf)) <= 0) // Protocol implementation
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
                    // we got some data from server
                    printf("%s",buf);
                    
                }
            }
            if (FD_ISSET(i, &write_fds))
            {
                // Socket is ready for writing
                // Read user input
                //printf("You: ");
                fgets(message, sizeof(message), stdin);

                // Send the message to the server
                // if (send(sockfd, message, strlen(message), 0) == -1) {
                if (send_msg(sockfd, message, strlen(message), "rohirto") == -1)
                    {
                        perror("send");
                        close(sockfd);
                        exit(3);
                    }
               
            }


        }
    }

    close(sockfd);
    return 0;
}
