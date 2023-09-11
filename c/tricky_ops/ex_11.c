#include <stdio.h>
int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    int *p = arr;

    printf("%d\n", *(p++)); //1 will print
    printf("%d\n", *++p);   //3 will print
    printf("%d\n", *(++p)); //4 will print

    return 0;
}