#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "types.h"
#include "errors.h"

int main(int argc, char* argv[]) {
    printf("MAIN\n");
	char* linea = NULL;
	size_t capacidad = 0;
    status_t st;

	while(getline(&linea, &capacidad, stdin) != -1){
        printf("ENTRO\n");
        if((st = execute_command(argv)) != OK) {
            show_error(st);
            break;
        }
	}
	free(linea);
    return st;
}