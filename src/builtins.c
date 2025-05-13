#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

char *builtins[3] = {
    "exit",
    "echo",
    "type",
};

bool check_builtins(char *cmd) {
    for (unsigned long i = 0; i < sizeof(builtins) / sizeof(builtins[0]); ++i) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return true;
        }
    }
    return false;
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
