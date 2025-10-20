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
#include <cstdio>
#ifndef PROJECT_DIR
#define PROJECT_DIR ""
#endif

#ifndef HOME_DIR
#define HOME_DIR ""
#endif

#ifndef SHELL_SCRIPT_SHA256SUM
#define SHELL_SCRIPT_SHA256SUM ""
#endif

#include <openssl/sha.h>
#include <string.h>

#include <gtest/gtest.h>

// #define ASK_TEST_MODE <-- defined by the Makefile
// necessary for mixing the source code, which is pure C,
// with this C++ file
extern "C" {
    #include "../ask/ask.h"
}

#include "utility/helpers.h"

file_path_t CORRECT_ROOT_DIR;
TEST(FileSystem, ResolvePath) {
    const char *relative_path_a = "hello",
               *relative_path_b = "hello/there",
               *relative_path_c = "";

    file_path_t absolute_path_a,
                absolute_path_b,
                absolute_path_c;

    resolve_root_dir(expected_a, relative_path_a);
    resolve_root_dir(expected_b, relative_path_b);
    resolve_root_dir(expected_c, relative_path_c);
    
    int r1 = filesystem_resolve_path(relative_path_a, &absolute_path_a);
    int r2 = filesystem_resolve_path(relative_path_b, &absolute_path_b);
    int r3 = filesystem_resolve_path(relative_path_c, &absolute_path_c);

    EXPECT_EQ(r1, 0);
    EXPECT_EQ(r2, 0);
    EXPECT_EQ(r3, 0);
    EXPECT_EQ(strcmp(absolute_path_a, expected_a), 0);
    EXPECT_EQ(strcmp(absolute_path_b, expected_b), 0);
    EXPECT_EQ(strcmp(absolute_path_c, expected_c), 0);

    int r4 = filesystem_resolve_path(nullptr, &absolute_path_a);
    int r5 = filesystem_resolve_path(relative_path_a, nullptr);
    int r6 = filesystem_resolve_path(nullptr, nullptr);
    EXPECT_EQ(r4, -1);
    EXPECT_EQ(r5, -1);
    EXPECT_EQ(r6, -1);
}

TEST(FileSystem, ResolvePathFailCondition) {
    file_path_t absolute_path_a;

    create_string_of_size(path_a,MAX_SUBDIRECTORY_SIZE);
    int r1 = filesystem_resolve_path(path_a, &absolute_path_a);
    EXPECT_EQ(r1, 0);

    file_path_t absolute_path_b;

    create_string_of_size(path_b,MAX_SUBDIRECTORY_SIZE+1);
    EXPECT_NE(path_b, nullptr);
    int r2 = filesystem_resolve_path(path_b, &absolute_path_b);
    EXPECT_EQ(r2, -1);
    free(path_b);
}

TEST(FileSystem, IsAllowedPath) {
    EXPECT_EQ(strcmp(ROOT_DIR, CORRECT_ROOT_DIR), 0);

    file_path_t not_allowed = "test/123456";
    EXPECT_EQ(filesystem_is_allowed_path(&not_allowed), 0);

    resolve_root_dir(allowed, "saved");

    EXPECT_EQ(filesystem_is_allowed_path(&allowed), 1);
}

TEST(File, GetName) {
    file_path_t path_a = "a/b/c.txt";
    
    char* fname;
    int r1 = file_get_name(&path_a, &fname);
    EXPECT_EQ(r1, 0);
    EXPECT_EQ(strcmp(fname, "c.txt"), 0);

    file_path_t path_b = "some/path",
                path_c = "some/path/";
    
    char* dname1, *dname2;
    int r2 = file_get_name(&path_b, &dname1);
    int r3 = file_get_name(&path_c, &dname2);
    EXPECT_EQ(r2, 0);
    EXPECT_EQ(r3, 0);
    EXPECT_EQ(strcmp(dname1, dname2), 0);

    file_path_t path_d = "/",
                path_e = "",
                path_f = "//";
    char* dname3, *dname4, *dname5;
    int r4 = file_get_name(&path_d, &dname3);
    int r5 = file_get_name(&path_e, &dname4);
    int r6 = file_get_name(&path_f, &dname5);
    EXPECT_EQ(r4, -1);
    EXPECT_EQ(r5, -1);
    EXPECT_EQ(r6, -1);
    EXPECT_EQ(strcmp(dname3, ""), 0);
    EXPECT_EQ(strcmp(dname4, ""), 0);
    EXPECT_EQ(strcmp(dname5, ""), 0);

    int r7 = file_get_name(&path_b, nullptr);
    int r8 = file_get_name(nullptr, &dname1);
    int r9 = file_get_name(nullptr, nullptr);
    EXPECT_EQ(r7, -1);
    EXPECT_EQ(r8, -1);
    EXPECT_EQ(r9, -1);
}

