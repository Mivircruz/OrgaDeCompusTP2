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

    for(counter = 0; line[counter] != '\0' && line[counter] != ' '; counter++);
    char* result = malloc(sizeof(char) * (counter + 1));

    for(counter = 0; line[counter] != '\0' && line[counter] != ' '; counter++); {
        result[counter] = line[counter];
    }
    result[counter] = '\0';

    return result;
}


/* ******************************************************************
 *                      COMMAND FUNCTIONS
 * *****************************************************************/

status_t execute_command(char* line) {
    char* argument = get_command(line);

    if(!strcmp(argument, FLUSH_CMD)) {
        //init();
        free(argument);
        return OK;
    } else if (!strcmp(argument, READ_CMD)) {
        //read_byte(args[2]);
        free(argument);
        return OK;
    } else if (!strcmp(argument, WRITE_CMD)) {
        //write_byte(args[2], args[3]);
        free(argument);
        return OK;
    } else if (!strcmp(argument, MISS_RATE_CMD)) {
        //get_miss_rate();
        free(argument);
        return OK;
    }
    free(argument);
    return ERROR_INVALID_CMD;
}