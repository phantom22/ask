#ifndef ASK_H_
#define ASK_H_

// note that both of these defines are also specified
// during compilation from the Makefile
#define _POSIX_C_SOURCE 200809L

#include "filesystem/filesystem.h"
#include "filesystem/file.h"
#include "filesystem/command.h"
#include "integrity/verify_checksum.h"

int ask_init();
void ask_cleanup();

#endif