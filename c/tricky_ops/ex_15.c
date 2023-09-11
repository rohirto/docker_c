//Example of static use

#include <stdio.h>
void func()
{
    static int x = 5;   //static variable retains its value between function calls.
    x++;
    printf("%d ", x);
} 

int main()
{
    func();
    func();
    func();
    return 0;

}