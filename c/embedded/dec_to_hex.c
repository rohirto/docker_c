/*C program to convert number from decimal to hexadecimal*/

#include <stdio.h>

int main()
{
    int number, cnt, i;
    char hex[32]; /*bcoz it contains characters A to F*/

    printf("Enter decimal number: ");
    scanf("%d", &number);

    cnt = 0; /*initialize index to zero*/
    while (number > 0) {
        switch (number % 16) {
        case 10:
            hex[cnt] = 'A';
            break;
        case 11:
            hex[cnt] = 'B';
            break;
        case 12:
            hex[cnt] = 'C';
            break;
        case 13:
            hex[cnt] = 'D';
            break;
        case 14:
            hex[cnt] = 'E';
            break;
        case 15:
            hex[cnt] = 'F';
            break;
        default:
            hex[cnt] = (number % 16) + 0x30; /*converted into char value*/
        }
        number = number / 16;
        cnt++;
    }

    /*print value in reverse order*/
    printf("Hexadecimal value is: ");
    for (i = (cnt - 1); i >= 0; i--)
        printf("%c", hex[i]);

    return 0;
}