#ifndef ASK_H_
#define ASK_H_

// note that both of these defines are also specified
// during compilation from the Makefile
#define _POSIX_C_SOURCE 200809L

#include "filesystem/filesystem.h"
#include "filesystem/file.h"
#include "filesystem/command.h"
#include "init.h"
#include "integrity/verify_checksum.h"


file_path_t ROOT_DIR;
file_path_t SAVE_DIR;
unsigned int ROOT_DIR_LENGTH;
unsigned int SAVE_DIR_LENGTH;
unsigned int MAX_SUBDIRECTORY_SIZE;
unsigned int MAX_CONVERSATION_SIZE;

#endif