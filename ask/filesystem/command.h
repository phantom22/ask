#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdlib.h>
#include <stdio.h>

#define COMMAND_SIZE 128u
typedef char command_t[COMMAND_SIZE];

#define CAPTURED_BUFFER_SIZE 8192u
typedef unsigned char command_output_t[CAPTURED_BUFFER_SIZE];

inline int shell_command(command_t command) {
    return system(command);
}

inline int capture_shell_command(const char* command, command_output_t* output) {
    if (output == nullptr) return -1;
    
    FILE* fp = popen(command, "r");

    size_t nbytes;
    while ((nbytes = fread(*output, 1, CAPTURED_BUFFER_SIZE, fp)) > 0) {}

    pclose(fp);

    return 0;
}

#endif