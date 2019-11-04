#include "errors.h"
#include <stdio.h>
#include "types.h"

char* errors[MAX_ERRORS] = {
    "Ok",
    "Invalid command"
};

void show_error(status_t st) {
    fprintf(stderr, "%s\n", errors[st]);
}