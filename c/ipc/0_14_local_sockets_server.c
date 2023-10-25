/**
 * Bidirectional Communication Mechanism
 * Allows commincation between 2 processes even on 2 different machines.
 * Internet uses sockets
 * 
 * Socket Concepts - to create sockets we must specify 3 parameters
 * 1. communication style
 * 2. namespace 
 * 3. protocol
 * 
 * Communication Style - How socket treats transmitted data and specifies no of communication partners. Data is packed into packets.
 *                       Communication style determines how these packets are handled, how they handled from sender to reciever
 *                       Communication Style - Guarantees the delivery of packets in the order they were sent
 *                       Datagram Style - Best effort to ensure order of delivery, no guarantee though
 * 
 * Socket Namespace - Specifes How Socket addresses are written (Local namespace socket names - ordinary filenames, Internet Namespace - socket names/addresses 
 *                      consist of Internet Protocol address plus port number)
 * 
 * Protocol - Protocol specifies how data is transmitted (eg. TCP/IP, UNIX Local Communication Protocol)
 * 
 * 1. Creation of Socket
 * While creating a Socket need to specify above 3 parameters
 *      Namespace - (PF_LOCAL, PF_UNIX etc PF stands for Protocol family)
 *      Communication Style - (SOCK_STREAM, SOCK_DGRAM )
 * 
 * 2. Calling Connect
 * Specify the Server Address to connect
 * 
 * 3. Servers
 *      Creation of Socket -> Binding an address to socket -> placing a call to listen -> accepting incoming connections -> close Socket
 * 
 * Below example of Local Sockets 
 * 
 * How to Run this code after building
 *  $ ./0_14_local_sockets_server.out /tmp/socket
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Read text from the socket and print it out. Continue until the socket closes.
   Return nonzero if the client sent a "quit" message, zero otherwise. */
int server(int client_socket) 
{
    while (1) 
    {
        int length;
        char *text;

        /* First, read the length of the text message from the socket.
           If read returns zero, the client closed the connection. */
        if (read(client_socket, &length, sizeof(length)) == 0)
            return 0;

        /* Allocate a buffer to hold the text. */
        text = (char *)malloc(length);

        /* Read the text itself, and print it. */
        read(client_socket, text, length);
        printf("%s\n", text);

        

        /* If the client sent the message "quit," we're all done. */
        if (!strcmp(text, "quit"))
        {
            /* Free the buffer. */
            free(text);
            return 1;
        }
        else
        {
            /* Free the buffer. */
            free(text);
        }
    }
}

int main(int argc, char *const argv[]) 
{
    const char *const socket_name = argv[1];
    int socket_fd;
    struct sockaddr_un name;
    int client_sent_quit_message;

    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);

    /* Indicate that this is a server. */
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    bind(socket_fd, (struct sockaddr *)&name, SUN_LEN(&name));

    /* Listen for connections. */
    listen(socket_fd, 5);

    /* Repeatedly accept connections, spinning off one server() to deal with each client.
       Continue until a client sends a "quit" message. */
    do 
    {
        struct sockaddr_un client_name;
        socklen_t client_name_len;
        int client_socket_fd;

        /* Accept a connection. */
        client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_name, &client_name_len);

        /* Handle the connection. */
        client_sent_quit_message = server(client_socket_fd);

        /* Close our end of the connection. */
        close(client_socket_fd);
    } while (!client_sent_quit_message);

    /* Remove the socket file. */
    close(socket_fd);
    unlink(socket_name);

    return 0;
}



