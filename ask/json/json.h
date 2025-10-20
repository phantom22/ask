#ifndef JSON_H_
#define JSON_H_

#include "../filesystem/file.h"
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

// json_t *root;
// json_error_t error;
// root = json_load_file("bin/test.json", 0, &error);

// json_t *name = json_object_get(root, "name");
// const char *name_str = json_string_value(name);

// json_object_set_new(root, "name", json_string("new name"));
// json_dump_file(root, "bin/new_test.json", JSON_INDENT(2));

// json_decref(root);

#endif