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
#include "verify_checksum.h"
#include "../filesystem/file.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include <string.h>
#include <strings.h>
#include <unistd.h>

int verify_checksum(file_path_t* path, hex_checksum_t* checksum) {
    struct file f = file_open(*path, 0);
    if (f.is_valid == 0) {
        return 0;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (ctx == nullptr || EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1) {
        if (ctx != nullptr)
            EVP_MD_CTX_free(ctx);
        file_close(&f);
        return 0;
    }

    unsigned char buffer[__SHA256_BUFFER_SIZE];
    size_t bytes;
    while ((bytes = read(f.fd, buffer, __SHA256_BUFFER_SIZE))) {
        if (EVP_DigestUpdate(ctx, buffer, bytes) != 1) {
            EVP_MD_CTX_free(ctx);
            file_close(&f);
            return 0;
        }
    }

    unsigned char sha256[EVP_MAX_MD_SIZE];
    if (EVP_DigestFinal_ex(ctx, sha256, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        file_close(&f);
        return 0;
    }

    EVP_MD_CTX_free(ctx);
    file_close(&f);

    hex_checksum_t sha256_hex;
    for (size_t i=0; i<SHA256_DIGEST_LENGTH; ++i)
        snprintf(&sha256_hex[0] + i*2, 3lu, "%02x", sha256[i]);
    sha256_hex[64] = '\0';

    //printf("verify_checksum: {\n  computed:\"%s\",\n  expected:\"%s\"\n}\n", sha256_hex, *checksum);

    return strncmp(sha256_hex, *checksum, HEX_CHECKSUM_SIZE) == 0;
}