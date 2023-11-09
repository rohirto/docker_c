/**
 * MEGA Exercise
 * Chat application [4.5 day, 30 points)
 * 
 * Develop a chat application using socket programming. Make sure to follow the amendments as followed:
 *  1. Multiple clients with a single server.
 *  2. Server should create a dynamic database for storing all the client data and should dynamically update the database based on the client connection –
 *       disconnection. – (Persistent storage is fine)
 *  3. Multiple clients can chat with each other – BUT, each message should be via server. 
 *      (Use multi-thread, queue for saving and sending older messages)
 *  4. Console based is fine.  No fancy UI required.
 * 
 * This exercise will include, file operations, socket programming, multhreaded communication, data structures and OS concepts. 
 * 
 * Before starting the above exercise, please prepare a document which would include top level architecture (flow chart containing function names) 
 * on how are you planning to implement the solution and get approval from the Mentor on the same.
 * 
 * Will be going with Event Driven Approach, which is Asynchronous as opposed to earlier Synchronous approach (Select functions)
 * 
 * 
 * Username Database format
 * [UserId][comma][Username - 8 bytes][LF - 1 byte]
 * ..
 * ..
 * <EOF>
 * 
 * 
 * Passwsord Database format 
 * [UserId][comma][Password][LF - 1 Byte]
 * ..
 * ..
 * <EOF>
 * 
 * Status Database Format
 * [UserId][comma][1/0][LF -1 Byte]
 * ..
 * ..
 * <EOF>
 * 
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
#include <stdarg.h>
#include "app_debug.h"
#include <event2/event.h>  //Used for asynchronous sync


//Defines
#define MAX_USERS       128
#define PORT "9034" // port we're listening on


//GLobal Variables
const char* username_file = "/workspaces/docker_c/c/build/username.txt";
const char* password_file = "/workspaces/docker_c/c/build/passwd.txt";
const char* status_file = "/workspaces/docker_c/c/build/status.txt";



/**
 * @brief User context struct
 * 
 */
typedef struct user_cntxt
{
    unsigned char userID;
    unsigned char username[8];  //8 Bytes username
    unsigned char password[8];  //8 byte password
    unsigned char status; //0x00 Offline, 0x01 Online
    unsigned char send_msg[128];
    unsigned char rx_msg[128];
}User_Context;


/**
 * @brief Server context struct
 * 
 */
typedef struct Server_Context
{
    unsigned int no_of_active_connections;  /* No of Active Connections */

    FILE* username;             /* File Pointer of Username file */
    FILE* password;             /* File Pointer of Password File */
    FILE* status;               /* File Pointer of Status File */

    int listener;               /* Listener for new connetions */
    struct addrinfo hints;      /* Some Server related structs */
    struct addrinfo *ai; 
    struct addrinfo *p; 

    struct sockaddr_storage remoteaddr;     /* Client Connection related Variables*/
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int newfd;



}server_cnxt;

/**
 * @brief Get the server context object
 * 
 * @return server_cnxt* 
 */
server_cnxt* get_server_context()
{
    static server_cnxt chat_server;
    return &chat_server;
}

int fopen_db_files()
{
    server_cnxt* init_server_context = get_server_context();
    if(init_server_context != NULL)
    {
        //Add the File pointers
        if((init_server_context->username = fopen(username_file,"a+")) != NULL &&
            (init_server_context->password =fopen(password_file,"a+")) != NULL &&
            (init_server_context->status = fopen(status_file,"a+")) != NULL
        )
        {
            //File pointers are Ok
            init_server_context->no_of_active_connections = 0;

            return 0;
        }
    }
    debugError("fopen db files");
    return -1;
}

int fclose_db_files()
{
    server_cnxt *free_Server_cnxt = get_server_context();
    if (free_Server_cnxt != NULL)
    {
        if (fclose(free_Server_cnxt->username) == 0 &&
            fclose(free_Server_cnxt->password) == 0 &&
            fclose(free_Server_cnxt->status) == 0)
        {
            // Files closed
            return 0;
        }
    }
    debugError("fclose db files");
    return -1;
}

