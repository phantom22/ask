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
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#define FILE_PATH_SIZE 256u
typedef char file_path_t[FILE_PATH_SIZE];

/** Resolves to "$HOME/.ask" */
extern file_path_t ROOT_DIR;
extern unsigned int ROOT_DIR_LENGTH;

/** Resolves to "$HOME/.ask/saved" */
extern file_path_t SAVE_DIR;
extern unsigned int SAVE_DIR_LENGTH;

/** Actual size in bytes */
extern unsigned int MAX_SUBDIRECTORY_SIZE;
/** Actual size in bytes */
extern unsigned int MAX_CONVERSATION_SIZE;

int filesystem_resolve_constants();
int filesystem_resolve_path(const char* rel_path, file_path_t* output);
int filesystem_is_allowed_path(file_path_t* p);

#endif