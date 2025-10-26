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
#include <jansson.h>
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
#include "../ask/ask.h"

#include "utility/helpers.h"

file_path_t DIR_CORRECT_ROOT,
            DIR_CORRECT_SAVED,
            FILE_CORRECT_DOT_ASK,
            FILE_CORRECT_DATA_JSON;


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

    create_string_of_size(path_a,SIZE_MAX_SUBDIRECTORY);
    int r1 = filesystem_resolve_path(path_a, &absolute_path_a);
    EXPECT_EQ(r1, 0);

    file_path_t absolute_path_b;

    create_string_of_size(path_b,SIZE_MAX_SUBDIRECTORY+1);
    EXPECT_NE(path_b, nullptr);
    int r2 = filesystem_resolve_path(path_b, &absolute_path_b);
    EXPECT_EQ(r2, -1);
    free(path_b);
}

TEST(FileSystem, IsAllowedPath) {
    EXPECT_EQ(strcmp(DIR_ROOT, DIR_CORRECT_ROOT), 0);

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
    resolve_project_dir(path_a, "tests/assets/file_open/file_to_create.txt");
    resolve_project_dir(path_b, "tests/assets/file_open/existing_empty_file.txt");
    resolve_project_dir(path_c, "tests/assets/file_open/existing_not_empty_file.txt");

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

TEST(File, InitializeIfEmpty) {
    resolve_project_dir(path_a, "tests/assets/file_initialize/file_a.txt");
    resolve_project_dir(path_b, "tests/assets/file_initialize/file_b.txt");

    struct file fa = file_open(path_a, 0);
    ASSERT_EQ(fa.is_valid, 1);
    struct file fb = file_open(path_b, 0);
    ASSERT_EQ(fb.is_valid, 1);

    char fa_content_initialization[] = "just some text";
    file_initialize_if_empty(&fa, fa_content_initialization);

    char* fa_contents;
    ASSERT_EQ(file_get_contents(&fa, &fa_contents), 0);
    
    EXPECT_EQ(strcmp(fa_contents, fa_content_initialization), 0);

    file_close(&fa);
    file_close(&fb);
}

TEST(Integrity, sha256sum) {
    resolve_project_dir(path_a, "tests/assets/checksum/file1.txt");
    resolve_project_dir(path_b, "tests/assets/checksum/file2.txt");
    resolve_project_dir(path_c, "tests/assets/checksum/file3.txt");

    EXPECT_EQ(validate_checksum(path_a, "4eb22759388dd8f1e0dfeaffcb51bd4d7167a1eb6fd26258cdec98689844c378", path_to_file),1);
    EXPECT_EQ(validate_checksum(path_b, "fe0f065a3dbc9a9df275204ed5157589461b8d8107d7e43b7f0a9e0f64d23ea9", path_to_file), 1);
    EXPECT_EQ(validate_checksum(path_c, "e2aa2d21f6c11079c4f5b5eb5ed6d121b662d3e7724ac10d59fbfb82437f602d", path_to_file), 1);
}

TEST(Command, CaptureShellCommand) {
    char test_output1[] = "JUST a TeSt.";
    test_command(full_cmd1, output1, size1, line_count1,
        unsigned long*, indices1,
        "JUST a TeSt.", 0,
        test_output1, 1,
        EXPECT_EQ(indices1[0], 0)
    );

    test_command(
        full_cmd2, output2, size2, line_count2, 
        unsigned long*, indices2, 
        "JUST a TeSt.\n", remove_trailing_new_line,
        test_output1, 1,
        EXPECT_EQ(indices2[0], 0)
    );

    test_command(
        full_cmd3, output3, size3, line_count3, 
        unsigned long*, indices3, 
        "$HOME\n", remove_trailing_new_line,
        HOME_DIR, 1, 
        EXPECT_EQ(indices3[0], 0)
    );

    test_command(
        full_cmd4, output4, size4, line_count4, 
        unsigned long*, indices4, 
        "Multi\nline\noutput\njust\nfor\nfun!\n", remove_trailing_new_line,
        "Multi\nline\noutput\njust\nfor\nfun!", 6, 
        EXPECT_EQ(indices4[0], 0);
        EXPECT_EQ(indices4[1], 6);
        EXPECT_EQ(indices4[2], 11);
        EXPECT_EQ(indices4[3], 18);
        EXPECT_EQ(indices4[4], 23);
        EXPECT_EQ(indices4[5], 27);
    );

    test_command(
        full_cmd5, output5, size5, line_count5, 
        char**, pointers5, 
        "A\nfew\nlines!\n", remove_trailing_new_line | indices_as_pointers,
        "A\nfew\nlines!", 3, 
        EXPECT_EQ(pointers5[0], &output5[0]);
        EXPECT_EQ(pointers5[1], &output5[2]);
        EXPECT_EQ(pointers5[2], &output5[6]);
    );

    test_command_no_indices(
        full_cmd6, output6, size6, line_count6, 
        "A\nB\nC\nD\nE!\n", null_terminate_lines,
        "A\0B\0C\0D\0E!", 5
    );

    test_command_no_indices(
        full_cmd7, output7, size7, line_count7, 
        "A\nB\nC!", null_terminate_lines,
        "A\0B\0C!", 3
    );

    test_command_no_indices(
        full_cmd8, output8, size8, line_count8, 
        "", null_terminate_lines,
        "", 0
    );

    test_command_no_indices(
        full_cmd9, output9, size9, line_count9, 
        " ", null_terminate_lines,
        " ", 1
    );

    test_command_no_indices(
        full_cmd10, output10, size10, line_count10, 
        "\n\n\n\n", ignore_empty_lines,
        "", 0
    );

    test_command_no_indices(
        full_cmd11, output11, size11, line_count11, 
        "\n\n\n\na\n\n\nb\n\n\nc\n\nd\ne\n", ignore_empty_lines,
        "a\nb\nc\nd\ne\n", 5
    );

    test_command_no_indices(
        full_cmd12, output12, size12, line_count12, 
        "\n\n\n\na\n\n\nb\n\n\nc\n\nd\ne\n", ignore_empty_lines | remove_trailing_new_line,
        "a\nb\nc\nd\ne", 5
    );

    test_command_no_indices(
        full_cmd13, output13, size13, line_count13, 
        "\n\n\n\na\n\n\nb\n\n\nc\n\nd\ne\n", ignore_empty_lines | remove_trailing_new_line | null_terminate_lines,
        "a\0b\0c\0d\0e", 5
    );

    test_command(
        full_cmd14, output14, size14, line_count14, 
        char**, pointers14, 
        "\n\n\n\na\n\n\nb\n\n\nc\n\nd\ne\nfg\n\nh\n\n\ni\n\n\n\nj\n", ignore_empty_lines | remove_trailing_new_line | indices_as_pointers | null_terminate_lines,
        "a\0b\0c\0d\0e\0fg\0h\0i\0j", 9, 
        EXPECT_EQ(pointers14[0], &output14[0]);
        EXPECT_EQ(pointers14[1], &output14[2]);
        EXPECT_EQ(pointers14[2], &output14[4]);
        EXPECT_EQ(pointers14[3], &output14[6]);
        EXPECT_EQ(pointers14[4], &output14[8]);
        EXPECT_EQ(pointers14[5], &output14[10]);
        EXPECT_EQ(pointers14[6], &output14[13]);
        EXPECT_EQ(pointers14[7], &output14[15]);
        EXPECT_EQ(pointers14[8], &output14[17]);
    );
}

TEST(Json, Open) {
    resolve_project_dir(path_a, "tests/assets/json/valid.json");
    resolve_project_dir(path_b, "tests/assets/json/invalid.json");

    struct file fa = file_open(path_a, 0);
    ASSERT_EQ(fa.is_valid, 1);
    struct json_file ja = json_open(&fa);
    struct file fb = file_open(path_b, 0);
    ASSERT_EQ(fb.is_valid, 1);
    struct json_file jb = json_open(&fb);

    EXPECT_EQ(ja.is_valid, 1);
    EXPECT_EQ(jb.is_valid, 1);

    json_t *valid_bool1, *valid_bool2, *invalid_bool1, *invalid_bool2;
    json_get_boolean(&ja, "valid_bool1", 0, &valid_bool1);
    json_get_boolean(&ja, "valid_bool2", 1, &valid_bool2);
    json_get_boolean(&ja, "invalid_bool1", 1, &invalid_bool1);
    json_get_boolean(&ja, "invalid_bool2", 0, &invalid_bool2);

    EXPECT_EQ(valid_bool1->type, JSON_TRUE);
    EXPECT_EQ(valid_bool2->type, JSON_FALSE);
    EXPECT_EQ(invalid_bool1->type, JSON_TRUE);
    EXPECT_EQ(invalid_bool2->type, JSON_FALSE);
    EXPECT_EQ(json_boolean_value(valid_bool1), 1);
    EXPECT_EQ(json_boolean_value(valid_bool2), 0);
    EXPECT_EQ(json_boolean_value(invalid_bool1), 1);
    EXPECT_EQ(json_boolean_value(invalid_bool2), 0);

    json_t *valid_string1, *valid_string2, *invalid_string1, *invalid_string2;
    json_get_string(&ja, "valid_string1", "default 1", &valid_string1);
    json_get_string(&ja, "valid_string2", "default 2", &valid_string2);
    json_get_string(&ja, "invalid_string1", "default 3", &invalid_string1);
    json_get_string(&ja, "invalid_string2", "default 4", &invalid_string2);

    EXPECT_EQ(valid_string1->type, JSON_STRING);
    EXPECT_EQ(valid_string2->type, JSON_STRING);
    EXPECT_EQ(invalid_string1->type, JSON_STRING);
    EXPECT_EQ(invalid_string2->type, JSON_STRING);
    EXPECT_EQ(strcmp(json_string_value(valid_string1),""), 0);
    EXPECT_EQ(strcmp(json_string_value(valid_string2),"test string"), 0);
    EXPECT_EQ(strcmp(json_string_value(invalid_string1),"default 3"), 0);
    EXPECT_EQ(strcmp(json_string_value(invalid_string2),"default 4"), 0);

    json_t *valid_array, *invalid_array;
    json_get_array(&ja, "valid_array", &valid_array);
    json_get_array(&ja, "invalid_array", &invalid_array);
    EXPECT_EQ(valid_array->type, JSON_ARRAY);
    EXPECT_EQ(invalid_array->type, JSON_ARRAY);

    json_close(&ja);
    EXPECT_EQ(ja.is_valid, 0);

    json_t *before_field_a, *before_field_b, *before_field_c;
    json_get_boolean(&jb, "field_a", 1, &before_field_a);
    json_get_string(&jb, "field_b", "my string value", &before_field_b);
    json_get_array(&jb, "field_c", &before_field_c);

    EXPECT_EQ(before_field_a->type, JSON_TRUE);
    EXPECT_EQ(before_field_b->type, JSON_STRING);
    EXPECT_EQ(before_field_c->type, JSON_ARRAY);

    EXPECT_EQ(json_dump(&jb), 0);
    json_close(&jb);
    EXPECT_EQ(jb.is_valid, 0);

    file_close(&fa);
    EXPECT_EQ(fa.is_valid, 0);
    file_close(&fb);
    EXPECT_EQ(fb.is_valid, 0);

    struct file fb_again = file_open(path_b, 0);
    ASSERT_EQ(fb_again.is_valid, 1);
    struct json_file jb_again = json_open(&fb_again);
    EXPECT_EQ(jb_again.is_valid, 1);

    json_t *after_field_a, *after_field_b, *after_field_c;
    json_get_boolean(&jb_again, "field_a", 0, &after_field_a);
    json_get_string(&jb_again, "field_b", "not my string value", &after_field_b);
    json_get_array(&jb_again, "field_c", &after_field_c);

    EXPECT_EQ(after_field_a->type, JSON_TRUE);
    EXPECT_EQ(after_field_b->type, JSON_STRING);
    EXPECT_EQ(after_field_c->type, JSON_ARRAY);

    EXPECT_EQ(json_boolean_value(after_field_a), 1);
    EXPECT_EQ(strcmp(json_string_value(after_field_b),"my string value"), 0);

    json_close(&jb_again);
    EXPECT_EQ(jb_again.is_valid, 0);
    file_close(&fb_again);
    EXPECT_EQ(fb_again.is_valid, 0);
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
        std::cout << "HOME_DIR: '" << HOME_DIR << "'" << std::endl
                  << "PROJECT_DIR: '" << PROJECT_DIR << "'" << std::endl
                  << "SHELL_SCRIPT_SHA256SUM: '" << SHELL_SCRIPT_SHA256SUM << "'" << std::endl;
        
        int r1 = snprintf(DIR_CORRECT_ROOT, SIZE_FILE_PATH, "%s/.ask", HOME_DIR),
            r2 = snprintf(DIR_CORRECT_SAVED, SIZE_FILE_PATH, "%s/saved", DIR_CORRECT_ROOT),
            r3 = snprintf(FILE_CORRECT_DOT_ASK, SIZE_FILE_PATH, "%s/.ask", DIR_CORRECT_ROOT),
            r4 = snprintf(FILE_CORRECT_DATA_JSON, SIZE_FILE_PATH, "%s/data.json", DIR_CORRECT_ROOT);

        if (r1 < 0 || r2 < 0 || r3 < 0 || r4 < 0) {
            std::cerr << "snprintf failed to resolve the correct dirs!" << std::endl;
            return 1;
        }

        std::cout << "DIR_CORRECT_ROOT: '" << DIR_CORRECT_ROOT << "'" << std::endl 
                  << "DIR_CORRECT_SAVED: '" << DIR_CORRECT_SAVED << "'" << std::endl 
                  << "FILE_CORRECT_DOT_ASK: '" << FILE_CORRECT_DOT_ASK << "'" << std::endl 
                  << "FILE_CORRECT_DATA_JSON: '" << FILE_CORRECT_DATA_JSON << "'" << std::endl;

        _resolve_project_dir(prepare_assets_sh, "tests/prepare_assets.sh", return 1);

        file_path_t full_command;
        if (snprintf(full_command, SIZE_FILE_PATH, "%s \"%s\"", prepare_assets_sh, PROJECT_DIR) < 0) {
            std::cerr << "snprintf failed to build the command to run 'tests/prepare_assets.sh'." << std::endl;
            return 1;
        }
        else if (system(full_command) == 1) {
            std::cerr << "The shell scrit 'tests/prepare_assets.sh' failed." << std::endl;
            return 1;
        }
        
        if (!validate_checksum(&prepare_assets_sh, SHELL_SCRIPT_SHA256SUM, path_to_file)) {
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