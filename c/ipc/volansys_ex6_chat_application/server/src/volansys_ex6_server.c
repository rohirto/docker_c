/**
 * @file volansys_ex6_server.c
 * @author rohit S
 * @brief Server file for the Me chat application
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023 Volansys Technologies
 * 
 * @paragraph MEGA Exercise
 * Chat application [4.5 day, 30 points)
 * 
 * Develop a chat application using socket programming. Make sure to follow the amendments as followed:
 *  1. Multiple clients with a single server. -Done
 *  2. Server should create a dynamic database for storing all the client data and should dynamically update the database based on the client connection –
 *       disconnection. – (Persistent storage is fine) - Done
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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
#include "app_socket.h"
#include "app_event.h"
#include <event2/event.h>  //Used for asynchronous sync
#include "app_queue.h"
#include "app_config.h"
#include "app_db.h"







// //GLobal Variables
// const char* username_file = "../db/username.txt";
// const char* password_file = "../db/passwd.txt";
// const char* status_file = "../db/status.txt";

//Function Prototypes
void *thread_function(void*);

#if USE_THREADS
//Sync Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  /**< Mutex for client Queue */
pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER; /**< Mutex for client ops */
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER; /**< Cond var for client Queue */
pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;  /**< Mutex for message Queue */
pthread_cond_t m_cond_var = PTHREAD_COND_INITIALIZER; /**< Cond var for client Queue */
#endif

/**
 * @brief Initialize Databases
 * 
 * @return int 0 on success and -1 on failure
 * @callgraph
 * @callergraph
 */
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


/**
 * @brief Initialize the server
 * 
 * @return int 0 On sucess -1 on failure
 * @callgraph
 * @callergraph
 */
int init_server()
{
    server_cnxt* init_context = get_server_context();

    int rv;
    int yes = 1; // for setsockopt() SO_REUSEADDR, below

#if USE_THREADS
    //Createa bunch of threads which wde are going to reuse 
    debugLog2("%s\n","Creating a new threads to handle sockets ");
    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&init_context->thread_pool[i],NULL,thread_function, &i);
    }
#endif

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
#if USE_SELECT
        //Set listener as nonblocking
        if(make_sock_nonblocking(init_context->listener) ==  -1)
        {
            debugError("listner nonblocks");
            return -1;
        }
#endif
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
 * @callgraph
 * @callergraph
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}


/**
 * @brief Client handle, the thread after getting a new connection, handles it through this function
 * 
 * @param pclient User Context, struct which stores all the data related to client handled by that thread
 * @callgraph
 * @callergraph
 */
void client_handle(const User_Context* pclient)
{
    //server_cnxt* server_context = get_server_context();
    User_Context client = *pclient;  
    int retval;
    client.config_flag = 0;
    client.msg_flag = 0;
    client.error_flag = 0;
    EventType event;
    // Set up event handlers
    EventHandler eventHandler = 
    {
        .onRead = onReadHandler,
        .onWrite = onWriteHandler,
        .onException = onExceptionHandler
    };


#if USE_SELECT_CLI
    fd_set c_master;                      // master file descriptor list
    fd_set c_read_fds;                    // temp file descriptor list for select()
    fd_set c_write_fds;
    int c_fdmax;

    //Initialize FDs
    // Initialize file descriptor sets
    FD_ZERO(&c_master); // clear the master and temp sets
    FD_ZERO(&c_read_fds);
    FD_ZERO(&c_write_fds);

    FD_SET(client.socket, &c_master);
    // keep track of the biggest file descriptor
    c_fdmax = client.socket; // so far, it's this one
 #else

    // receive Username
    unsigned char c, buffer[128];
    if (recv(client.socket, &c, 1, 0) == -1)
    {
        debugError("Socket error\n");
    }
    unsigned char len;
    if (recv(client.socket, &len, 1, 0) == -1)
    {
        debugError("Socket error\n");
    }
    recv(client.socket, buffer, len, 0);
    memset(client.username, 0x00, 8);
    memcpy(client.username, buffer, len);
    debugLog1_constr("Username: %s\n", client.username);
    if ((client.userID = username_handling(client.username)) == -1)
    {
        debugError("config packet");
    }
    else
    {
        // set the username to online
        client.status = ONLINE;
        if (status_handling(client.userID, client.status) == -1)
        {
            debugError("config packet");
        }
        else
        {
            // Send the list to client
            if (send_username(client.socket, client.userID) == -1)
            {
                debugError("write");
            }
        }
    }
#endif
    while(1)
    {
#if USE_SELECT_CLI
        c_read_fds = c_master;                                              // copy it, need to keep a safe copy of master fds
        c_write_fds = c_master;  

        // Use select to wait for activity on descriptors
        if (select(c_fdmax + 1, &c_read_fds, &c_write_fds, NULL, NULL) == -1) 
        {

            debugError("select");
            continue;
        }

        // run through the existing connections looking for data to read
        // Visualizing this loop as a polling agent,which will be used to call our event dispatcher
        if (FD_ISSET(client.socket, &c_read_fds))
        {
            event = READ_EVENT;
            pthread_mutex_lock(&c_mutex);
            retval = dispatchEvent(&client, event, &eventHandler);
            pthread_mutex_unlock(&c_mutex);
            if(retval  == -2)
            {
                //Close the socket and return only when 0 is received on recv
                close(client.socket);
                FD_CLR(client.socket,&c_master);
                return; //Socket closed due to error
            }
            else
            {
                // Some normal after event activities
                if (client.msg_flag == 1)
                {
                    // Some message has been received and needs to be sent to an appropriate thread
                    msg_t *message = malloc(sizeof(msg_t));
                    if (message == NULL)
                    {
                        debugError("malloc");
                        break;
                    }
                    message->userID = client.chat_userID;
                    sprintf(message->msg, "%s:%s", client.username, client.rx_msg);

                    pthread_mutex_lock(&m_mutex);
                    enqueue_msg(message);
                    pthread_mutex_unlock(&m_mutex);

                    client.msg_flag = 0;
                }
            }
        }
        if (FD_ISSET(client.socket, &c_write_fds))
        {
            if (client.status == ONLINE && (client.config_flag == 1 || client.error_flag != 0))
            {
                event = WRITE_EVENT;
                pthread_mutex_lock(&c_mutex);
                if ((retval = dispatchEvent(&client, event, &eventHandler)) == -1)
                {
                    close(client.socket);
                    FD_CLR(client.socket, &c_master);
                }

                pthread_mutex_unlock(&c_mutex);
            }


            // Check if any message from other threads
            int x = pthread_mutex_trylock(&m_mutex);
            if (x == 0)
            {
                // Mutex acquired
                msg_t *message = dequeue_msg(client.userID);
                pthread_mutex_unlock(&m_mutex);
                if (message != NULL)
                {
                    event = WRITE_EVENT;
                    // We have a message
                    client.send_msg[0] = MESSAGE_PACKET;
                    strcpy(&client.send_msg[1], message->msg);
                    pthread_mutex_lock(&c_mutex);
                    if ((retval = dispatchEvent(&client, event, &eventHandler)) == -1)
                    {
                        close(client.socket);
                        FD_CLR(client.socket, &c_master);
                    }
                    pthread_mutex_unlock(&c_mutex);
                }
            }
        }

#else

        if (check_connection(client.socket) == -1)
        {
            close(client.socket);
            // set the username to offline
            client.status = OFFLINE;
            pthread_mutex_lock(&c_mutex);
            if (status_handling(client.userID, client.status) == -1)
            {
                debugError("status update\n");
            }
            pthread_mutex_unlock(&c_mutex);
            debugLog1_destr("Closed Connection on %d\n", client.socket);
        }
#endif
        
    }



}


