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
#ifndef UTILITY_H_
#define UTILITY_H_

#define _STRINGIFY(x) #x
#define _VARNAME_TO_STRING(x) _STRINGIFY(x)

#define _resolve_root_dir(varname, rpath, how_to_exit) \
    file_path_t varname; \
    if (snprintf(varname, SIZE_FILE_PATH, "%s/%s", DIR_CORRECT_ROOT, rpath) < 0) { \
        printf("resolve_root_dir: snprintf failed on variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        how_to_exit; \
    }

#define resolve_root_dir(varname, rpath) _resolve_root_dir(varname, rpath, FAIL() << "snprintf failed")

#define _resolve_project_dir(varname, rpath, how_to_exit) \
    file_path_t varname; \
    if (snprintf(varname, SIZE_FILE_PATH, "%s/%s", PROJECT_DIR, rpath) < 0) { \
        printf("resolve_project_dir: snprintf failed on variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        how_to_exit; \
    }

#define resolve_project_dir(varname, rpath) _resolve_project_dir(varname, rpath, FAIL() << "snprintf failed")

#define create_string_of_size(varname,size) \
    char *varname = (char*)malloc(size); \
    if (varname == nullptr) { \
        printf("create_string_of_size: failed to malloc string variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        FAIL() << "malloc failed"; \
    } \
    for (int i=0; i<size-1; ++i) \
        varname[i] = 'a'; \
    varname[size-1] = '\0'

#endif