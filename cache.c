#include "cache.h"
#include <stdio.h>

#define MEMORY_SIZE 64 * 1024   //64KB
#define CACHE_SIZE 16 * 1024    //16KB
#define BLOCK_SIZE 64           //64B
#define CACHE_WAYS 8
#define CACHE_METADATA 3
#define CACHE_SET CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)

/* ******************************************************************
 *             STRUCT CACHE ADT DEFINITION
 * *****************************************************************/

//Main memory
//Size: 64KB
//Block Size: 16B
unsigned char MAIN_MEMORY[MEMORY_SIZE / BLOCK_SIZE][BLOCK_SIZE];

struct cache
{
    //Cache memory - 8-WSA
    //Size: 16KB
    //Block Size: 64B
    unsigned char cache_memory[CACHE_WAYS][CACHE_SET][BLOCK_SIZE];

    //Metadata
    //First byte: Valid
    //Second byte: Dirty
    //Other 6 bytes: Counter
    unsigned char cache_metadata[CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)][CACHE_METADATA * CACHE_WAYS];

    //Miss rate
    size_t MR;

} CACHE;


/* ******************************************************************
 *             CACHE FUNCTIONS
 * *****************************************************************/


//Reads the block 'blocknum' from memory and 
//stores it on the specified way and set on cache memory
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set)
{
    for(int offset = 0; offset < BLOCK_SIZE; offset++)
    {
        CACHE.cache_memory[way][set][offset] = MAIN_MEMORY[blocknum][offset];
    }
}


//Returns the content of the cache set to which the address maps
unsigned int find_set(unsigned int address)
{

}

//Writes the value on main memory on the address given 
//and on cache memory
//If the block is not on the cache, it only writes the value on main memory
void write_byte(unsigned int address, unsigned char value)
{

}