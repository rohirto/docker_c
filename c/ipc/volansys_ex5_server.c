/**
 * For each exercise select whether you will use connection-oriented or connectionless socket with reason for your choice. 
 * By default server should serve multiple-clients at a time
 * 
 * 
 * Develop a simple file-transfer client server application with following functionalities (20 points):
 *      1. Server on-startup should scan pre-defined directory and create list of files.
 *      2. As soon as server can accept client connections it should on demand present the file list to client.
 *      3. Server on demand should transfer the selected file to client as soon as possible.
 *      4. If demanded, Server should be able to provide file information (metadata) like size, type, creation date to client.
 *      5. Server should have idle timeout of 120 seconds, i.e. if any client has no activity till specified timeout, server should terminate the connection with that client.
 *      6. Client on start-up or as demanded by user should fetch file list from server.
 *      7. As per file selected by user, client should ask server to transfer that file.
 *      8. If demanded by user, client should fetch information of file from server.
 * 
 * Connection Oriented Approach needed here: 
 * 
 * Can use threads for concurrency on server side
 * Go step wise
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

//Defines
#define PORT "9034" // port we're listening on

#define TIMEOUT_SECONDS 120  //Time out of 120 seconds
#define TIMEOUT_MICROSECONDS 0

//Global Variables
const char* dir_path = "../ipc/";
volatile bool error_flag = false;  //Flag to indicate Error has occured or not
volatile int send_file_flag = -1;   //Flag to indicate if to send a file or not, -1 when on file to send, value greater than zero to indicate file no to send

/**
 * @brief Struct to store the Metadata of file 
*/
struct FileInfo 
{
    char name[256];
    off_t size;
    time_t creation_time;
};



/** 
 * @brief Error Handling
*/
void cleanup(struct FileInfo* file_list, int file_count) 
{
    for (int i = 0; i < file_count; i++) 
    {
        // Free any resources within the FileInfo structure, if needed
    }
    free(file_list);
    exit(0);
}

/**
 * @brief SIGINT Handler
*/
void sigint_handler(int signum) 
{
    printf("Received SIGINT (Ctrl+C)\n");
    // Handle the SIGINT signal, e.g., perform cleanup or take specific actions.
    error_flag = true;
    
}


