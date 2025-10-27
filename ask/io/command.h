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
#ifndef COMMAND_H_
#define COMMAND_H_

/** Only affects the last output char (before the null terminator), if said char is '\n'
  *  the whole output gets shortened by one byte, replacing the new line character
  *  with a null terminator.
  */
#define remove_trailing_new_line 1u
/** Each new line character contained in the output will be replaced by a null terminator. */
#define null_terminate_lines 2u
/** output_line_indices will be an array of all the characters that are at the beginning of a line.
  * Note that if you free the output, these pointers will point to an invalid memory region.
  */
#define indices_as_pointers 4u
/** Filter out all the empty lines that basically only consist of {'\n','\0\}.
  * This will also prevent those lines from contributing to the total line count.
  */
#define ignore_empty_lines 8u
/** On success output will point to a malloc'ed char* that contains the full output of the specified command. Returns -1 on fail.
  * output_size, output_line_count, output_lines and output_line_indices can be omitted by passing a nullptr.
  *
  * If output_line_indices is not a nullptr, then it will be malloc'ed and will need to be freed afterwards,
  *   just like with output.
  *
  * The default output type for the output_line_indices is (unsigned long*), but with the
  *   indices_as_pointers it will be (char**).
  * remove_trailing_new_line, null_terminate_lines, indices_as_pointers, ignore_empty_lines are available flags.
  *
  * If indices_as_pointers was specified: after freeing output, all the pointers in output_line_indices will point
  *   to an invalid region in memory. 
  *
  * Finally if output was empty, verifiable by checking either output or
  *   output_line_count == 0, output_line_indices will be freed automatically.
  */
int capture_shell_command(
    const char* command, 
    char** output, unsigned long* output_size,
    unsigned long* output_line_count, void* output_line_indices, 
    unsigned int flags
);

#endif