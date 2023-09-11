#include <stdio.h>        

int main()
{
struct demo{
    char * a;
    int n;
}; 

struct demo p = {"hello", 2015};
struct demo q = p;
printf("%d", printf("%s",q.a));   //From the right, first printf prints hello and returns number of characters printed, ie 5
                                  // Then left most printf prints 5
                                  // Thus output will be hello5

return 0;
}