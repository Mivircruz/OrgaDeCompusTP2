#ifndef CACHE__H
#define CACHE__H

#include <stdio.h>

/* *****************************************************************
 *                   CACHE ADT DEFINITION
 * *****************************************************************/

typedef struct cache CACHE;


/* *****************************************************************
 *                   CACHE ADT FUNCTIONS
 * *****************************************************************/

//Initializes:
//Main memory to 0
//Cache lines as invalid
//Miss rate to 0
void init(void);

//Returns the byte's offset of the memory block to which the address maps
unsigned int get_offset (unsigned int address);

//Returns the number of the cache set to which the address maps
unsigned int find_set(unsigned int address);

//Returns the way containing the oldest block inside the set 'setnum'
unsigned int select_oldest(unsigned int setnum);

//Reads the block 'blocknum' from memory and 
//stores it on the specified set and offset on cache memory
void read_tocache(unsigned int blocknum, unsigned int set, unsigned int offset);

//Seeks the byte's value according to address's position on the cache memory
//and returns it
//If the byte is not on the cache, it brings that block from main memory
unsigned char read_byte(unsigned int address);

//Updates the blocknum in the 'set' and the in the 'offset' of main memory
void write_tomem(unsigned int blocknum, unsigned int set, unsigned int offset);

//Writes the value on main memory on the address given 
//and on cache memory
//If the block is not on the cache, it only writes the value on main memory
void write_byte(unsigned int address, unsigned char value);

//Returns miss rate
float get_miss_rate(void);

//Releases ADT cache
void release(void);

#endif