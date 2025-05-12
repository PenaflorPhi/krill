#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS        16

char *builtins[3] = {
    "exit",
    "echo",
    "type",
};

typedef struct {
    char         input[MAX_BUFFER_SIZE];
    uint8_t      input_size;
    bool         valid_input;
    char        *argv[MAX_ARGS];
    unsigned int argc;
    bool         builtin;
} InputBuffer;

InputBuffer Create_Input_Buffer() {
    InputBuffer input_buffer;

    input_buffer.argc        = 0;
    input_buffer.input_size  = 0;
    input_buffer.valid_input = false;
    input_buffer.builtin     = false;

    return input_buffer;
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

bool check_builtins(char *cmd) {
    for (unsigned long i = 0; i < sizeof(builtins) / sizeof(builtins[0]); ++i) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool check_input(InputBuffer *input_buffer) {
    input_buffer->builtin = check_builtins(input_buffer->argv[0]);

    if (input_buffer->builtin) {
        input_buffer->valid_input = true;
    }

    return input_buffer->valid_input;
}

void echo(char **argv, int argc) {
    for (int i = 1; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    printf("\n");
}
void type(char **argv, int argc) {
    if (argc == 1) {
        puts("");
        return;
    }

    if (check_builtins(argv[1])) {
        printf("%s is a shell builtin\n", argv[1]);
    } else {
        printf("%s: not found\n", argv[1]);
    }
}

void handle_builtin(InputBuffer *input_buffer) {
    if (strcmp("exit", input_buffer->argv[0]) == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp("echo", input_buffer->argv[0]) == 0) {
        echo(input_buffer->argv, input_buffer->argc);
    } else if (strcmp("type", input_buffer->argv[0]) == 0) {
        type(input_buffer->argv, input_buffer->argc);
    }
}

void print_input_buffer(InputBuffer *input_buffer) {
    printf("input: %s\n", input_buffer->input);
    printf("input_size: %d\n", input_buffer->input_size);
    printf("valid_input: %d\n", input_buffer->valid_input);

    printf("argc: %u\n", input_buffer->argc);
    printf("builtin: %d\n", input_buffer->builtin);
}

int main(void) {
    setbuf(stdout, NULL);

    printf("$ ");
    InputBuffer input_buffer = Create_Input_Buffer();

    while (capture_input(&input_buffer)) {
        check_input(&input_buffer);
        if (input_buffer.valid_input) {
            if (input_buffer.builtin) {
                handle_builtin(&input_buffer);
            }
        } else {
            printf("%s: command not found\n", input_buffer.input);
        }

        printf("$ ");
    }
    return EXIT_SUCCESS;
}
