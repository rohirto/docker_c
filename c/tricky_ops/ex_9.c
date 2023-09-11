#include<stdio.h>
int main()
{
    enum days {MON=-1, TUE, WED=6, THU, FRI, SAT};
    printf("%d, %d, %d, %d, %d, %d\n", MON, TUE, WED, THU, FRI, SAT);  // after -1 0 will come
                                                                       // thus -1 0 6 7 8 9
    return 0;

}