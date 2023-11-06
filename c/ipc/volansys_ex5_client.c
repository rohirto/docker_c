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


volatile bool send_file_req_flag = false;
struct FileInfo file_list[1024];
unsigned int  file_count = 0;
unsigned int file_no = 0;


void print_files(int file_count, struct FileInfo* file_list) 
{
    printf("List of files in the directory:\n");
    for (int i = 0; i < file_count; i++) 
    {
        printf("Name: %s, Size: %ld bytes, Creation Time: %s", file_list[i].name, file_list[i].size, ctime(&file_list[i].creation_time));
    }
    printf("Total files: %d\n", file_count);
}

int receiveFileOverSocket(int socket, const char* filePath) 
{
    FILE* file = fopen(filePath, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return -1;
    }

    printf("\nRecieve in progress.. ");
    fflush(stdout);
    char buffer[1024];
    unsigned char bytesRead = -1;

    // while ((bytesRead = recv(socket, buffer, sizeof(buffer), 0)) > 0) 
    // {
    //     fprintf(stderr,"...");
    //     if (fwrite(buffer, 1, bytesRead, file) < bytesRead) 
    //     {
    //         perror("Error writing to file");
    //         fclose(file);
    //         return -1;
    //     }
    // }

    while(1)
    {
        int n = recv(socket, buffer, 1024, 0);
        bytesRead +=n;
        fprintf(file, "%s", buffer);
        if(n<=0)
        {
            break;
        }
        
        bzero(buffer, 1024);
    }

    // if (bytesRead < 0) {
    //     perror("Error receiving file");
    //     fclose(file);
    //     return -1;
    // }
    
    fclose(file);
    printf("\n File recieved Successfully!");
    fflush(stdout);
    return 0;
}

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
 * To reliably send data over sock stream
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
    static int file_n = 0;
    

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
        // printf("%d.Name: %s Size: %ld bytes, Creation Time: %s\n", file_no,fileInfo.name,fileInfo.size,ctime(&fileInfo.creation_time));
        // fflush(stdout);
        //Add to the file_list data structure
        file_list[file_n++] = fileInfo;
        

        return FILE_LIST_ACK;
        break;
    case FILE_PACKET:
        //Start receiving a file
        if(receiveFileOverSocket(s,file_list[file_no].name) !=0)
        {
            //errorr
            perror("File Receive");
            return -1;
        }
        return FILE_ACK;
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
                        case FILE_ACK:
                        printf("\nfile received!\n");
                            break;
                        
                        default:
                            break;
                        }
                    }
                }
                else if(i == fd)  //stdin, every time if something is available on stdin
                {
                    switch (a)
                    {
                    case 0:
                        // Get the File list by just pressing enter
                        fgets(buff, sizeof(buff), stdin);
                        buff[strcspn(buff, "\n")] = '\0'; // remove the new line
                        print_files(file_count, file_list);
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
                        a = 1;
                        break;
                    case 1:
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

                        //valid = false;
                        send_file_req_flag = true;

                        a = 2;
                        break;
                    case 2:
                        // You sent the file no to server
                        printf("\nWait! processing your request\n");
                        a = 3;
                        break;

                    default:
                        break;
                    }
                }
            }
            if (FD_ISSET(i, &write_fds))
            {
                 // Socket is ready for writing
                 if(send_file_req_flag == true)
                 {
                    unsigned char tmp[3];
                    int t_len = 2;
                    packi16(tmp,file_no);
                    if(sendall(sockfd,tmp,&t_len) == -1)
                    {
                        perror("sendall");
                    }
                    printf("\nSending request for File no: %u Name: %s", file_no,file_list[file_no].name);
                    fflush(stdout);
                    valid = false;
                    send_file_req_flag = false;

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
