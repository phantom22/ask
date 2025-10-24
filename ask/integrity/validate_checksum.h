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
 #ifndef VERIFY_CHECKSUM_H_
 #define VERIFY_CHECKSUM_H_
 
#include "../types.h"

struct file;
/** Returns 0 on success or -1 on fail. */
int sha256sum(struct file* f, hex_checksum_t* output);

enum checksum_file_type {
    path_to_file,
    struct_file_ptr
};
/** Returns -1 on fail. Otherwise returns 1 if file has the specified checksum.
  * file can be either a struct file* or a file_path_t. Note that file_type must reflect this decision.
  */
int validate_checksum(void* file, const hex_checksum_t checksum, enum checksum_file_type file_type);

#endif