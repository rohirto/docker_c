# GDB cheatsheet
This sheet is intended to be a guide to use GDB from command line
## GDB Usage
```gcc -g -o ../build/gdb_example gdb_example.c
```

## Cheatsheet
1. run (or r): Start the program with optional command-line arguments.
2. break (or b): Set a breakpoint at a specific line number, function, or address.
    Example: break main or b 42
    info break (or i b): List all breakpoints.
3. delete (or d): Delete a specific breakpoint by its number.
    Example: delete 1
4. continue (or c): Continue program execution until the next breakpoint.
5. next (or n): Execute the current line and stop at the next line.
6. step (or s): Step into a function call, allowing you to debug the function's code.
7. finish: Continue execution until the current function returns.
8. list (or l): Display the source code around the current line.
9. print (or p): Evaluate and print the value of an expression.
    Example: print variable
10. backtrace (or bt): Display the call stack, showing all function calls leading to the current point.
11. info locals: Display the values of local variables in the current stack frame.
12. info args: Display the values of function arguments in the current stack frame.
13. info variables: List all global and static variables in the program.
14. set variable: Change the value of a variable during debugging.
15. Example: set variable x = 42
16. display: Add an expression to the display list, and it will be automatically printed each time the program stops.
17. info display: List the expressions in the display list.
18. disable: Disable a breakpoint by its number.
19. Example: disable 1
20. enable: Enable a disabled breakpoint by its number.
    Example: enable 1
21. run < file.in or run > file.out: Redirect input or output from a file during program execution.
22. attach <pid>: Attach GDB to a running process by specifying its process ID.
23. detach: Detach GDB from the current process.
24. quit (or q): Exit GDB.
25. help (or h): Get help on GDB commands.
26. info registers: Display the values of CPU registers.
27. set follow-fork-mode: Set the behavior for debugging child processes (e.g., set follow-fork-mode child to follow the child after a fork).

## Small tutorial to run on gdb_example.c
1. Start GDB: Run gdb gdb_example.
2. Set a breakpoint in the main function: break main.
3. Start the program with run.
4. Use next to execute the program line by line, or step to step into function calls.
5. Print variables with print:
    print x
    print y
    print result
6. Use list to display the source code around the current line:
    list
7. Continue program execution with continue.
8. When the program stops at the breakpoint, examine the backtrace with bt.
9. Add an expression to the display list:
    display x + y
10. Continue execution and see how the display expression updates.
