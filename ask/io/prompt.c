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
#include "prompt.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef ASK_TEST_MODE
int prompt_for_confirmation(const char* question_text) {
    if (question_text == nullptr)
        return -1;

    char full_question_text[256];
    snprintf(full_question_text, 256, "%s? (y/n): ", question_text);

    printf("%s", full_question_text);

    int input = getchar();
    return input == 'y' || input == 'Y' ? 1 : 0;
}
#else
int prompt_for_confirmation(const char* question_text, const char* user_input) {
    if (question_text == nullptr || user_input == nullptr)
        return -1;

    int input = (int)user_input[0];
    return input == 'y' || input == 'Y' ? 1 : 0;
}
#endif

#ifndef ASK_TEST_MODE
int prompt_for_selection(const char* selection_block_title, const char* question_text, char** string_list, unsigned long list_size) {
    if (selection_block_title == nullptr || question_text == nullptr || string_list == nullptr || list_size == 0)
        return -1;

    printf("%s\n", selection_block_title);
    for (unsigned long i=0; i<list_size; ++i)
        printf("[%lu] %s\n", i+1, string_list[i]);

    char full_question_text[256];
    if (list_size == 1) {
        snprintf(full_question_text, 256, "%s: 1", question_text);
        printf("%s\n", full_question_text);
        return 0;
    }

    snprintf(full_question_text, 256, "%s [1-%lu]: ", question_text, list_size);
    printf("%s", full_question_text);
    
    char user_input[32];
    fgets(user_input, 32, stdin);

    int user_selection = atoi(user_input);
    return (user_selection <= 0 || user_selection > list_size) ? -1 : user_selection - 1;
}
#else
int prompt_for_selection(const char* selection_block_title, const char* question_text, char** string_list, unsigned long list_size, const char* user_input) {
    if (selection_block_title == nullptr || question_text == nullptr || string_list == nullptr || list_size == 0 || user_input == nullptr)
        return -1;
    
    if (list_size == 1)
        return 0;

    int user_selection = atoi(user_input);
    return (user_selection <= 0 || user_selection > list_size) ? -1 : user_selection - 1;
}
#endif