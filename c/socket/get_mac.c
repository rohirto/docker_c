#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


int main() {
    int socket_fd;
    struct ifreq ifr;

    // Open a socket (AF_INET is not important here)
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("Error opening socket");
        return 1;
    }

    // Set the interface name
    strcpy(ifr.ifr_name, "eth0"); // Change to the desired interface name

    // Get the MAC address
    if (ioctl(socket_fd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("Error getting MAC address");
        return 1;
    }

    // Close the socket
    close(socket_fd);

    // Print the MAC address
    printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           (unsigned char)ifr.ifr_hwaddr.sa_data[0],
           (unsigned char)ifr.ifr_hwaddr.sa_data[1],
           (unsigned char)ifr.ifr_hwaddr.sa_data[2],
           (unsigned char)ifr.ifr_hwaddr.sa_data[3],
           (unsigned char)ifr.ifr_hwaddr.sa_data[4],
           (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    return 0;
}