//Pointers 

#include <stdio.h>
int main()
{

    int a=10,b=20,*p,s=0;
    p = &a;
    a++;        //a = 11
    (*p)++;     // pointer to a ++ -> 12
    s = a + b + *p;         //s -> 44
    printf("%d\n",s);
    return 0;

}