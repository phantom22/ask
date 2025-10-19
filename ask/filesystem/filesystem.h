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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int filesystem_resolve_constants() {
    char* home = getenv("HOME");
    int r_length = snprintf(ROOT_DIR, FILE_PATH_SIZE, "%s/.ask", home),
        s_length = snprintf(SAVE_DIR, FILE_PATH_SIZE, "%s/saved", ROOT_DIR);
    //sprintf(ROOT_DIR, "%s/.ask", home);
    if (r_length < 0 || s_length < 0) {
        ROOT_DIR_LENGTH = 0;
        SAVE_DIR_LENGTH = 0;
        return -1;
    }
    ROOT_DIR_LENGTH = r_length;
    SAVE_DIR_LENGTH = s_length;
    MAX_SUBDIRECTORY_SIZE = FILE_PATH_SIZE - r_length - 1;
    MAX_CONVERSATION_SIZE = FILE_PATH_SIZE - s_length - 1;

    #ifdef ASK_PRINT_GLOBALS
    printf("ROOT_DIR: \"%s\"\nSAVE_DIR: \"%s\"\nFILE_PATH_SIZE: %i\nROOT_DIR_LENGTH: %i\nSAVE_DIR_LENGTH: %i\nMAX_SUBDIRECTORY_SIZE: %i\nFILE_PATH_SIZE: %i\n", ROOT_DIR, SAVE_DIR, FILE_PATH_SIZE, ROOT_DIR_LENGTH, SAVE_DIR_LENGTH, MAX_SUBDIRECTORY_SIZE, FILE_PATH_SIZE);
    #endif
    
    return 0;
}

/** Returns -1 on fail. */
int filesystem_resolve_path(const char* rel_path, file_path_t* output) {
    if (rel_path == nullptr || output == nullptr) return -1;
    else if (strlen(rel_path) > MAX_SUBDIRECTORY_SIZE - 1) return -1;
    return -(snprintf(*output, FILE_PATH_SIZE, "%s/%s", ROOT_DIR, rel_path) < 0);
}

int filesystem_is_allowed_path(file_path_t* p) {
    return strstr(*p, ROOT_DIR) == *p;
}

#endif