#include "commands.h"
#include <stdio.h>
#include "string.h"
#include "types.h"

status_t execute_command(char* args[]) {
    if(!strcmp(args[1], FLUSH_CMD)) {
        //init();
        return OK;
    } else if (!strcmp(args[1], READ_CMD)) {
        //read_byte(args[2]);
        return OK;
    } else if (!strcmp(args[1], WRITE_CMD)) {
        //write_byte(args[2], args[3]);
        return OK;
    } else if (!strcmp(args[1], MISS_RATE_CMD)) {
        //get_miss_rate();
        return OK;
    }
    return ERROR_INVALID_CMD;
}