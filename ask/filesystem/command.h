#ifndef COMMAND_H_
#define COMMAND_H_

#define COMMAND_SIZE 128u
typedef char command_t[COMMAND_SIZE];

#define CAPTURED_BUFFER_SIZE 8192u
typedef unsigned char command_output_t[CAPTURED_BUFFER_SIZE];

int shell_command(command_t command);
int capture_shell_command(const char* command, command_output_t* output);

#endif