#include "command.h"

#include <stdlib.h>
#include <stdio.h>

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