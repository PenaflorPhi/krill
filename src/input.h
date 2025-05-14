#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS        16

typedef struct {
    char         input[MAX_BUFFER_SIZE];
    uint8_t      input_size;
    char        *argv[MAX_ARGS];
    unsigned int argc;
    bool         builtin;
    bool         is_executable;
    char        *program;
} InputBuffer;

InputBuffer Create_Input_Buffer();
bool        check_input(InputBuffer *input_buffer);
uint8_t     capture_input(InputBuffer *input_buffer);
void        Clear_Input_Buffer(InputBuffer *input_buffer);
void        print_input_buffer(InputBuffer *input_buffer);
#endif // !INPUT_H
