/**
 * The problem mentioned previously (non blocking, polling sockets) has a more elegant solution select()
 * 
 * select() - Synchronous I/O Multiplexing
 * 
 * It gives you power to monitor multiple sockets at sametime (sockets are just file descriptors - thus select can be used on such FDs hich are expecting data 
 * to asynchronously come)
 * 
 * Portable, but one of the slowest method for monitoring sockets - possible alternative is libevent (need to check it out)
 * 
 * 
 * #include <sys/time.h>
 * #include <sys/types.h>
 * #include <unistd.h>
 * 
 * int select(int numfds, fd_set *readfds, fd_set *writefds,
 *              fd_set *exceptfds, struct timeval *timeout);
 * 
 * Needs Macros for its functionality:
 * 1. FD_SET(int fd, fd_set *set); Add fd to the set.
 * 2. FD_CLR(int fd, fd_set *set); Remove fd from the set.
 * 3. FD_ISSET(int fd, fd_set *set); Return true if fd is in the set.
 * 4. FD_ZERO(fd_set *set); Clear all entries from the set.
 * 
 * Below is a select demo
 * 
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0 // file descriptor for standard input
int main(void)
{
    struct timeval tv;
    fd_set readfds;

    //Time out of 2.5 seconds
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    // Filing up the MACROS needed by the select()
    FD_ZERO(&readfds);      //Clear all entries from the set.
    FD_SET(STDIN, &readfds); //Add fd to the set

    // don't care about writefds and exceptfds:
    select(STDIN + 1, &readfds, NULL, NULL, &tv);  //numfds has to be value of highest FD + 1

    if (FD_ISSET(STDIN, &readfds))      //Return true if fd is in the set.
        printf("A key was pressed!\n");  // A key must be pressend within 2.5 sec along with Enter
    else
        printf("Timed out.\n");
    return 0;
}