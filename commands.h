#ifndef COMMANDS__H
#define COMMANDS__H

#include "types.h"

#define FLUSH_CMD "FLUSH"
#define READ_CMD "R"
#define WRITE_CMD "W"
#define WRITE_SEP ','
#define MISS_RATE_CMD "MR"

//Executes the command given
//If everything's fine, returns OK
//Otherwise, returns ERROR
status_t execute_command(char* line);

#endif