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
#include "validate_checksum.h"
#include "../filesystem/file.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include <string.h>
#include <strings.h>
#include <unistd.h>

#define __SHA256_BUFFER_SIZE 4096lu

int sha256sum(struct file* f, hex_checksum_t* output) {
    if (file_is_invalid(f) || output == nullptr)
        return -1;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (ctx == nullptr || EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1) {
        if (ctx != nullptr)
            EVP_MD_CTX_free(ctx);
        file_close(f);
        return -1;
    }

    unsigned char buffer[__SHA256_BUFFER_SIZE];
    size_t bytes;
    while ((bytes = read(f->fd, buffer, __SHA256_BUFFER_SIZE))) {
        if (EVP_DigestUpdate(ctx, buffer, bytes) != 1) {
            EVP_MD_CTX_free(ctx);
            file_close(f);
            return -1;
        }
    }

    unsigned char sha256[EVP_MAX_MD_SIZE];
    if (EVP_DigestFinal_ex(ctx, sha256, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        file_close(f);
        return -1;
    }

    EVP_MD_CTX_free(ctx);

    for (size_t i=0; i<SHA256_DIGEST_LENGTH; ++i) {
        if (snprintf(*output + i*2, 3lu, "%02x", sha256[i]) < 0) {
            file_close(f);
            return -1;
        }
    }
    *(*output + 64) = '\0';

    return 0;
}

int validate_checksum(void* file, const hex_checksum_t expected_checksum, enum checksum_file_type file_type) {
    if (file == nullptr)
        return -1;

    struct file f;
    switch (file_type) {
        case path_to_file:
            f = file_open(((char*)file), 0);
            break;
        case struct_file_ptr:
            f = *((struct file*) file);
            break;
        default:
            return -1;
    }

    if (f.is_valid != 1 || f.fd == -1) {
        return -1;
    }

    hex_checksum_t real_checksum;
    if (sha256sum(&f, &real_checksum) == -1)
        return 0;

    if (file_type == path_to_file)
        file_close(&f);

    return strncmp(real_checksum, expected_checksum, HEX_CHECKSUM_SIZE) == 0;
}