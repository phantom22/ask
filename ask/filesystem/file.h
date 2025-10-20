#ifndef FILE_H_
#define FILE_H_

#include "filesystem.h"

struct file {
    int fd;
    file_path_t full_path;
    // pointer to char in full_path
    char* name;
    int created;
    int is_empty;
    int is_valid;
};

int file_get_name(file_path_t* p, char** fname);
int file_touch(struct file f);
void file_close(struct file *f);
struct file file_open(const char *rpath, int open_flags);
int file_initialize_if_empty(struct file *f, const char* with);

#endif