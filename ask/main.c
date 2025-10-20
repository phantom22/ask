//#define ASK_PRINT_GLOBALS
#include "ask.h"

#include <ncurses.h>
#include <jansson.h>

int main() {
    initscr();
    printw("Hello World! Press any key to exit...");
    refresh();
    getch();
    endwin();

    ask_init();

    command_output_t output;
    capture_shell_command("ls -a", &output);
    printf(" output: \"%s\"\n", output);

    ask_cleanup();

    return 0;
}   