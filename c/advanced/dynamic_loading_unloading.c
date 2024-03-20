/**
 * Load some code without explicity linking in that code 
*/
#include <stdio.h>
#include <dlfcn.h> //Necessary header for the dynamic loading

int main()
{
    // Load the shared library
    void* handle = dlopen("/workspaces/docker_c/shared_lib/libmylib.so", RTLD_LAZY);

    if (!handle)
    {
        fprintf(stderr, "Error loading shared library: %s\n", dlerror());
        return 1;
    }

     // Function pointer to the function in the shared library
    int (*my_function)();

    // Get a pointer to the function from the loaded library
    my_function = dlsym(handle, "func1");

    if (!my_function) {
        fprintf(stderr, "Error getting symbol from shared library: %s\n", dlerror());
        dlclose(handle); // Close the library
        return 1;
    }

    //Call the function
    my_function();

    // Unload the library
    dlclose(handle);

    return 0;


}