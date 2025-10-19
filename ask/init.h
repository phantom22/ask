#ifndef INIT_H_
#define INIT_H_

#include "filesystem/filesystem.h"

int init_constants() {
    return filesystem_resolve_constants();
}

// int init_filesystem() {
//     mkdir(ROOT_DIR, 0755);

//     file_path_t saved
// }

int init() {
    return 0;
}

void cleanup() {

}

#endif