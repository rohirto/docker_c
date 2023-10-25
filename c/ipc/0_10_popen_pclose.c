/**
 * The popen and pclose functions ease this paradigm by eliminating the
 *need to invoke pipe, fork, dup2, exec, and fdopen.
 *
 * This method is a condensed method of previous example
*/

#include <stdio.h>
#include <unistd.h>

int main() 
{
    FILE *stream = popen("sort", "w");
    fprintf(stream, "This is a test.\n");
    fprintf(stream, "Hello, world.\n");
    fprintf(stream, "My dog has fleas.\n");
    fprintf(stream, "This program is great.\n");
    fprintf(stream, "One fish, two fish.\n");
    return pclose(stream);
}
