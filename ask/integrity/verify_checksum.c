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