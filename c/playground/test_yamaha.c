#include <stdio.h>

void fun (int input1,int input2,int inbit,int numbit ,int outbit,)
{
    int temp;
    for(int i = 0; i < numbit; i++)
    {
        temp = input1 & ~(1<<inbit++);
    }
    
    input2 = input2 | (temp<<outbit);
    
}
int main()
{
    
}