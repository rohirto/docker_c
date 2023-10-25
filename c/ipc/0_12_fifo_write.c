/**
 * Continued from previous example, writing to earlier created FIFO
 * 
 * File functions of C library are used here (fopen, fclose etc)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE* fp;
    char message[] = "Hello from Parent!";
    
    // Open the named pipe for writing
    if ((fp = fopen("myfifo", "w")) == NULL) {
        perror("fopen");
        exit(1);
    }
    
    // Write the message to the FIFO
    fprintf(fp, "%s\n", message);
    
    // Flush the buffer to ensure data is sent immediately
    fflush(fp);
    
    // Close the file pointer
    fclose(fp);
    
    return 0;
}
