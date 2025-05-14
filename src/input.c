#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "exec.h"
#include "input.h"

InputBuffer Create_Input_Buffer() {
    InputBuffer input_buffer;

    input_buffer.argc          = 0;
    input_buffer.input_size    = 0;
    input_buffer.builtin       = false;
    input_buffer.is_executable = false;

    return input_buffer;
}

void Clear_Input_Buffer(InputBuffer *input_buffer) {
    input_buffer->input[0]      = '\0';
    input_buffer->argc          = 0;
    input_buffer->input_size    = 0;
    input_buffer->builtin       = false;
    input_buffer->is_executable = false;
    input_buffer->program       = NULL;

    for (int i = 0; i < MAX_ARGS; ++i) {
        input_buffer->argv[i] = NULL;
    }
}

uint8_t capture_input(InputBuffer *input_buffer) {
    if (!fgets(input_buffer->input, MAX_BUFFER_SIZE, stdin))
        return 0;

    input_buffer->input[strcspn(input_buffer->input, "\n")] = '\0';
    input_buffer->input_size                                = strlen(input_buffer->input);

    input_buffer->argc = 0;
    char *token        = strtok(input_buffer->input, " ");

    while (token && input_buffer->argc < MAX_ARGS) {
        input_buffer->argv[input_buffer->argc++] = token;
        token                                    = strtok(NULL, " ");
    }

    return input_buffer->input_size;
}

void print_input_buffer(InputBuffer *input_buffer) {
    printf("input: %s\n", input_buffer->input);
    printf("input_size: %d\n", input_buffer->input_size);
    printf("argc: %u\n", input_buffer->argc);
    printf("builtin: %d\n", input_buffer->builtin);
    printf("exec: %d\n", input_buffer->is_executable);
    printf("program: %s\n", input_buffer->program);
    printf("args:\n");
    for (unsigned int i = 0; i < input_buffer->argc; ++i) {
        printf("\targ %d: %s\n", i, input_buffer->argv[i]);
    }
}

bool check_input(InputBuffer *input_buffer) {
    if ((input_buffer->builtin = check_builtins(input_buffer->argv[0])) == true) {
        input_buffer->is_executable = false;
        return true;
    } else if ((input_buffer->program = check_executable(input_buffer->argv[0]))) {
        input_buffer->is_executable = true;
        return true;
    }

    return false;
}
