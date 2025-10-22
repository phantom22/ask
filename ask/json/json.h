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

struct file;
struct json_t;

struct json_file {
    struct file* file;
    struct json_t* root;
    int is_valid;
};

struct json_file json_open(struct file* f);
int json_dump(struct json_file* j);
void json_close(struct json_file* j);

int json_node_is_boolean(int);
int json_node_is_string(int);
int json_node_is_array(int);
int json_get_field(struct json_file* j, const char* key, int(type_check)(int), struct json_t* new_value, struct json_t** output);

#define json_get_boolean(j,key,default_value,output_var_ptr) json_get_field(j,key,json_node_is_boolean,json_boolean(default_value),output_var_ptr)
#define json_get_string(j,key,default_value,output_var_ptr) json_get_field(j,key,json_node_is_string,json_string(default_value),output_var_ptr)
#define json_get_array(j,key,output_var_ptr) json_get_field(j,key,json_node_is_array,json_array(),output_var_ptr)

#endif