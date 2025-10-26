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

#include <ncurses.h>
#include <stdlib.h>

int main() {
    initscr();
    printw("Hello World! Press any key to exit...");
    refresh();
    getch();
    endwin();

    ask_init();

    // char* output = nullptr;
    // size_t output_size = 0, line_count = 0, *line_indices;
    // capture_shell_command("ls -a", &output, &output_size, &line_count, &line_indices, null_terminate_lines | remove_trailing_new_line);

    // for (size_t i=0; i<line_count; ++i) {
    //     printf("line: '");
    //     for (size_t j=line_indices[i]; i<line_count-1&&j<line_indices[i+1] || i==line_count-1&&j<output_size; ++j) {
    //         if (output[j] == '\n')
    //             printf("\\n");
    //         else
    //             printf("%c", output[j]);
    //     }
    //     printf("'\n");
    // }

    char* output = nullptr;
    size_t output_size = 0, line_count = 0;
    char **line_pointers;
    capture_shell_command("ls -a", &output, &output_size, &line_count, &line_pointers, null_terminate_lines | indices_as_pointers);

    // only with null_terminate_lines flag
    // for (size_t i=0; i<line_count; ++i) {
    //     printf("line: '%s'\n", line_pointers[i]);
    // }

    for (size_t i=0; i<line_count; ++i) {
        printf("line: '");
        for (char* j=line_pointers[i]; *j!='\0'; j=j+1) {
            if (*j == '\n') {
                printf("\\n");
                break;
            }
            else
                printf("%c", *j);
        }
        printf("'\n");
    }

    if (output != nullptr)
        free(output);
    if (line_pointers != nullptr)
        free(line_pointers);

    ask_cleanup();

    return 0;
}   