#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "exec.h"
#include "input.h"

int main(void) {
    setbuf(stdout, NULL);

    printf("$ ");
    InputBuffer input_buffer = Create_Input_Buffer();

    while (capture_input(&input_buffer)) {
        check_input(&input_buffer);
        if (input_buffer.builtin) {
            handle_builtin(&input_buffer);
        } else if (input_buffer.is_executable) {
            handle_executable(&input_buffer);
        } else {
            printf("%s: command not found\n", input_buffer.input);
        }

        Clear_Input_Buffer(&input_buffer);

        printf("$ ");
    }
    return EXIT_SUCCESS;
}
