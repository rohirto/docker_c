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


#define PORT "9034" // port we're listening on
const char* dir_path = "../ipc/";
volatile bool error_flag = false;
volatile bool file_list_resp = false;  //Expect response from client on file list
volatile int send_file_flag = -1;

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
    for (int i = 0; i < file_count; i++) {
        // Free any resources within the FileInfo structure, if needed
    }
    free(file_list);
}


/* 2. As soon as server can accept client connections it should on demand present the file list to client.*/

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

int sendall(int s, unsigned char *buf, int *len)
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

int recvall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've received
    int bytesleft = *len; // how many bytes we have left to receive
    int n = 0;
    while (total < *len)
    {
        n = recv(s, buf + total, bytesleft, 0);
        if (n == -1  || n == 0)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;            // return the number actually received here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


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
    printf("Total files converted: %d\n", file_count);
    fflush(stdout);

    return 1;
}

int sendFileOverSocket(int socket, const char* filePath) 
{
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) 
    {
        perror("File not found");
        return -1;
    }
    unsigned char c = 0x03;
    if (send(socket, &c, sizeof(c),0) == -1)
    {
        perror("send");
        fclose(file);
        return -1;
    }
    char buffer[1024];
    size_t bytesRead;

//    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) 
//    {
//         if (send(socket, buffer, bytesRead, 0) == -1) {
//             perror("Error sending file");
//             fclose(file);
//             return -1;
//         }
//    }
    while (fgets(buffer, 1024, file) != NULL)
    {
        if (send(socket, buffer, sizeof(buffer), 0) == -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(buffer, 1024);
    }
    printf("\nFile Sent Successfully!\n");
    fclose(file);
    send_file_flag = -1;
    return 0;
}


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
    unsigned char buf[1024];
    int nbytes;
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
                        

                        //Send the File list to client
                        if(send_file_list(newfd,fileInfo,file_count) == -1)
                        {
                            perror("send file list");
                        }

                        file_list_resp = true;
                    }
                }
                else
                {
                    // handle data from a client
                    if(file_list_resp == true)
                    {
                        unsigned char tmp[3];
                        int t_len = 2;
                        if(recvall(i,tmp,&t_len) == -1)
                        {
                            perror("recvall");
                            error_flag = true;
                            close(i);           // bye!
                            FD_CLR(i, &master); // remove from master set
                            fprintf(stderr,"selectserver: socket %d hung up\n", i);
                            
                        }
                        int file_no = unpacki16(tmp);

                        file_list_resp = false;

                        send_file_flag = file_no;

                        printf("\n Received Req for File no:%d\n", file_no);
                        fflush(stdout);
                        
                    }
                } // END handle data from client
            }     // END got new incoming connection
            if (FD_ISSET(i, &write_fds))
            {
                //Socket ready for writing
                if(send_file_flag > -1)
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
                    // //Send the requested file
                    // int pid = fork();
                    // if (pid == 0) 
                    // {
                    //     char path[256];
                    //     sprintf(path, "%s%s", dir_path, fileInfo[send_file_flag].name);
                    //     if(sendFileOverSocket(i,path) == -1)
                    //     {
                    //         perror("file send");
                    //         fprintf(stderr, "File Send Failed for file no: %d",send_file_flag);
                    //     }
                        
                    // }
                    // else if(pid > 0)
                    // {
                    //     //Parent process
                        
                    // }
                    // else if(pid < 0)
                    // {
                    //     perror("fork");
                    //     fprintf(stderr,"Fork failed");
                    // }
                }
            }
        }         // END looping through file descriptors
    }          

}

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

    //Do the part one of Exercise
    if (list_all_files(&file_list, &file_count) != 0) 
    {
        perror("files initialize");
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

    free(file_list);

    return 0;
}

