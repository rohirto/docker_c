/* C program to check whether a number is palindrome or not */

#include <stdio.h>

/*function to check Palindrome Number*/
int isPalindrome(int num)
{
    int tempNumber = num;
    int dig, revNumber;

    /*getting reverse number*/
    revNumber = 0;
    while (num > 0) {
        dig = num % 10;
        revNumber = (revNumber * 10) + dig;
        num /= 10;
    }

    if (revNumber == tempNumber)
        return 1; /*Palindrome Number*/
    else
        return 0; /*Not a Palindrome Number*/
}

int main()
{
    int number;

    printf("Enter an integer number: ");
    scanf("%d", &number);

    if (isPalindrome(number))
        printf("%d is a palindrome.", number);
    else
        printf("%d is not a palindrome.", number);

    return 0;
}