#include <ncurses.h>
#include <curses.h>
#include <string.h>

int main() {
    // Initialize ncurses
    initscr();

    // Print "Hello, World!" in the center of the screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    const char* message = "Hello, World!";
    int x = (max_x - strlen(message)) / 2;
    int y = max_y / 2;
    mvprintw(y, x, message);
    refresh();

    // Wait for a key press and then exit
    getch();

    // Clean up and exit ncurses
    endwin();

    return 0;
}