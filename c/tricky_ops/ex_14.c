  #include <stdio.h>

int main()
{
    int x = 5;
    if (sizeof(x + 1.5) == sizeof(int)) //sizeof(x + 1.5) will be size of float
        printf("A\n");
    else
        printf("B\n");
    return 0;

}