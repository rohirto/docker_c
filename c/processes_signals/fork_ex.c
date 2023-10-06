/**
 * fork() is used to create new process by duplicating the current calling process, and newly created process is known as 
 * child process and the current calling 
 * process is known as parent process. So we can say that fork() is used to create a child process of calling process.
*/

#include <stdio.h>
#include <unistd.h>

int main()
{
    int id, i;

    printf("Start of main...\n");

    id = fork();
    if (id > 0) {
        /*parent process*/
        printf("Parent section...\n");
    }
    else if (id == 0) {
        /*child process*/
        printf("\nfork created...\n");
    }
    else {
        /*fork creation faile*/
        printf("\nfork creation failed!!!\n");
    }

    printf("Printing the numbers from 1 to 10\n");
    for (i = 1; i <= 10; i++)
        printf("%d ", i);
    printf("\n");
    printf("End of the main function...\n");

    return 0;
}