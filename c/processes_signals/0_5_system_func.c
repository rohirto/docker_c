/**
 * System Fucntion - use to invoke linux commands from C file by created a subshell
 * But the o/p depends on what shell and stuff, thus less efficent way to create a process
*/

#include <stdlib.h>

int main() {
    int return_value;

    // Execute the "ls -l /" command and store its exit status in return_value
    return_value = system("ls -l /");

    // Return the exit status of the executed command
    return return_value;
}

