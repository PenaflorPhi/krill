#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdbool.h>

#include "input.h"

bool check_builtins(char *cmd);
void handle_builtin(InputBuffer *input_buffer);

#endif // !BUILTINS_H