int init_database()
{
    
    server_cnxt* init_context = get_server_context();
    if(fopen_db_files() == -1)
    {
        return -1;
    }

    //Do the init here
    if(init_context != NULL)
    {
        init_context->no_of_active_connections = 0;
    }
    if (fclose_db_files() == -1)
    {
        return -1;
    }
    return 0;
}

int make_sock_nonblocking(int fd)
{
    if(fcntl(fd, F_SETFL, O_NONBLOCK) != 0)
    {
        debugError("fcntl");
    }
    return 0;
}

int init_server()
{
    server_cnxt* init_context = get_server_context();

    int rv;
    int yes = 1; // for setsockopt() SO_REUSEADDR, below

    // get us a socket and bind it
    memset(&init_context->hints, 0, sizeof(init_context->hints));
    init_context->hints.ai_family = AF_UNSPEC;
    init_context->hints.ai_socktype = SOCK_STREAM;
    init_context->hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &init_context->hints, &init_context->ai)) != 0) // fill up the structs
    {
        fprintfRed(stderr, "selectserver: %s\n", gai_strerror(rv));
        return -1; 
    }

    for (init_context->p = init_context->ai; init_context->p != NULL; init_context->p = init_context->p->ai_next)
    {
        init_context->listener = socket(init_context->p->ai_family, init_context->p->ai_socktype, init_context->p->ai_protocol); // Create the socket descriptor
        if (init_context->listener < 0)
        {
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(init_context->listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(init_context->listener, init_context->p->ai_addr, init_context->p->ai_addrlen) < 0)
        {
            close(init_context->listener);
            continue;
        }
        break;

        // if we got here, it means we didn't get bound
        if (init_context->p == NULL)
        {
            fprintfRed(stderr, "selectserver: failed to bind\n");
            return -1;
        }

        freeaddrinfo(init_context->ai); // all done with this  //This can have dangeorous effect on struct?
    }
    return 0;
}

/**
 * @brief Get the in addr object
 * 
 * @param sa 
 * @return * void* 
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int server_listen()
{
    server_cnxt* listener_context = get_server_context();
    // listen
    if (listen(listener_context->listener, 10) == -1)
    {
        debugError("listen");
        return -1;
    }
    // main loop
    for (;;)
    {
        // handle new connections
        listener_context->addrlen = sizeof(listener_context->remoteaddr);
        listener_context->newfd = accept(listener_context->listener,
                       (struct sockaddr *)&listener_context->remoteaddr,
                       &listener_context->addrlen);
        if (listener_context->newfd == -1)
        {
            debugError("accept");
        }
        else
        {
            debugLog2("selectserver: new connection from %s on "
                   "socket %d\n",
                   inet_ntop(listener_context->remoteaddr.ss_family,
                             get_in_addr((struct sockaddr *)&listener_context->remoteaddr),
                             listener_context->remoteIP, INET6_ADDRSTRLEN),
                   listener_context->newfd);

            listener_context->no_of_active_connections++;
            
            //Need to see if the connection is valid
            //Check username in database, if already there, validate password
            //If new username, add to database - update the 3 databases

        }
    }
}


/**
 * @brief Main function
 * 
 * @return int 
 */
int main()
{
    debugLog1_constr("%s","Server Database Init.. \n");
    if(init_database()  == -1)
    {
        debugError("Init Database Failed");
        exit(1);
    }

    debugLog1_constr("%s","Server Sockets Init..\n");
    if(init_server() == -1)
    {
        debugError("Socket Init Failed");
        exit(1);
    }
    debugLog1_constr("%s","Server Listening..\n");
    if(server_listen() == -1)
    {
        debugError("Server Listen Error");
        exit(1);
    }


    debugLog1_constr("%s","Exiting Server\n");
    return 0;
}