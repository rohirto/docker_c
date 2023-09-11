#include <stdio.h>
void func(int *ptr)
{
    *ptr = 10;
    ptr++;
    *ptr = 20;
}

int main()
{
    int arr[3] = {0};
    int *ptr = arr;
    func(ptr);
    printf("%d %d %d\n", arr[0], arr[1], arr[2]);   //10 20 0
    
    return 0;

}    