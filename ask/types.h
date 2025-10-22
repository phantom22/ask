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
#ifndef TYPES_H_
#define TYPES_H_

#define SIZE_FILE_PATH 256u
typedef char file_path_t[SIZE_FILE_PATH];

#define HEX_CHECKSUM_SIZE 65lu
typedef char hex_checksum_t[HEX_CHECKSUM_SIZE];

#define COMMAND_SIZE 128u
typedef char command_t[COMMAND_SIZE];

#define COMMAND_OUTPUT_SIZE 8192u
typedef unsigned char command_output_t[COMMAND_OUTPUT_SIZE];

#define OLLAMA_MODEL_SIZE 128u
typedef char model_name_t[OLLAMA_MODEL_SIZE];

#define ALIAS_KEY_SIZE 64u
typedef char alias_key_t[ALIAS_KEY_SIZE];

#define ALIAS_VALUE_SIZE 1024u
typedef char alias_value_t[ALIAS_VALUE_SIZE];

struct _alias {
    alias_key_t key;
    alias_value_t value;
};

#define MAX_ALIAS_COUNT 64u
typedef struct _alias aliases_t[MAX_ALIAS_COUNT];

#endif