/***
 * 2 Ways to do the system calls 
 * 1. use syscall function -> various nos to be passed into the function is given 
 *          LINUX System Call Quick Reference -> refer to get the function nos
 * 2. Use libc wrapped sys function call -> in libs directly function getpid() is given
 *      This one seems to be the more preferred way
*/

#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void)
{
    long ID1, ID2;
    /*-----------------------------*/
    /* direct system call */
    /* SYS_getpid (func no. is 20) */
    /*-----------------------------*/
    ID1 = syscall(SYS_getpid);
    printf("syscall(SYS_getpid)=%ld\n", ID1);
    /*-----------------------------*/
    /* "libc" wrapped system call */
    /* SYS_getpid (Func No. is 20) */
    /*-----------------------------*/
    ID2 = getpid();
    printf("getpid()=%ld\n", ID2);
    return (0);
}