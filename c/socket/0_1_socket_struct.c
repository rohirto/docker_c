/**
 * All the files from 0_1_xxxx are made by refering beej socket programming.pdf
 * 
 * Socket descriptor - int
 * 
 * struct addrinfo - prep the socket address for subsequent use -> used with getaddrinfo()
 * 
 * struct sockaddr - holds socket address information of many types of socket
 * 
 * struct sockaddr_in - in for internet - a pointer to to a struct sockaddr_in can be cast to a pointer to a struct sockaddr and vice-versa.
 * 
 * struct sockaddr_in6 - same as above only ipv6
 * 
 * 
*/
//Converting the IP Addresses to the binary rep, in some structs as mentioned above 

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    // IPv4:
    struct sockaddr_in sa; // pretend this is loaded with something

    sa.sin_family = AF_INET;     // IPV4
    sa.sin_port = htons(80);     //host to network short conversion
    sa.sin_addr.s_addr= inet_addr("192.168.1.1");  //Convert the string IP to in_addr_t type
    



    char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
    
    inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);  // “ntop” means “network to presentation”—you can call it “network to printable”
    printf("The IPv4 address is: %s\n", ip4);

    // IPv6:
    char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
    struct sockaddr_in6 sa6; // pretend this is loaded with something
    inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
    printf("The IPv6 address is: %s\n", ip6);

    return 0;
}