/**
 * @brief Listener Function for the Server, It is the Main thread which listens for new connections
 * 
 * @return int Never returns, but on error and failure returns -1
 * @callgraph
 * @callergraph
 */
int server_listen()
{
    server_cnxt* listener_context = get_server_context();
    //int i = 0;  //Iterator for thread
    // listen
    if (listen(listener_context->listener, MAX_LISTEN_BACKLOG) == -1)
    {
        debugError("listen");
        return -1;
    }
#if USE_SELECT
    FD_ZERO(&listener_context->master);
    FD_ZERO(&listener_context->readset);
    FD_ZERO(&listener_context->writeset);
    FD_ZERO(&listener_context->exset);

    FD_SET(listener_context->listener, &listener_context->master);
    listener_context->maxfd = listener_context->listener;
#endif

    // main loop
    for (;;)
    {
#if USE_SELECT
        listener_context->readset = listener_context->master;
        listener_context->writeset = listener_context->master;
        listener_context->exset = listener_context->master;

        

        //Select 
        if(select((listener_context->maxfd ) + 1, &listener_context->readset,&listener_context->writeset,
        &listener_context->exset, NULL) == -1)
        {
            debugError("select");
            return -1;
        }

        // handle new connections
        if (FD_ISSET(listener_context->listener, &listener_context->readset))
        {
#endif
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
                debugLog2("No of Active Connections: %d\n", listener_context->no_of_active_connections);

#if USE_SELECT
                //Make the new socket nonblocking
                if(make_sock_nonblocking(listener_context->newfd) == -1)
                {
                    debugError("sock nonblocking");
                    continue;
                }
#endif

                // Need to see if the connection is valid
                // Check username in database, if already there, validate password
                // If new username, add to database - update the 3 databases
#if USE_SELECT
                //Add to FD SET
                FD_SET(listener_context->newfd,&listener_context->master);
                if(listener_context->newfd > listener_context->maxfd)
                {
                    listener_context->maxfd = listener_context->newfd;
                }
#endif
#if USE_THREADS
//Make the new socket nonblocking
#if USE_SELECT_CLI
                if(make_sock_nonblocking(listener_context->newfd) == -1)
                {
                    debugError("sock nonblocking");
                    continue;
                }
#endif
                User_Context *new_client = malloc(sizeof(User_Context));
                if(new_client == NULL )
                {
                    debugError("malloc");
                    continue;
                }
                new_client->socket = listener_context->newfd;
                new_client->first_flag = 255;   //To signify that its a brand new connection and not a reconnection
                //After this will need some synchronization mechanisms
                pthread_mutex_lock(&mutex);
                enqueue(new_client);
                pthread_cond_signal(&condition_var);
                pthread_mutex_unlock(&mutex);
               
#endif

            }
#if USE_SELECT
        }
#endif
    }
}

#if USE_THREADS
/**
 * @brief Thread Function, Here the thread waits on a queue for getting new connections from listener
 * 
 * @param arg Thread argument, userID in this case
 * @return void* 
 * @callgraph
 * @callergraph
 * 
 */
void *thread_function(void* arg)
{
    //Thread Function
    while (1)
    {
        User_Context *pclient;
        pthread_mutex_lock(&mutex);
        if((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&condition_var, &mutex);
            //Try again
            pclient = dequeue();
        }
        pthread_mutex_unlock(&mutex);
        if(pclient != NULL)
        {
            //We have a new connection handle it acc
            pclient->threadID = *(int*) arg;  //Store the thread ID
            client_handle(pclient);

            //If here then means client closed the connection
            debugLog1_destr("Client has Disconnected\n");


        }
    }
    
}
#endif


/**
 * @brief Main function
 * 
 * @return int 
 * @callgraph
 * @callergraph
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