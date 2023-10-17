/***
 * stdout is buffered : Unless the buffer is full, the op is not sent to console
 * stderr is not buffered: the data in stderr is sent directly to console
*/
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i = 0;
    //stdout example
    /**
     *  this loop does not print one
        period every second; instead, the periods are buffered, and a bunch of them are printed
        together when the buffer fills.
    */
   printf("\n");
   fflush(stdout);  //Clean the buffer
   while (i < 20)
   {
        printf(".");
        sleep(1);
        i++;
   }
   i = 0;
   printf("\n");
   // This loop prints period for one second
   while (i < 20)
   {
        fprintf(stderr, ".");
        sleep(1);
        i++;
   }

    return 0;
}