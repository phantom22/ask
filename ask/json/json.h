#ifndef JSON_H_
#define JSON_H_

#include "../file/file.h"
#include <jansson.h>

struct json_file {
    struct file file;
    json_t *root;
    int is_valid;
};

/** */
int validate_json(struct file f) {
    return 0;
}

#endif