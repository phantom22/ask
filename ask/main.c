/*
 * Ask - a program that facilitates discussion with Ollama models.
 * Copyright (C) 2025  Maksym Naumenko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7:
 * This program is available under additional permissions allowing
 * linking with OpenSSL. For complete details, see the LICENSE file
 * in the root directory of this project.
 */
#include "ask.h"
#include "io/command.h"
#include "io/prompt.h"

#include <ncurses.h>
#include <stdlib.h>

int main() {
    // initscr();
    // printw("Hello World! Press any key to exit...");
    // refresh();
    // getch();
    // endwin();

    if (ask_init() == -1) {
        return 1;
    }

    unsigned int flags = null_terminate_lines;
    char* output = nullptr;
    size_t output_size = 0, line_count = 0;
    void **line_pointers;
    // "ollama list | sed '1d' | awk '{print $1}'"
    int r = capture_shell_command("ollama list | sed '1d' | awk '{print $1}'", &output, &output_size, &line_count, &line_pointers, flags);

    // int sel = prompt_for_selection("Available models:", "Choose the model that you want to use", line_pointers, line_count);
    // if (sel == -1) {
    //     printf("Invalid selection.\n");
    //     return 1;
    // }
    // else {
    //     printf("selected: '%s'\n", line_pointers[sel]);
    // }

    // int agrees = prompt_for_confirmation("Are you totally sure about this");

    // agrees ? printf("yes\n") : printf("no\n");

    if (line_pointers == nullptr)
        printf("is empty\n");
    else if (flags & indices_as_pointers)
        printf("%s\n", ((char**)line_pointers)[0]);
    else
        printf("%lu\n", ((unsigned long*)line_pointers)[0]);

    if (output != nullptr)
        free(output);
    if (line_pointers != nullptr)
        free(line_pointers);

    ask_cleanup();

    return 0;
}   