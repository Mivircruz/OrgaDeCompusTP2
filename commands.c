#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "cache.h"

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
    size_t counter = 0;
    char* result =  strchr(line, ARGS_SEP);
    for(; result[counter] != '\n'; counter++);
    result[counter] = '\0';
    return result;
}

void separate_arguments(char* line, unsigned int* arg1, unsigned int* arg2) {
    char* arguments = get_arguments(line);
    char* arg_string;
    char* aux;
    size_t counter;

    for(counter = 0; arguments[counter] != WRITE_SEP; counter++);
    arg_string = malloc(sizeof(char) * counter);

    for(counter = 0; arguments[counter] != WRITE_SEP; counter++) {
        arg_string[counter] = arguments[counter];
    }
    arg_string[counter] = '\0';
    *arg1 = strtol(arg_string, &aux, 10);
    free(arg_string);

    arg_string = strchr(arguments, WRITE_SEP);
    *arg2 = strtol(arg_string, &aux, 10);
}
/* ******************************************************************
 *                      COMMAND FUNCTIONS
 * *****************************************************************/

status_t execute_command(char* line) {
    char* command = get_command(line);
    char* aux;
    unsigned int arg1;
    unsigned int arg2;

    if(!strcmp(command, FLUSH_CMD)) {
        init();
        free(command);
        return OK;
    } else if (!strcmp(command, READ_CMD)) {
        arg1 = strtol(get_arguments(line), &aux, 10);  //to convert string to int
        //read_byte(arg1);
        free(command);
        return OK;
    } else if (!strcmp(command, WRITE_CMD)) {
        separate_arguments(line, &arg1, &arg2);
        //write_byte(arg1, arg2);
        free(command);
        return OK;
    } else if (!strcmp(command, MISS_RATE_CMD)) {
        printf("Miss Rate = %f\n", get_miss_rate());
        free(command);
        return OK;
    }
    free(command);
    return ERROR_INVALID_CMD;
}