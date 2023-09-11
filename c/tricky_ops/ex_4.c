#include <stdio.h>
#include <string.h>
int main()
{
    char str[20] = "ABCDEFGHIJK";
    int s = strlen(str);  //11
    str[3] = '\0';
    s += strlen(str); //14
    strcpy(str,"ABCDEF"); 
    s += strlen(str);  //20
    strcat(str,"ABC"); 
    s += strlen(str);  //29     
    printf("%d\n",s);

    return 0;

}