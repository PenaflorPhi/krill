#ifndef EXEC_H
#define EXEC_H

#include "input.h"

char *check_executable(char *cmd);
void  handle_executable(InputBuffer *input_buffer);

#endif // !EXEC_H
