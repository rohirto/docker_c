/**
 * Develop a parent-child process application, wherein child process calculates square of numbers (using progressive arithmetic 
 * i.e. square of 2 = 2+2, square of 3 = 3+3+3 and so on) from 1 to 1000 and parent process prints result as soon as soon as child 
 * process produce it. Use Pipes for IPC (10 points)
 * 
 * 
 * pipes(fd)  //fd[2] -> its an array of size 2
 * fd[0] -> For reading
 * fd[1] -> For writing
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CHILD_PROCESS   (pid_t)0
#define COUNT           1000


void square_calculater(int count, FILE* stream) 
{
    
    for (int i = 1; i <= count; i++)
    {
        int square = 0;
        //Calculate the square using AP
        for(int j = i; j > 0; j--)
        {
            square += i;

        }
        /* Write the message to the stream, and send it off immediately. */
        fprintf(stream, "%d\n", square);
        fflush(stream);

    }
}

void reader(FILE* stream) 
{
    char buffer[1024];
    /* Read until we hit the end of the stream. fgets reads until
    either a newline or the end-of-file. */
    while (!feof(stream) && !ferror(stream) && fgets(buffer, sizeof(buffer), stream) != NULL)
        fputs(buffer, stdout);
}

int main()
{
    int fds[2];
    pid_t pid;

    /* Create a pipe. File descriptors for the two ends of the pipe are
    placed in fds. */
    pipe(fds);

    /* Fork a child process. */
    pid = fork();

    if(pid == CHILD_PROCESS)
    {
        //Child Process
        FILE* stream;
        /* This is the child process. Close our copy of the reader end of
        the file descriptor. */
        close(fds[0]);
        /* Convert the write file descriptor to a FILE object, and write to it. */
        stream = fdopen(fds[1], "w");
        square_calculater(COUNT, stream);
        close(fds[1]);


    }
    else if (pid > CHILD_PROCESS)
    {
        //Parent Process
        FILE* stream;
        /* This is the Parent process. Close our copy of the write end of
        the file descriptor. */
        close(fds[1]);
        /* Convert the read file descriptor to a FILE object, and read
        from it. */
        stream = fdopen(fds[0], "r");
        reader(stream);
        close(fds[0]);

    }
    else
    {
        perror("fork");
        exit(1);
    }
    return 0;
}