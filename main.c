#include <stdio.h>
#include "commands.h"
#include "types.h"
#include "errors.h"

int main(int argc, char* argv[]) {
    status_t st;
    if((st = execute_command(argv)) != OK) {
        show_error(st);
    }
    return st;
}