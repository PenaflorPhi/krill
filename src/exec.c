#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "input.h"

char *check_executable(const char *cmd) {
    const char *path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    // Make a writable copy
    char *path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    char *token = strtok(path_copy, ":");
    while (token) {
        char full_path[MAX_BUFFER_SIZE];
        snprintf(full_path, MAX_BUFFER_SIZE, "%s/%s", token, cmd);

        if (access(full_path, X_OK) == 0) { // file exists and is executable
            free(path_copy);
            return strdup(full_path); // caller must free
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

void exec(InputBuffer *input_buffer) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    }

    if (pid == 0) {
        char **argv = calloc(input_buffer->argc + 1, sizeof(char *));
        for (unsigned int i = 0; i < input_buffer->argc; ++i) {
            argv[i] = input_buffer->argv[i];
        }
        argv[input_buffer->argc] = NULL;

        execvp(argv[0], argv);

        fprintf(stderr, "%s exited with failure\n", argv[0]);
        _exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally\n");
        }
    }
}

void handle_executable(InputBuffer *input_buffer) { exec(input_buffer); }
