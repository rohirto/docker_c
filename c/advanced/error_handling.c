#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <path> <user_id> <group_id>\n", argv[0]);
        exit(1);
    }

    const char *path = argv[1];
    uid_t user_id = (uid_t)atoi(argv[2]);
    gid_t group_id = (gid_t)atoi(argv[3]);

    int rval = chown(path, user_id, group_id);

    if (rval != 0) {
        // Save errno because it’s clobbered by the next system call.
        int error_code = errno;

        // The operation didn’t succeed; chown should return -1 on error.
        assert(rval == -1);

        // Check the value of errno, and take appropriate action.
        switch (error_code) {
            case EPERM:
            case EROFS:
            case ENAMETOOLONG:
            case ENOENT:
            case ENOTDIR:
            case EACCES:
                // Something’s wrong with the file. Print an error message.
                fprintf(stderr, "Error changing ownership of %s: %s\n", path, strerror(error_code));
                // Don’t end the program; perhaps give the user a chance to choose another file...
                break;
            case EFAULT:
                // PATH contains an invalid memory address. This is probably a bug.
                abort();
            case ENOMEM:
                // Ran out of kernel memory.
                fprintf(stderr, "%s\n", strerror(error_code));
                exit(1);
            default:
                // Some other, unexpected, error code. We’ve tried to handle all possible error codes; if we’ve missed one, that’s a bug!
                abort();
        };
    } else {
        printf("Ownership of %s changed successfully.\n", path);
    }

    return 0;
}
