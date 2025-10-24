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
#include "filesystem.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int filesystem_resolve_constants() {
    char* home = getenv("HOME");
    int r_length = snprintf(DIR_ROOT, SIZE_FILE_PATH, "%s/.ask", home),
        s_length = snprintf(DIR_SAVED, SIZE_FILE_PATH, "%s/saved", DIR_ROOT);

    if (r_length < 0 || s_length < 0 || 
        snprintf(FILE_DOT_ASK, SIZE_FILE_PATH, "%s/.ask", DIR_ROOT) < 0 || 
        snprintf(FILE_DATA_JSON, SIZE_FILE_PATH, "%s/data.json", DIR_ROOT) < 0
    ) {
        LENGTH_ROOT_DIR = 0;
        LENGTH_SAVED_DIR = 0;
        return -1;
    }

    LENGTH_ROOT_DIR = r_length;
    LENGTH_SAVED_DIR = s_length;
    SIZE_MAX_SUBDIRECTORY = SIZE_FILE_PATH - r_length - 1;
    MAX_CONVERSATION_SIZE = SIZE_FILE_PATH - s_length - 1;
    
    #ifdef ASK_PRINT_GLOBALS
    printf("DIR_ROOT: \"%s\"\nDIR_SAVED: \"%s\"\nFILE_DOT_ASK: \"%s\"\nFILE_DATA_JSON: \"%s\"\nSIZE_FILE_PATH: %i\nDIR_LENGTH_ROOT: %i\nLENGTH_SAVE_DIR: %i\nSIZE_MAX_SUBDIRECTORY: %i\nSIZE_FILE_PATH: %i\n",
        DIR_ROOT,
        DIR_SAVED,
        FILE_DOT_ASK,
        FILE_DATA_JSON,
        SIZE_FILE_PATH,
        LENGTH_ROOT_DIR,
        LENGTH_SAVED_DIR,
        SIZE_MAX_SUBDIRECTORY,
        SIZE_FILE_PATH
    );
    #endif
    
    return 0;
}

/** Returns -1 on fail. */
int filesystem_resolve_path(const char* rel_path, file_path_t* output) {
    if (rel_path == nullptr || output == nullptr) return -1;
    else if (strlen(rel_path) > SIZE_MAX_SUBDIRECTORY - 1) return -1;
    return -(snprintf(*output, SIZE_FILE_PATH, "%s/%s", DIR_ROOT, rel_path) < 0);
}

int filesystem_is_allowed_path(file_path_t* p) {
    return strstr(*p, DIR_ROOT) == *p;
}