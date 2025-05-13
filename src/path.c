#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

char *check_on_path(const char *cmd) {
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