/* 2. As soon as server can accept client connections it should on demand present the file list to client.*/

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
int sendall(int s, void *buf, int *len)
{
    unsigned char* buff = (unsigned char*) buf;
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n =0;
    while (total < *len)
    {
        n = send(s, buff + total, bytesleft, 0);      //Can use send or sendto
                        
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


/**
 * @brief check if the client is still connected or not
 * @param int socket - file descriptor of the client socket 
 * @returns -1 on closed socket, 1 on active socket
*/
int check_connection(int socket)
{
    char data;
    if(recv(socket,&data,1, MSG_PEEK) == 0) //read one byte
    {
        // Socket closed
        perror("recvall");
        fprintf(stderr,"selectserver: socket %d hung up\n", socket);
        return -1;
    }
    return 1;
}
/**
 * @brief Send File List to the client
 * @param int s - socket descriptor
 * @param struct FileInfo* file_list - starting pointer to the data structure that holds the file dir info
 * @param int file count - No of elements in the directory
 * @returns -1 on failure or 1 on success
 * 
 * @paragraph - to check if socket is closed on other side, just check the *len, if it is zero then connection was closed
*/
int send_file_list(int s,struct FileInfo* file_list, int file_count)
{
    unsigned char bin_data[1024];
    unsigned int len_to_send = 0;
    printf("Converting File List into binary:\n");
    for (int i = 0; i < file_count; i++) 
    {
        memset(bin_data, 0x00, 1024);
        printf("Name: %s, Size: %ld bytes, Creation Time: %s\n", file_list[i].name, file_list[i].size, ctime(&file_list[i].creation_time));
        //pack the data: 
        len_to_send = pack(bin_data+2,"sll",file_list[i].name,file_list[i].size, file_list[i].creation_time);
        bin_data[0] = 0x01;         //To signify file list packet
        bin_data[1] = (unsigned char) len_to_send;  //To signify len of file list packet excluding starting 2 bytes
        len_to_send = len_to_send + 2;
        if(sendall(s,bin_data,&len_to_send) == -1)
        {
            return -1;
        }
    }
    printf("Total files converted: %d Total bytes sent for last file:%u\n", file_count, len_to_send);
    fflush(stdout);
    printf("Sending Ending packet\n");
    unsigned char c = 0xBF; // End packet
    if (send(s, &c, 1, 0) == -1)
    {
        return -1;
    }

    return 1;
}

/**
 * @brief Send a File over socket
 * @param int socket - File Descriptor of the socket
 * @param const char* filepath - filename along with file path of the file to be sent
 * @returns -1 on failure, 0 on success
*/
int sendFileOverSocket(int socket, const char* filePath) 
{
    struct stat s;
    if (stat(filePath, &s) == -1)
    {
        printf("Can't get file info"); 
        return -1;
    }
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) 
    {
        perror("File not found");
        return -1;
    }


    long size = s.st_size;
    long tmp_size = htonl(size);
    int len_to_tx = sizeof(tmp_size);
    if (sendall(socket, &tmp_size, &len_to_tx) == 0)  //Need to see how to send data over non blocking sockets
    {
        while (size > 0)
        { 
            char buf[1024];
            int rval = fread(buf, 1, min(sizeof(buf), size), file); 
            if (rval < 1)
            {
                printf("Can't read from file");
                break;
            }
            len_to_tx = rval;
            if (sendall(socket, buf, &len_to_tx) == -1)
                break;
            size -= rval;
        }
    }
    printf("\nFile Sent Successfully!\n");
    fclose(file);
    send_file_flag = -1;
    return 0;
}

/**
 * @brief Main Server Handler
 * @param struct FileInfo* file_list - starting pointer to the data structure that holds the file dir info
 * @param int file count - No of elements in the directory
 * @returns -1 on failure, returns only on failure
*/
int server_handle(struct FileInfo* fileInfo, int file_count)
{
    /***** LOCAL VARIABLES *******************************/
    fd_set master;                      // master file descriptor list
    fd_set read_fds;                    // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;                          // maximum file descriptor number
    int listener;                       // listening socket descriptor
    int newfd;                          // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1; // for setsockopt() SO_REUSEADDR, below
    int i, rv;
    struct addrinfo hints, *ai, *p;

    static unsigned int state = 0;  //State of the server

    struct timeval timeout;       //Set timout for 120 seconds
    timeout.tv_sec = 120;
    timeout.tv_usec = 0;
    time_t last_activity_time[10]; // Array to store the last activity time for each client

    /********************************************************/

    //Macros for select()
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)   //fill up the structs
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        //exit(1);
        //error_flag = true;
        return -1;
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
        //exit(2);
        //error_flag = true;
        return -1;
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        //exit(3);
        //error_flag = true;
        return -1;
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one
    // main loop
    for (;;)
    {
        read_fds = master; // copy it, need to keep a safe copy of master fds
        write_fds = master; ////Save a copy of master
        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)   //we are only interested in readfds
        {
            perror("select");
            //exit(4);
            //error_flag = true;
            return -1;
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
                        
                        //Set the start time
                        last_activity_time[newfd] = time(NULL);
                        
                        //Writes to socket to be handled by writefds
                        state = 1; //Now we can send file list to client
                    }
                }
                else
                {
                    switch (state)
                    {
                    case 2:   //Expect a file no from user
                        unsigned char tmp[3];
                        int t_len = 2;
                        //Check if client still connected or not
                        if(check_connection(i) == -1)
                        {
                            //Client hung up
                            perror("recvall");
                            close(i);           // bye!
                            FD_CLR(i, &master); // remove from master set
                            fprintf(stderr,"selectserver: socket %d hung up\n", i);
                            state = 0;
                            break;
                        }
                        if(recvall(i,tmp,&t_len) == -1)
                        {
                            perror("recvall");
                            close(i);           // bye!
                            FD_CLR(i, &master); // remove from master set
                            fprintf(stderr,"selectserver: socket %d hung up\n", i);
                            state = 0;
                            break;
                            
                        }
                        int file_no = unpacki16(tmp);

                         

                        send_file_flag = file_no;

                        printf("\n Received Req for File no:%d\n", file_no);
                        fflush(stdout);

                        state = 3;  //Go to next state, write the file to socket
                        break;

                    case 4: //Do the clean up
                            printf("\n File Cycle over, starting again\n");
                            state = 2;
                        break;
                    
                    default:
                        break;
                    }

                    //Update the last time
                    last_activity_time[i] = time(NULL);
                } // END handle data from client
            }     // END got new incoming connection
            if (FD_ISSET(i, &write_fds))
            {
                if (i == newfd)
                {
                    switch (state)
                    {
                    case 1: //ready to send the file list to client
                        if(send_file_list(i,fileInfo,file_count) == -1)
                        {
                            perror("send file list");
                        }
                        state = 2; //Next state, expect a file no from client
                        break;
                    case 3: //Send the file over socket
                        if (send_file_flag > -1)
                        {
                            char path[256];
                            sprintf(path, "%s%s", dir_path, fileInfo[send_file_flag].name);
                            printf("Sending File: %s\n", path);
                            fflush(stdout);
                            if (sendFileOverSocket(i, path) == -1)
                            {
                                perror("file send");
                                fprintf(stderr, "File Send Failed for file no: %d", send_file_flag);
                            }
                        }
                        state = 4;  //Goto next state, clean up and start over again
                        break;

                    default:
                        break;
                    }
                }
            }
        } // END looping through file descriptors
        //Check for timouts
        for (i = 0; i <= fdmax; i++)  
        {
            if (i != listener && FD_ISSET(i, &master))
            {
                time_t current_time = time(NULL);
                if (current_time - last_activity_time[i] >= TIMEOUT_SECONDS)
                {
                    // Client has been idle for TIMEOUT_SECONDS, close the connection
                    printf("Client on socket %d has been idle for too long, closing the connection\n", i);
                    close(i);           // bye!
                    FD_CLR(i, &master); // remove from master set
                }
            }
        }
        if(error_flag == true)
        {
            return -1;
        }
    }          

}
/**
 * @brief Print the files in the directory
 * @param int file_count, no of file in directory
 * @param struct FileInfo* file_list Pointer to struct holding File info
 * @returns none
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

/**
 * 1. Server on-startup should scan pre-defined directory and create list of files.
*/
/**
 * @brief List all files in the dirpath variable on startup
 * @param struct FileInfo** file_list - Note its pointer to pointer - needed for dynamic resizing of array
 * @param int* file_count - the address of variable where file count is stored
 * @returns -1 on failure, 0 on success
*/
int list_all_files(struct FileInfo** file_list, int* file_count) 
{
    DIR* dp = opendir(dir_path);
    if (dp == NULL) 
    {
        perror("opendir");
        return -1; // Indicate error
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) 
    {
        if (entry->d_type == DT_REG) {
            struct FileInfo* temp = realloc(*file_list, (*file_count + 1) * sizeof(struct FileInfo));
            if (temp == NULL) 
            {
                perror("realloc");
                return -1; // Indicate error
            }
            *file_list = temp;

            strcpy((*file_list)[*file_count].name, entry->d_name);

            struct stat st;
            char path[256];
            sprintf(path, "%s%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0) 
            {
                (*file_list)[*file_count].size = st.st_size;
            } 
            else 
            {
                (*file_list)[*file_count].size = -1;
            }

            (*file_list)[*file_count].creation_time = st.st_ctime;

            (*file_count)++;
        }
    }

    closedir(dp);

    return 0; // Indicate success
}

int main() 
{
    struct FileInfo* file_list = NULL;
    int file_count = 0;

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    //Do the part one of Exercise
    if (list_all_files(&file_list, &file_count) != 0) 
    {
        perror("files initialize");
        free(file_list);
        exit(1);
    }

    // See if the file list is created or not
    print_files(file_count, file_list);

    //Start the server
    if(server_handle(file_list,file_count) == -1)
    {
        cleanup(file_list,file_count);
    }
    //Convert 

    //free(file_list);

    return 0;
}

