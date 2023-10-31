/**
 * 
 * #include <sys/types.h>
 * #include <sys/socket.h>
 *  int socket(int domain, int type, int protocol); -> Here you get a socket descriptor
 *  int bind(int sockfd, struct sockaddr *my_addr, int addrlen); 
 *  int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
 *  int listen(int sockfd, int backlog);  -> backlog a queue which increases unless you accept()
 *  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); -> creates a new socket descriptor for the new connection you accepted,
 *                                                                        old descriptor keeps listening to new connections
 *  int send(int sockfd, const void *msg, int len, int flags);
 *  int recv(int sockfd, void *buf, int len, int flags);
 *  int sendto(int sockfd, const void *msg, int len, unsigned int flags,
 *              const struct sockaddr *to, socklen_t tolen);
 *  int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
 *              struct sockaddr *from, int *fromlen);
 *  close(sockfd);
 *  int shutdown(int sockfd, int how); //Shutdown like close but we have more control (how - 0 Further receives are disallowed
 *                                                                                           1 Further sends are disallowed
 *                                                                                           2 Further sends and receives are disallowed (like close())
 *  int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
 * 
 * 
 * 
 * 1. socket() -> get fd
 * 2. bind() -> what port am I on?
 * 3. connect() -> Hey you!  (Done by Client)
 * 4. listen() -> Will somebody please call me? (Done by Server)
 * 5. accept() -> Thankyou for calling port (Done by Server)
 *          new descriptor created for the accepted connection, which is handled through send() and recv() calls
 * 6. send() and recv() -> Talk to me baby! - for stream sockets or connected datagram sockets
 *    sendto() and recvfrom() - for regular unconnected datagram  sockets
 * 7. close()and shutdown() -> Get out of my face
 * 8. getpeername() -> who are you?
 *    
 * 
 * 
*/