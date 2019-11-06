#include "cache.h"
#include <stdio.h>

/* ******************************************************************
 *             STRUCT CACHE ADT DEFINITION
 * *****************************************************************/

struct cache
{
    //Principal Memory
    unsigned char[65536];

    //Cache memory - blocks
    unsigned char[128][16384]

    size_t MR;

};


/* ******************************************************************
 *             CACHE FUNCTIONS
 * *****************************************************************/
