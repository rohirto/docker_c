/**
 * Archives is a collection of object files, it is a static way of linking a library with our program
 * Archives can be created with ar command, has a .a extension
 * usage: archive_lib $ ar rcs mylib.a test1.o test2.o  <== Create a libtest.a using object files
 * Linking: $ gcc -o ../build/archives.out archives.c -Larchive_lib -lmylib   <== linux adds the predix "lib" and suffix ".a" to the -ltest
 *                                                                                  -L flag -> library location
 * VSCode tasks are created to handle this
*/

#include <stdio.h>

int main()
{
    func1();  //This function will be sourced from our .a library in archive_lib folder
    func2();  // This function will be sourced from our .a library in archive_lib folder

    //We might get warning of implicit warning in the code, but no worries

    return 0;
}