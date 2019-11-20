#include "cache.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define MEMORY_SIZE 64 * 1024   //64KB
#define CACHE_SIZE 16 * 1024    //16KB
#define BLOCK_SIZE 64           //64B
#define CACHE_WAYS 8
#define CACHE_METADATA 16
#define CACHE_SET CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)

#define SIZE_SET_BITS 5
#define SIZE_OFFSET_BITS 6
#define SIZE_TAG_BITS 5

#define MASK_TAG 0x3E00
#define MASK_VALID 0x8000
#define MASK_OFFSET 0x003F
#define MASK_COUNTER 0x01FF
#define MASK_DIRTY 0x4000

/* ******************************************************************
 *             STRUCT CACHE ADT DEFINITION
 * *****************************************************************/

//Main memory
//Size: 64KB
//Block Size: 16B
unsigned char MAIN_MEMORY[MEMORY_SIZE / BLOCK_SIZE][BLOCK_SIZE];

struct cache {
    //Cache memory - 8-WSA
    //Size: 16KB
    //Block Size: 64B
    unsigned char cache_memory[CACHE_WAYS][CACHE_SET][BLOCK_SIZE];

    //Metadata
    //First bit: Valid
    //Second bit: Dirty
    //3,4,5,6,7 bits: Tag
    //Rest : counter
    uint16_t cache_metadata[CACHE_SIZE / (BLOCK_SIZE * CACHE_WAYS)][CACHE_WAYS];

    //Miss rate
    float access;
    float misses;
    

}cache;

cache_t CACHE; 


/* *****************************************************************
 *                    AUXILIAR FUNCTIONS
 * *****************************************************************/

// Write in main memory the value in the address
void write_in_main_memory(unsigned int address, unsigned char value)
{
    unsigned int set = address / BLOCK_SIZE;
    unsigned int offset = address % BLOCK_SIZE;
    MAIN_MEMORY[set][offset] = value;
}

//Ads 1 to all way's set counter except new_way
void add_1_to_counter(unsigned int set, unsigned int new_way)
{
    uint16_t metadata;

    for(size_t way = 0; way < CACHE_WAYS; way++) {
        if(way != new_way) {
            metadata = CACHE.cache_metadata[set][way];
            metadata++;
        }
    }
}

void _write_in_cache(unsigned int way, unsigned int set, unsigned int offset, unsigned char value) {
    CACHE.cache_memory[way][set][offset] = value;
    CACHE.cache_metadata[way][set] |= MASK_VALID; // set byte valid with all ones
    add_1_to_counter(set, way);
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
    memset(CACHE.cache_metadata, 0, sizeof(CACHE.cache_metadata)); //Valid, Dirty, Counter and Tag and counter initialized to 0
}


//Returns the byte's offset of the memory block to which the address maps
unsigned int get_offset (unsigned int address){
    unsigned int mask = pow(2,SIZE_OFFSET_BITS) - 1;  //loads 1 in 6 bits less significatives
    unsigned int offset = address & mask;
    return offset;
}

//Returns the number of the cache set to which the address maps
unsigned int find_set(unsigned int address)
{
    unsigned int mask = pow(2,SIZE_SET_BITS) - 1;               //load ones in five bits less significatives
    unsigned int adress_desplazed = address >> SIZE_SET_BITS;   //right shift
    return (adress_desplazed & mask);                           //return only five bits less significatives
}

//Returns the tag of the memory block to which the address maps
unsigned int get_tag(unsigned int address)
{
    unsigned int mask = pow(2,SIZE_TAG_BITS) - 1;  
    unsigned int tag = (address >> (SIZE_SET_BITS + SIZE_OFFSET_BITS)) & mask;
    return tag;
}

//Returns the way containing the oldest block inside the set 'setnum'
unsigned int select_oldest(unsigned int setnum)
{
    unsigned int oldest_block = 0;
    unsigned int oldest_way = 0;
    int16_t metadata, counter_metadata;

    for (int way = 0; way < CACHE_WAYS; way++)
    {
        metadata = CACHE.cache_metadata[way][setnum]; //we check de metadata of a specific block 
        counter_metadata = (metadata & MASK_COUNTER);
        if (counter_metadata > oldest_block) 
        {
            oldest_block = counter_metadata;
            oldest_way = way;
        }

    }
    return oldest_way;    
}

