# C Playground for Interviews and Skills
This repo contains examples and template codes based on C and C++ which can be used in your projects

## Setup Info
Using Docker and VSCode. On windows need to install the Docker Windows software. 
### pre-requisite
1. Docker Installed on system
2. VSCode installed
### Steps
1. Opem Folder in VSCode, a notification to open the project in container will pop up - Accept it [It happens because of .devcontainer folder]
2. If fisrt time set up then based on Internet connection it will take som time
3. Open any source code file -> press Ctrl + Shift + P -> Run Tasks -> Build (Based on C, C threads, C math lib, C ncurses)
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

### Static Library - Archive
1. Build the Library by ctrl+shift+P -> Build Archive Library
2. Above task invokes a Make file in archive_lib folder, all the source files in archive lib are compiled and object files are bundled into a libmylib.a file
3. This file can be included in any other C file by opening the C file in editor, then ctrl+shift+p -> Build C file using static library
4. For example see c/advanced/archives.c

### Shared Library 
1. Build the Library by ctrl+shift+P -> Build Shared Library
2. Above task invokes a Makefile in shared_lib folder, all the source files gets compiled and shared library with a version no is created. Additionally a symbolic link to shared library is created to reference it in gcc command
3. The functions which are to be used, its protoype must be included in include/(any_heaher).h
4. This file can be included in any other C file by opening the C file in editor, then ctrl+shift+p -> Build C file using Shared Library
5. For example see c/advanced/shared_lib.c

### Include Folder
1. Includes all .h files which are used by static and shared libraries or our C libraries

### C Notebooks
1. C Notebook can be added to the repo(cling-xeus project). They are just like python Jupyter Notebooks. 
2. These Notebooks are a combination of C code blocks and markdown blocks, are used to understand the theory behind any programming concept
3. They have an extension of .ipynb
4. Select kernel -> Jupyter Kernel -> C++17 
5. Select Language -> bottom right of the code block -> Select C

### Cross Compilation
1. Can cross compile C and C++ codes for stm32f1
2. Debugging capabilities
   
