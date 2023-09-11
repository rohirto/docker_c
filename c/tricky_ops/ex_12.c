#include <stdio.h>
#include <string.h>

int main()
{

    char a[] = "Include Help";                  //char a[] = "Include Help"; declares an array a containing the string "Include Help". 
                                                //This array will have enough storage to hold the characters in the string, 
                                                //including the null terminator.
    char *b = "Include Help";                   //char *b = "Include Help"; declares a pointer b pointing to a string literal "Include Help". 
                                                //This pointer does not allocate space for the string; 
                                                //it simply points to the existing string literal in memory.

    printf("%d, %d,", sizeof(a), sizeof(b));   //Since a is an array, sizeof(a) gives the size of the entire array, including the string and the null terminator. 
                                                //sizeof(b) gives the size of a pointer (typically 4 or 8 bytes depending on the architecture).
    printf("%d, %d", sizeof(*a), sizeof(*b));   //prints the size of the dereferenced pointers *a and *b. 
                                                //Since both a and b are pointers (or decay to pointers in the case of arrays), 
                                                //sizeof(*a) and sizeof(*b) both give the size of a char (usually 1 byte).

    return 0;

}