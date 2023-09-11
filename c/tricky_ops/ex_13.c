//strcmp example

#include <stdio.h>
#include <string.h>

int main()
{

    char *s1, *s2;
    s1= "abcdef" ;
    s2= "abcdeg" ;
    printf(" %d ", strcmp (s1,s2));   // If s1 < s2 then -1
    printf(", ");
    s1="abcdef";
    s2="abcdef";
    printf(" %d ", strcmp (s1,s2));   // Equal 0
    printf(", ");
    s1="abcdef";
    s2="abcdee";
    printf(" %d ", strcmp (s1,s2));     // If s1 > s2 then 1
    printf(", ");
    
    return 0;

}