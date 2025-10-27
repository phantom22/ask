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
#ifndef ASK_H_
#define ASK_H_

// note that this define is also done
// during compilation by the Makefile
#define _POSIX_C_SOURCE 200809L

#ifdef __cplusplus
extern "C" {
#endif

#include "filesystem/filesystem.h"
#include "filesystem/file.h"
#include "integrity/validate_checksum.h"
#include "integrity/is_program_installed.h"
#include "io/command.h"
#include "io/config.h"
#include "io/prompt.h"
#include "json/json.h"

/** Returns -1 on fail. */
int ask_init();
void ask_cleanup();

#ifdef __cplusplus
}
#endif

#endif