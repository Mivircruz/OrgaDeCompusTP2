#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

/* ******************************************************************
 *                      AUXILIAR FUNCTIONS
 * *****************************************************************/

//Returns the command 
char* get_command(char* line){
    size_t counter;

    for(counter = 0; line[counter] != '\n' && line[counter] != ' '; counter++);
    char* result = malloc(sizeof(char) * (counter + 1));

    for(counter = 0; line[counter] != '\n' && line[counter] != ' '; counter++) {
        result[counter] = line[counter];
    }
    result[counter] = '\0';

    return result;
}

char* get_arguments(char* line) {
    return strchr(line, ARGS_SEP);
}
/* ******************************************************************
 *                      COMMAND FUNCTIONS
 * *****************************************************************/

status_t execute_command(char* line) {
    char* command = get_command(line);
    char* arguments;

    if(!strcmp(command, FLUSH_CMD)) {
        init();
        free(command);
        return OK;
    } else if (!strcmp(command, READ_CMD)) {
        //read_byte();
        free(command);
        return OK;
    } else if (!strcmp(command, WRITE_CMD)) {
        //write_byte(args[2], args[3]);
        free(command);
        return OK;
    } else if (!strcmp(command, MISS_RATE_CMD)) {
        get_miss_rate();
        free(command);
        return OK;
    }
    free(argument);
    return ERROR_INVALID_CMD;
}