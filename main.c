#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "types.h"
#include "errors.h"

int main(int argc, char* argv[]) {
	char* line = NULL;
	size_t capacity = 0;
    status_t st;

	while(getline(&line, &capacity, stdin) != -1){
        if((st = execute_command(line)) != OK) {
            show_error(st);
            break;
        }
	}
	free(line);
    return st;
}