# C and CPP Playground for Interviews and Skills
This repo contains examples and template codes based on C and C++ which can be used in your projects

## Setup Info
Using Docker and VSCode. On windows need to install the Docker Windows software. 
### pre-requisite
1. Docker Installed on system
2. VSCode installed
### Steps
1. Opem Folder in VSCode, a notification to open the project in container will pop up - Accept it [It happens because of .devcontainer folder]
2. If fisrt time set up then based on Internet connection it will take som time
3. Open any source code file -> press Ctrl + Shift + P -> Run Tasks -> Build (Based on C, C++, C threads, C math lib)
4. Again press Ctrl + Shift + P -> Run Program

### Features - Debugging
1. Can use gdb debugger. Already Launch.json has been configured to do the debugging
2. On the left most pane - select Run and Debug Option

### Features - Static Code Analyzer
1. cppcheck used as static code analyzer
2. vscode extension fly lint is used to integrate vscode with cppcheck

### Features - Google Test
1. Used for Unit testing

### Valgrind Memory Debugging
1. First compile the program using ctrl+shift+ p -> Build C program
2. Make the file executable using ctrl+shift+p -> Make file execuatable
3. Run the Valgrind on file using ctrl+shift+p -> Run using Valgrind

### Ncurses
1. Codespace to play with ncurses library
2. Compile the code with Ctrl+shift+p -> Build ncurses c program
3. Run the code ctrl+shift+p -> run program

### C++ Notebook
1. Using Xeus-Cling for this purpose
2. Using notebook gives a more interactive way to learn cpp
3.
   
