#include <stdio.h>
#include <unistd.h>

int main() {
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Hostname: %s\n", hostname);
    } else {
        perror("gethostname");
    }

    return 0;
}
