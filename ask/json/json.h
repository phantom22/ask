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