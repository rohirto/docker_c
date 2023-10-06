
# API Reference: 

1. #include <sys/types.h>: This is a header file that provides data types used in system calls and other library functions.

2. #include <unistd.h>: This header file provides access to various POSIX operating system API functions.

3. #include <sys/wait.h>: This header file provides access to functions related to process management and waiting for child processes to terminate.

4. #include <signal.h>: This header file provides access to functions and data types related to signal handling in POSIX-compliant systems.

5. pid_t fork(void);: The fork system call is used to create a new process (child process) that is a copy of the current process (parent process). It returns the process ID (PID) of the child process to the parent process and 0 to the child process.

6. pid_t wait(int *status);: The wait system call is used by a parent process to wait for the termination of a specific child process or any child process. It returns the PID of the terminated child process and stores the exit status of the child process in the status variable.

7. pid_t waitpid(pid_t pid, int *status, int options);: The waitpid system call is similar to wait but allows you to specify which child process to wait for using the pid argument. It provides more control over the waiting process through the options argument.

8. sighandler_t signal(int signum, sighandler_t handler);: The signal function is used to set a custom signal handler for a specific signal (signum). When the specified signal is received, the custom handler function is called to handle the signal.

9. int pause(void);: The pause function is used to pause the calling process until a signal is received. It effectively puts the process to sleep until it receives a signal.

10. int kill(pid_t pid, int sig_num);: The kill function sends a signal (sig_num) to a specific process identified by its PID (pid). It's commonly used to send signals to other processes, for example, to terminate or interrupt them.

11. int raise(int sig_num);: The raise function sends a signal (sig_num) to the calling process itself. It's a way for a process to generate signals for itself.

12. int execl(const char *path, const char *arg, ...);: The execl function is used to replace the current process image with a new program specified by path. It takes a list of arguments, with the last argument being a NULL pointer.

13. int execlp(const char *path, const char *arg, ...);: Similar to execl, but it searches for the program in directories listed in the PATH environment variable.

14. int execle(const char *path, const char *arg, ..., char *const envp[]);: A variant of execl that allows you to specify the environment variables for the new process.

15. int execv(const char *path, char *const argv[]);: Replaces the current process image with a new program specified by path, using an array of arguments.

16. int execvp(const char *file, char *const argv[]);: Similar to execv, but it searches for the program in directories listed in the PATH environment variable.

17. int execve(const char *filename, char *const argv[], char *const envp[]);: A variant of execv that allows you to specify both arguments and environment variables for the new process.

18. unsigned int alarm(unsigned int secs);: The alarm function sets a timer that generates a SIGALRM signal after a specified number of seconds (secs). This can be used for implementing timeouts in programs.

19. void exit(int status);: The exit function is used to terminate the current process and return an exit status to the parent process.

20. int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);: The sigaction function is used to examine or change the action taken by a process on a specific signal (signum). It provides more control over signal handling compared to signal.

21. These functions and system calls are essential for process management, signal handling, and process execution in POSIX-compliant systems like Linux and Unix. They enable developers to create, control, and communicate between processes effectively.