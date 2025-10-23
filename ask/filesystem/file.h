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
#ifndef FILE_H_
#define FILE_H_

#include "../types.h"

struct file {
    int fd;
    file_path_t full_path;
    // pointer to char in full_path
    char* name;
    int created;
    int is_empty;
    int is_valid;
    unsigned long size;
    char* contents;
};

int file_get_name(file_path_t* p, char** fname);
struct file file_open(const char *rpath, int open_flags);
void file_close(struct file *f);
int file_erase_contents(struct file *f);
int file_touch(struct file *f);
int file_initialize_if_empty(struct file *f, const char* with);
int file_get_contents(struct file* p, char** output);

#endif