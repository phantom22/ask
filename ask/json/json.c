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
#include "json.h"
#include "../filesystem/file.h"

#include <unistd.h>
#include <fcntl.h>
#include <jansson.h>

struct json_file json_open(struct file* f) {
    struct json_file j;
    j.file = f;
    j.is_valid = 0;
    j.root = json_loadfd(f->fd, 0, nullptr);
    if (j.root == nullptr)
        j.root = json_object();
    j.is_valid = j.root != nullptr;
    return j;
}


int json_dump(struct json_file* j) { 
    if (file_erase_contents(j->file) == -1)
        return -1;
    return json_dumpfd(j->root, j->file->fd, JSON_INDENT(2));
}

void json_close(struct json_file *j) {
    if (j->is_valid) {
        json_decref(j->root);
        j->is_valid = 0;
    }
}

int json_node_is_boolean(int type) {
    return type == JSON_TRUE || type == JSON_FALSE;
}

int json_node_is_string(int type) {
    return type == JSON_STRING;
}

int json_node_is_array(int type) {
    return type == JSON_ARRAY;
}

int json_get_field(struct json_file* j, const char* key, int(type_check)(int), json_t* new_value, json_t** output) {
    if (output == nullptr) return -1;
    json_t* obj = json_object_get(j->root, key);

    if (obj != nullptr && type_check(obj->type)) {
        *output = obj;
        return 0;
    }

    if (new_value == nullptr)
        return -1;

    json_object_set_new(j->root, key, new_value);

    *output = new_value;
    return 0;
}

/**
 * {
 *   "model":"",
 *   "output_md":true,
 *   "explain_flags":true,
 *   "aliases": [{
 *       key:"",
 *       value:""
 *   },]
 * }
 */

/**
 * {
 *   "prompts": [{
 *     "time":13513950,
 *     "question":"",
 *     "context":"",
 *     "history":"",
 *     "answer":"",
 *     "model":""
 *   },],
 *   "from":"conversation-name"
 * }
 */