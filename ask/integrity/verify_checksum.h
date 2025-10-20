#ifndef VERIFY_CHECKSUM_H_
#define VERIFY_CHECKSUM_H_
#include "../filesystem/filesystem.h"

#define __SHA256_BUFFER_SIZE 4096lu
#define HEX_CHECKSUM_SIZE 65lu
typedef char hex_checksum_t[HEX_CHECKSUM_SIZE];

int verify_checksum(file_path_t* path, hex_checksum_t* checksum);

#endif