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
#define FILE_PACKET         0x03
#define FILE_ACK            0x04




struct FileInfo
{
    char name[256];
    off_t size;
    time_t creation_time;
};


struct FileInfo file_list[1024];
unsigned int  file_count = 0;
unsigned int file_no = 0;

/**
 * @brief Print the files in the directory
 * @param int file_count, no of file in directory
 * @param struct FileInfo* file_list Pointer to struct holding File info
 * 
*/
void print_files(int file_count, struct FileInfo* file_list) 
{
    printf("List of files in the directory:\n");
    for (int i = 0; i < file_count; i++) 
    {
        printf("Name: %s, Size: %ld bytes, Creation Time: %s", file_list[i].name, file_list[i].size, ctime(&file_list[i].creation_time));
    }
    printf("Total files: %d\n", file_count);
}

int write_all(FILE *file, const void *buf, int len)
{
    const char *pbuf = (const char *) buf;
    while (len > 0)
    {
        int written = fwrite(pbuf, 1, len, file);
        if (written < 1)
        {
            printf("Can't write to file");
            return -1;
        }
        pbuf += written;
        len -= written;
    }
    return 0;
}

int receiveFileOverSocket(int socket, const char* filePath) 
{
    int rval;
    FILE* file = fopen(filePath, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return -1;
    }

    printf("\nRecieve in progress.. ");
    fflush(stdout);
    char buf[1024];
    unsigned char bytesRead = -1;
    long size = 0;
    int len_to_rx = sizeof(size);
    if (recvall(socket, &size, &len_to_rx) == 0)
    {
        size = ntohl(size);
        
        while (size > 0)
        {
            len_to_rx = min(sizeof(buf), size);
            if (recvall(socket, buf, &len_to_rx) == 0)
            {
                rval = len_to_rx;
                if (rval == 0)
                    break;
                if (write_all(file, buf, rval) == -1)
                    break;
                size = size - rval;
            }
            else
            {
                perror("recvall");
                fclose(file);
                return -1;
            }

        } 
    }
    else
    {
        perror("recvall");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    printf("\n File recieved Successfully!");
    fflush(stdout);
    return 0;
}

/**
 * @brief get sockaddr, IPv4 or IPv6:
 **/ 
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/**
 * @brief send all the bytes of len to socket 
 * @param int s - socket descriptor
 * @param unsigned char *buf - buffer to be sent
 * @param int* len - len to be sent, also how many bytes were actually sent is updated here
 * @returns -1 on failure or 0 on success
 * 
 * @paragraph - to check if socket is closed on other side, just check the *len, if it is zero then connection was closed
*/
int sendall(int s, unsigned char *buf, int *len)
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

/**
 * @brief receive all the bytes of len to socket 
 * @param int s - socket descriptor
 * @param unsigned char *buf - buffer to be rx
 * @param int* len - len to be rx, also how many bytes were actually rx is updated here
 * @returns -1 on failure or 0 on success
 * 
 * @paragraph - to check if socket is closed on other side, just check the *len, if it is zero then connection was closed
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


int client_handle()
{
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;                          // maximum file descriptor number
    int fd = fileno(stdin);
    int sockfd;
    int rv;
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];
    int nbytes;
    unsigned char buff[20];
    
   
    bool valid = false;
    static unsigned int a = 0;
    static unsigned int state = 0;

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

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
    state = 1;
    /**** Successfully connected to Server till this point *********/

    //Load the file descriptors into master list
    FD_SET(sockfd, &master);  //Socket descriptor
    FD_SET(fd, &master);      //stdin descriptor
    if(sockfd > fd)         //Check which one is fdmax
    {
        fdmax = sockfd;
    }
    else
    {
        fdmax = fd;
    }

    //User Interaction
    printf("****** File Transfer Client ************************************\n");
    printf("AFTER 5 sec press Enter to get file_list\n");
     /********** WHILE 1 LOOP **********************/
    while (1) 
    {
        read_fds = master;  //Save a copy of master
        write_fds = master; ////Save a copy of master
        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)   //we will need both read and write functions
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds)) // If any of the read_fds is set
            {
                unsigned char buffer[1024];
                if (i == sockfd) // If receive on socketfd
                {
                    switch (state)
                    {
                    case 1:
                        int status;
                        unsigned char len, packet_type;
                        static int file_n = 0;
                        if ((status = recv(i, &packet_type, 1, 0)) == -1) // first byte of packet is packet type, this is our header
                        {
                            // Error
                            perror("recv");
                            return -1;
                        }
                        else if (status == 0)
                        {
                            printf("selectserver: socket %d hung up\n", i);
                            state = 0;
                            close(i);           // bye!
                            FD_CLR(i, &master); // remove from master set
                        }
                        // Analyze the packet header
                        switch (packet_type)
                        {
                        case 0x01:    // File info packet
                            if ((status = recv(i, &len, 1, 0)) == -1) // Receive len
                            {
                                // Error
                                perror("recv");
                                return -1;
                            }
                            else if (status == 0)
                            {
                                printf("selectserver: socket %d hung up\n", i);
                                state = 0; // Server hung up you
                                close(i);           // bye!
                                FD_CLR(i, &master); // remove from master set
                            }
                            int intPacketLen = (int)len;                 // Cast unsigned char to int
                            if (recvall(i, buffer, &intPacketLen) == -1) // read the remaining packet, username and message
                            {
                                // Error
                                perror("recv_all");
                                return -1;
                            }
                            // Print buffer
                            struct FileInfo fileInfo;
                            unpack(buffer, "256sll", fileInfo.name, &fileInfo.size, &fileInfo.creation_time);
                            // printf("%d.Name: %s Size: %ld bytes, Creation Time: %s\n", file_no,fileInfo.name,fileInfo.size,ctime(&fileInfo.creation_time));
                            // fflush(stdout);
                            // Add to the file_list data structure
                            file_list[file_n++] = fileInfo;
                            file_count = file_n;
                            break;
                        case 0xBF:     // File Info end
                            print_files(file_count, file_list);
                            state = 2; // File dir ready with us, prompt user to send file no to server on stdin
                            break;

                        default:
                            break;
                        }

                        break;

                    case 5: // expect a file from server
                        if (receiveFileOverSocket(i, file_list[file_no].name) != 0)
                        {
                            // errorr
                            perror("File Receive");
                            return -1;
                        }
                        printf("Receive Successful!");
                        state = 2;  //File receive cycle completed, clean up and go back to receiving file again
                        break;

                    default:
                        break;
                    }
                }
                else if(i == fd)  //stdin, every time if something is available on stdin
                {
                    switch (state)
                    {
                    case 2:
                        // Get the File list by just pressing enter
                        fgets(buff, sizeof(buff), stdin);
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line
                        //print_files(file_count, file_list);
                        if (file_count > 0)
                        {
                            // Now you can send to server the request file no
                        }
                        else
                        {
                            perror("File count");
                            fprintf(stderr, "File list still not got");
                        }
                        printf("\n\n Enter File number to get: ");
                        fflush(stdout);
                        state = 3;  //Printed the file list now, get the  file no
                        break;
                    case 3:   //After printing files get a valid file no
                        // Get File name
                        while (!(valid))
                        {
                           
                            // memset the File no
                            memset(buff, 0x00, sizeof(buff));

                            fgets(buff, sizeof(buff), stdin);

                            if ((file_no = atoi(buff)) != 0)
                            {
                                if(file_no < file_count)
                                {
                                    // Correct value
                                    valid = true;
                                }
                            }
                            else
                            {
                                printf("Enter Correct Vaule!");
                                fflush(stdout);
                            }
                        }
                        valid = false;
                        state = 4;  //Go to next state, send the file no to Server

                        break;

                    default:
                        break;
                    }
                }
            }
            if (FD_ISSET(i, &write_fds))
            {
                if( i == sockfd)
                {
                    switch (state)
                    {
                    case 4:  //Send the file no to server
                        unsigned char tmp[3];
                        int t_len = 2;
                        packi16(tmp, file_no);
                        if (sendall(i, tmp, &t_len) == -1)
                        {
                            perror("sendall");
                            return -1;
                        }
                        printf("\nSending request for File no: %u Name: %s", file_no, file_list[file_no].name);
                        fflush(stdout);
                        state = 5; //Expect the data from server
                        break;
                    
                    default:
                        break;
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
