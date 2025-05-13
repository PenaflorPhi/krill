#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "input.h"
#include "path.h"

InputBuffer Create_Input_Buffer() {
    InputBuffer input_buffer;

    input_buffer.argc       = 0;
    input_buffer.input_size = 0;
    input_buffer.builtin    = false;
    input_buffer.on_path    = false;

    return input_buffer;
}

void Clear_Input_Buffer(InputBuffer *input_buffer) {
    input_buffer->input[0]   = '\0';
    input_buffer->argc       = 0;
    input_buffer->input_size = 0;
    input_buffer->builtin    = false;
    input_buffer->on_path    = false;
    input_buffer->program    = NULL;

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
}

bool check_input(InputBuffer *input_buffer) {
    if ((input_buffer->builtin = check_builtins(input_buffer->argv[0])) == true) {
        return true;
    } else if ((input_buffer->program = check_on_path(input_buffer->argv[0]))) {
        input_buffer->on_path = true;
        return true;
    }

    return false;
}