// Write in cache the value only if the address is found in some block of cache.
void write_in_cache(unsigned int address, unsigned char value)
{
    // values are from the adress 
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    unsigned int tag = get_tag(address);
    uint16_t metadata;
    unsigned int tag_in_cache;

    for (int way = 0; way < CACHE_WAYS; way++)
    {
        metadata = CACHE.cache_metadata[way][set];
        tag_in_cache = (metadata & MASK_TAG) >> 9; //the 9 bits less significatives are droped (counter ones).
        if (tag_in_cache == tag) // the address match with the adress in cache metadata. The address i
        {
            if(!(metadata & MASK_VALID)) {
                continue;
            }
            if (metadata & MASK_DIRTY) // checks if we need to write that block in memory (write back politics)
            {
                write_tomem(CACHE.cache_memory[way][set][offset], address / BLOCK_SIZE, address % BLOCK_SIZE, value);
                CACHE.cache_metadata[way][set] &= ~MASK_DIRTY; // we set the dirty byte in 0
            }
            _write_in_cache(way, set, offset, value);
            return;
        }

    }
    //If the block is not on the cache, is a miss
    CACHE.misses++;

    //Seeks for an empty block
    for (int way = 0; way < CACHE_WAYS; way++) {
         metadata = CACHE.cache_metadata[way][set];
        if (!(metadata & MASK_VALID)) {
            _write_in_cache(way, set, offset, value);
            return;
        }
    }
    //If there isn't an empty block, replaces an empty one
    unsigned int oldest_one = select_oldest(set);
     _write_in_cache(oldest_one, set, offset, value);
}


//Reads the block 'blocknum' from memory and 
//stores it on the specified way and set on cache memory
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set)
{
    for (int offset = 0; offset < BLOCK_SIZE; offset++)
    {
        CACHE.cache_memory[way][set][offset] = MAIN_MEMORY[blocknum][offset];
    }
    
    CACHE.cache_metadata[way][set] |= MASK_VALID; //if its load the block in cache, its need to refresh metadata
}

unsigned char read_byte(unsigned int address)
{
    unsigned int set = find_set(address);
    unsigned int offset = get_offset(address);
    unsigned int tag = get_tag(address);
    unsigned char read_value = CACHE.cache_memory[tag][set][offset];

    uint16_t metadata;
    unsigned int tag_in_cache;

    CACHE.access++;

    for (int way = 0; way < CACHE_WAYS; way++)
    {
        metadata = CACHE.cache_metadata[way][set];
        tag_in_cache = (metadata & MASK_TAG) >> 9; // //the 9 bits less significatives are droped (counter ones).
        
        if (tag_in_cache == tag) // the address match with the adress in cache metadata. The address i
        {
            if (metadata & MASK_VALID) // if we don't have the data in cache, we bring it from main memory
            {
                read_tocache(address/BLOCK_SIZE, way, set);
                CACHE.misses++;
            }

            read_value = CACHE.cache_memory[way][set][offset];

            return read_value;
        }
    }
    return read_value;
}

//Writes the value on main memory on the address given 
//and on cache memory
//If the block is not on the cache, it brings it back from memory and then, writes it
void write_byte(unsigned int address, unsigned char value)
{
    CACHE.access++;

    //writing in main memory.
    write_in_main_memory(address, value);

    //writin in cache memory.
    write_in_cache(address, value);
}

//Updates the blocknum in the 'set' and the in the 'offset' of main memory
void write_tomem(unsigned int blocknum, unsigned int set, unsigned int offset, unsigned char value)
{
    MAIN_MEMORY[set][offset] = blocknum;
}

//Returns miss rate
float get_miss_rate(void) {
    float miss_rate = CACHE.misses/CACHE.access;
    return miss_rate;
}

//Releases ADT cache
void release(void){
    //do nothing
}