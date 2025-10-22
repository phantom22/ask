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
#include "file.h"
#include "filesystem.h"

#include <unistd.h> // open
#include <fcntl.h> // close
#include <sys/stat.h> // futimens, fstat
#include <string.h>
#include <time.h>

int file_get_name(file_path_t* p, char** fname) {
    if (p == nullptr || fname == nullptr) return -1;
    
    unsigned int l = 0, // path length
             po = 0, // previous occurrence
             o = 0; // occurrence
    int has_slash = 0;
    char c;
    for (; l<SIZE_FILE_PATH; ++l) {
        c = *(*p + l);
        if (c == '/')  {
            has_slash = 1;
            po = o;
            o = l;
            continue;
        }
        if (c == '\0')
            break;
    }
    if (has_slash) {
        if (o < l-1)
            *fname = *p + o + 1;
        else {
            *(*p + o) = '\0';
            *fname = *p + po + 1;
        }
    }
    else
        *fname = *p;

    return -(*fname[0] == '\0'); // -1 if string is empty, else 0
}

/** Return -1 on fail. */
int file_touch(struct file f) {
    struct timespec ts[2];
    ts[1].tv_sec = ts[0].tv_sec = time(NULL);
    ts[1].tv_nsec = ts[0].tv_nsec = 0;

    return futimens(f.fd, ts);
}

/** f assumed to be an opened file. */
inline void file_close(struct file *f) {
    close(f->fd);
    f->fd = -1;
    f->is_valid = 0;
}

/** Retrieves/creates the file specified by fname. */
struct file file_open(const char *rpath, int open_flags) {
    struct file f;
    if (rpath == nullptr) {
        f.fd = -1;
        f.is_valid = 0;
        f.is_empty = 1;
        return f;
    }

    file_path_t fpath;
    
    #ifndef ASK_TEST_MODE
        if (filesystem_resolve_path(rpath, &fpath) == -1) {
            f.fd = -1;
            f.is_valid = 0;
            f.is_empty = 1;
            return f;
        }
    #else
        memcpy(fpath, rpath, SIZE_FILE_PATH);
    #endif

    memcpy(f.full_path, fpath, SIZE_FILE_PATH);
    
    file_get_name(&fpath, &f.name);

    f.created = access(fpath, F_OK) != 0;
    // 0644 required by O_CREAT
    f.fd = open(fpath, O_RDWR | O_CREAT | open_flags, 0644);
    if (f.fd == -1) {
        f.is_valid = 0;
        f.is_empty = 1;
        return f;
    }

    struct stat st;
    if (fstat(f.fd, &st) == 0) {
        f.is_empty = st.st_size == 0;
        f.is_valid = 1;
    }
    else {
        f.is_empty = 1;
        f.is_valid = 0;
        file_close(&f);
    }
    
    return f;
}

/** Writes to the file described by f only if said file is empty. */
int file_initialize_if_empty(struct file *f, const char* with) {
    if (with == nullptr)
        return -1;

    if (f->is_valid == 0)
        return -1;

    unsigned long wlength = strlen(with);
    if (f->is_empty && wlength > 0) {
        if (write(f->fd, with, wlength) == -1) {
            file_close(f);
            return -1;
        }
    }
    return 0;
}