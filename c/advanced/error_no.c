/**
 * The GNU/Linux provides various error no to factor in the system call failures
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int main() 
{
    FILE *file = fopen("non_existent_file.txt", "r");

    //using stderror and errno
    if (file == NULL) 
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
    //Using perror
    if (file == NULL) {
        perror("Error opening file");
    }
    assert(file != NULL);
    fclose(file);
    return 0;
}