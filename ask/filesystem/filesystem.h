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

#include "../types.h"

/** Resolves to "$HOME/.ask" */
extern file_path_t DIR_ROOT;
extern unsigned int LENGTH_ROOT_DIR;

/** Resolves to "$HOME/.ask/saved" */
extern file_path_t DIR_SAVED;
extern unsigned int LENGTH_SAVED_DIR;

/** Actual size in bytes */
extern unsigned int SIZE_MAX_SUBDIRECTORY;
/** Actual size in bytes */
extern unsigned int MAX_CONVERSATION_SIZE;

extern file_path_t FILE_DOT_ASK;
extern file_path_t FILE_DATA_JSON;

int filesystem_resolve_constants();
int filesystem_resolve_path(const char* rel_path, file_path_t* output);
int filesystem_is_allowed_path(file_path_t* p);

#endif