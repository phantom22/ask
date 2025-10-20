#include "ask.h"

file_path_t ROOT_DIR;
file_path_t SAVE_DIR;
unsigned int ROOT_DIR_LENGTH;
unsigned int SAVE_DIR_LENGTH;
unsigned int MAX_SUBDIRECTORY_SIZE;
unsigned int MAX_CONVERSATION_SIZE;

int init_constants() {
    return filesystem_resolve_constants();
}

int ask_init() {
    return init_constants();
}

void ask_cleanup() {

}