TEST(File, Open) {
    resolve_project_dir(path_a, "tests/assets/open/file_to_create.txt");
    resolve_project_dir(path_b, "tests/assets/open/existing_empty_file.txt");
    resolve_project_dir(path_c, "tests/assets/open/existing_not_empty_file.txt");

    struct file a = file_open(path_a, 0);
    struct file b = file_open(path_b, 0);
    struct file c = file_open(path_c, 0);

    EXPECT_EQ(a.is_valid, 1);
    EXPECT_NE(a.fd, -1);
    EXPECT_EQ(a.is_empty, 1);
    EXPECT_EQ(a.created, 1);

    EXPECT_EQ(b.is_valid, 1);
    EXPECT_NE(b.fd, -1);
    EXPECT_EQ(b.is_empty, 1);
    EXPECT_EQ(b.created, 0);

    EXPECT_EQ(c.is_valid, 1);
    EXPECT_NE(c.fd, -1);
    EXPECT_EQ(c.is_empty, 0);
    EXPECT_EQ(c.created, 0);

    file_close(&a);
    EXPECT_EQ(a.fd, -1);
    EXPECT_EQ(a.is_valid, 0);
    file_close(&b);
    EXPECT_EQ(b.fd, -1);
    EXPECT_EQ(b.is_valid, 0);
    file_close(&c);
    EXPECT_EQ(c.fd, -1);
    EXPECT_EQ(c.is_valid, 0);

    struct file d = file_open(nullptr, 0);
    EXPECT_EQ(d.is_valid, 0);
    EXPECT_EQ(d.fd, -1);
    EXPECT_EQ(d.is_empty, 1);
}

int main(int argc, char **argv) {
    if (strcmp(HOME_DIR,"") == 0 || strcmp(PROJECT_DIR,"") == 0 || strcmp(SHELL_SCRIPT_SHA256SUM, "") == 0) {
        std::cerr << "The tests must be compiled with:       " << std::endl 
                  << "  -DPROJECT_DIR=\"path/to/project/dir\"" << std::endl 
                  << "  -DHOME_DIR=\"$HOME\"                 " << std::endl
                  << "  -DSHELL_SCRIPT_SHA256SUM=\"<...>\"   " << std::endl
                  << "these defines are handled by default in the Makefile." << std::endl
                  << std:: endl
                  << "Also note that both directories must not have a trailing '/' symbol" << std::endl
                  << "and SHELL_SCRIPT_SHA256SUM refers to the sha256 of \"tests/prepare_assets.sh\"." << std::endl;
        return -1;
    }
    else {
        std::cout << "HOME_DIR: \"" << HOME_DIR << "\"" << std::endl
                  << "PROJECT_DIR: \"" << PROJECT_DIR << "\"" << std::endl
                  << "SHELL_SCRIPT_SHA256SUM: \"" << SHELL_SCRIPT_SHA256SUM << "\"" << std::endl;
        
        snprintf(CORRECT_ROOT_DIR, FILE_PATH_SIZE, "%s/.ask", HOME_DIR);

        std::cout << "CORRECT_ROOT_DIR: \"" << CORRECT_ROOT_DIR << "\"" << std::endl;

        _resolve_project_dir(prepare_assets_sh, "tests/prepare_assets.sh", return 1);

        file_path_t full_command;
        if (snprintf(full_command, FILE_PATH_SIZE, "%s \"%s\"", prepare_assets_sh, PROJECT_DIR) < 0) {
            std::cerr << "Failed to build the command to run 'tests/prepare_assets.sh'." << std::endl;
            return 1;
        }

        //std::cout << "full_command: `"<< full_command << "`" << std::endl;

        if (system(full_command) == 1) {
            std::cerr << "The shell scrit 'tests/prepare_assets.sh' failed." << std::endl;
            return 1;
        }

        //std::cout << "full_command: `"<< full_command << "`" << std::endl;

        hex_checksum_t prepare_assets_hashsum = SHELL_SCRIPT_SHA256SUM;
        if (!verify_checksum(&prepare_assets_sh, &prepare_assets_hashsum)) {
            std::cerr << "Checksum verification failed on 'prepare_assets.sh'." << std::endl
                      << "Either re-compile with `make rebuildhash cleantests` or find the correct version of the file" << std::endl;
            return 1;
        }

        if (system(full_command) == 1) {
            std::cerr << "The shell scrit 'tests/prepare_assets.sh' failed." << std::endl;
            return 1;
        }
    }

    int init_status = ask_init();
    if (init_status == -1) {
        std::cerr << "Failed to initialize ask." << std::endl;
        exit(1);
    }
    else {
        std::cout << "ask initialized properly." << std::endl;
    }

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}