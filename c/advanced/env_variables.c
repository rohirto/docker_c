#include <stdio.h>
#include <stdlib.h>

extern char **environ; // The global environment variable array

int main() {
    // Get the value of an environment variable
    const char *path = getenv("PATH");
    if (path != NULL) {
        printf("PATH environment variable: %s\n", path);
    } else {
        printf("PATH environment variable is not set.\n");
    }

    // Set a new environment variable
    const char *newVarName = "MY_VARIABLE";
    const char *newVarValue = "Hello, World!";
    if (setenv(newVarName, newVarValue, 1) == 0) {
        printf("%s=%s is set as an environment variable.\n", newVarName, newVarValue);
    } else {
        perror("setenv");
    }

    // Get the value of the new environment variable
    const char *newVar = getenv(newVarName);
    if (newVar != NULL) {
        printf("%s=%s\n", newVarName, newVar);
    } else {
        printf("%s is not set.\n", newVarName);
    }

    // Unset the new environment variable
    if (unsetenv(newVarName) == 0) {
        printf("%s is unset.\n", newVarName);
    } else {
        perror("unsetenv");
    }

    // Access all environment variables using environ
    char **envVar = environ;
    while (*envVar != NULL) {
        printf("%s\n", *envVar);
        envVar++;
    }

    return 0;
}