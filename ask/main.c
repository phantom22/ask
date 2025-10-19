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

    // json_t *root;
    // json_error_t error;
    // root = json_load_file("bin/test.json", 0, &error);

    // json_t *name = json_object_get(root, "name");
    // const char *name_str = json_string_value(name);

    // json_object_set_new(root, "name", json_string("new name"));
    // json_dump_file(root, "bin/new_test.json", JSON_INDENT(2));

    // json_decref(root);

    init_constants();

    command_output_t output;
    capture_shell_command("ls -a", &output);
    printf(" output: \"%s\"\n", output);

    cleanup();

    return 0;
}   