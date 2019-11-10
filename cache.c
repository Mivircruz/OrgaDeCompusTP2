#include "cache.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MEMORY_SIZE 64 * 1024   //64KB
#define CACHE_SIZE 16 * 1024    //16KB
#define BLOCK_SIZE 64           //64B
#define CACHE_WAYS 8
#define CACHE_METADATA 3
#define CACHE_SET CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)

#define SIZE_SET_BITS 5
#define SIZE_OFFSET_BITS 6
#define SIZE_TAG_BITS 5

#define BINARY_MASK 32768 //mask = [1000 0000 0000 0000]

#define MASK_TAG 0x0000FF0000000000
#define MASK_VALID 0xFF00000000000000
#define MASK_OFFSET 0x003F

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
    //Third byte: Tag
    //Rest : counter
    unsigned char cache_metadata[CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)][CACHE_METADATA * CACHE_WAYS];

    //Miss rate
    float MR;

} cache_t;

cache_t CACHE; 


/* ******************************************************************
 *                    AUXILIAR FUNCTIONS
 * *****************************************************************/

unsigned int binary(unsigned int num) {
    unsigned int binary[64];
    unsigned int mask = BINARY_MASK;

    while(mask > 0) {
   if((num & mask) == 0 )
        strncat(binary, "0", 1);
   else
        strncat(binary, "1", 1);
    mask = mask >> 1 ;  // Right Shift
   }
   return binary;
}

/* ******************************************************************
 *                      CACHE FUNCTIONS
 * *****************************************************************/

//Initializes:
//Main memory to 0
//Cache lines as invalid
//Miss rate to 0
void init(void){
    memset(MAIN_MEMORY, 0, sizeof(MAIN_MEMORY)); //Main memory initialized to 0
    memset(CACHE.cache_metadata, 0, sizeof(CACHE.cache_metadata)); //Dirty, Tag and counter initialized to 0
    for(size_t i = 0; i < CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS); i++) {
        for(size_t j = 0; j < CACHE_METADATA * CACHE_WAYS; j += CACHE_METADATA)
            CACHE.cache_metadata[i][j] = 1; //Valid initialized to 1
    }
}

//Returns the byte's offset of the memory block to which the address maps
unsigned int get_offset (unsigned int address){
    unsigned int binary = binary(address);
    return binary & MASK_OFFSET;
}

//Reads the block 'blocknum' from memory and 
//stores it on the specified way and set on cache memory
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set)
{
    for(int offset = 0; offset < BLOCK_SIZE; offset++)
    {
        CACHE.cache_memory[way][set][offset] = MAIN_MEMORY[blocknum][offset];
    }
}


//Returns the number of the cache set to which the address maps
unsigned int find_set(unsigned int address)
{
    unsigned int mask = pow(2,SIZE_SET_BITS) - 1;               //load ones in five bits less significatives
    unsigned int adress_desplazed = address >> SIZE_SET_BITS;   //right shift
    return (adress_desplazed & mask);                           //return only five bits less significatives
}

//Writes the value on main memory on the address given 
//and on cache memory
//If the block is not on the cache, it only writes the value on main memory
void write_byte(unsigned int address, unsigned char value)
{
    //writing in main memory.
    write_in_main_memory(address, value);

    //writin in cache memory.
    write_in_cache(address, value);
}

// Write in main memory the value in the address
void write_in_main_memory(unsigned int address, unsigned char value)
{
    unsigned int set = address / BLOCK_SIZE;
    unsigned int offset = address % BLOCK_SIZE;
    MAIN_MEMORY[set][offset] = value;
}

// Write in cache the value only if the adress is found in some block of cache.
void write_in_cache(unsigned int address, unsigned char value)
{
    // values are from the adress 
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    unsigned int tag = get_tag(address);

    // TODO - ver validez
    for(int way = 0; way < CACHE_WAYS; way++)
    {
        int64_t metadata = CACHE.cache_metadata[way][set];
        unsigned int tag_in_cache = (metadata & MASK_TAG) >> 32; // the four bytes less significatives are droped.
        if (tag_in_cache == tag) // the address match with the adress in cache metadata. The address i
        {
            CACHE.cache_memory[way][set][offset] = value;
            CACHE.cache_metadata[way][set] = CACHE.cache_metadata[way][set] & MASK_VALID; // set byte valid with all ones
            return;
        }

    }
}

//Returns the tag of the memory block to which the address maps
unsigned int get_tag(unsigned int address)
{
    unsigned int mask = pow(2,SIZE_TAG_BITS) - 1;  
    unsigned int tag = (address >> (SIZE_SET_BITS + SIZE_OFFSET_BITS)) & mask;
    return tag;
}

//Returns miss rate
float get_miss_rate(void) {
    return CACHE.MR;
}

//Releases ADT cache
void release(void){
    //do nothing
}