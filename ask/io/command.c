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
#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COMMAND_OUTPUT_BLOCK_SIZE 1024lu

inline int shell_command(command_t command) {
    return system(command);
}

inline int capture_shell_command(const char* command, char** output, unsigned long* output_size, unsigned long* output_line_count, unsigned long** output_line_indices) {
    if (output == nullptr)
        return -1;
    
    FILE* fp = popen(command, "r");
    if (fp == nullptr)
        return -1;

    char *buffer = (char*)malloc(COMMAND_OUTPUT_BLOCK_SIZE),
         *new_buffer = nullptr;
    unsigned long *line_indices = nullptr,
                  *new_line_indices = nullptr;
    unsigned long malloced_indices = 0,
                  new_malloced_indices = 0;
    if (buffer == nullptr)
        return -1;
    if (output_line_indices != nullptr) {
        malloced_indices = 15;
        line_indices = (unsigned long*)malloc(sizeof(unsigned long) * malloced_indices);
    }

    buffer[0] = '\0';

    unsigned long buffer_size = COMMAND_OUTPUT_BLOCK_SIZE,
                  new_buffer_size = 0,
                  line_size = 0,
                  /* because buffer contains a single '\0'
                   * to simplify line 77 - memcpy DEST calculation
                   */
                  bytes_written = 1,
                  new_bytes_written = 0,
                  bytes_read = 0,
                  line_count = 0;

    char *line = nullptr;
    long chars_read;
    while ((chars_read = getline(&line, &line_size, fp)) != -1) {
        ++line_count;
        bytes_read = chars_read + 1;
        new_bytes_written = bytes_written + bytes_read;
        //printf("buffer_size: %lu\nbytes_written: %lu\nbytes_read: %ld\nnew_bytes_written: %lu\nline: \"%s\"\n----\n", buffer_size, bytes_written, bytes_read, new_bytes_written, line);
        if (new_bytes_written > buffer_size) {
            new_buffer_size = buffer_size + ((bytes_read / COMMAND_OUTPUT_BLOCK_SIZE) + 1) * COMMAND_OUTPUT_BLOCK_SIZE;
            //printf("[RESIZE] old_size: %lu, new_size: %lu, diff: %lu, blocks_size: %lu, n_blocks: %lu\n", buffer_size, new_buffer_size, new_buffer_size - buffer_size, COMMAND_OUTPUT_BLOCK_SIZE, (bytes_read / COMMAND_OUTPUT_BLOCK_SIZE) + 1);
            new_buffer = (char*)realloc(buffer, new_buffer_size);
            if (new_buffer == nullptr) {
                free(buffer);
                free(line);
                if (line_indices != nullptr)
                    free(line_indices);
                pclose(fp);
                return -1;
            }
            buffer_size = new_buffer_size;
            buffer = new_buffer;
        }
        memcpy(&buffer[bytes_written - 1], line, bytes_read);
        //line[bytes_read-2] = '\0';
        //printf("to: %lu, size: %lu, line: '%s\\n\\0'\n", bytes_written - 1, bytes_read, line);
        
        if (line_indices != nullptr && line[0] != '\0') {
            if (line_count > malloced_indices) {
                new_malloced_indices = malloced_indices * 2;
                new_line_indices = (unsigned long*)realloc(line_indices, sizeof(unsigned long) * new_malloced_indices);
                //printf("[RESIZE] old_length: %lu, new_length: %lu\n", malloced_indices, new_malloced_indices);
                if (new_line_indices == nullptr) {
                    free(line);
                    free(buffer);
                    pclose(fp);
                    free(line_indices);
                }
                line_indices = new_line_indices;
                malloced_indices = new_malloced_indices;
            }
            //printf("[%lu] = %lu\n", line_count-1, bytes_written-1);
            line_indices[line_count-1] = bytes_written-1;
        }

        bytes_written = bytes_written == 0 ? new_bytes_written : new_bytes_written - 1;
    }

    free(line);
    pclose(fp);

    if (bytes_written < buffer_size) {
        if (buffer[bytes_written-2] == '\n') {
            buffer[bytes_written-2] = '\0';
            new_buffer = (char*)realloc(buffer, bytes_written-1);
            buffer_size = bytes_written-1;
        }
        else {
            new_buffer = (char*)realloc(buffer, bytes_written);
            buffer_size = bytes_written;
        }

        //printf("[FIT] old_size: %lu, new_size: %lu\n", buffer_size, bytes_written);
        if (new_buffer == nullptr) {
            free(buffer);
            if (line_indices != nullptr)
                free(line_indices);
            return -1;
        }
        buffer = new_buffer;
    }

    *output = buffer;

    if (output_size != nullptr)
        *output_size = buffer_size;

    if (output_line_count != nullptr)
        *output_line_count = line_count;

    if (output_line_indices != nullptr) {
        if (line_count < malloced_indices) {
            //printf("[FIT] old_length: %lu, new_length: %lu\n", malloced_indices, line_count);
            new_line_indices = (unsigned long*)realloc(line_indices, sizeof(unsigned long) * line_count);
            if (new_line_indices == nullptr) {
                free(buffer);
                free(line_indices);
                return -1;
            }
            line_indices = new_line_indices;
        }
        *output_line_indices = line_indices;
    }

    return 0;
}