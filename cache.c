#include "cache.h"
#include <stdio.h>

/* ******************************************************************
 *             STRUCT CACHE ADT DEFINITION
 * *****************************************************************/

struct cache
{
    //Main memory
    //Size: 64KB
    //Block Size: 16B
    unsigned char main_memory[MEMORY_SIZE / BLOCK_SIZE][BLOCK_SIZE];

    //Cache memory - 8-WSA
    //Size: 16KB
    //Block Size: 16B
    unsigned char cache_memory[CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)][BLOCK_SIZE * CACHE_WAYS];

    //Metadata
    //First byte: Valid
    //Second byte: Dirty
    //Other 6 bytes: Counter
    unsigned char cache_metadata[CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)][CACHE_METADATA * CACHE_WAYS];

    //Miss rate
    size_t MR;

};


/* ******************************************************************
 *             CACHE FUNCTIONS
 * *****************************************************************/
