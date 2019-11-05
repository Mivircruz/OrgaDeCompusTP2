#ifndef CACHE__H
#define CACHE__H

//Initializes ADT cache:
//Principal memory with 0
//Cache lines as invalid
//Miss rate with 0
void init(void);

//Returns the block's offset of the mapped address
unsigned int get_offset (unsigned int address);

//Returns the content of the mappedd address
unsigned int find_set(unsigned int address);

//Returns the oldest block inside the set 'setnum'
unsigned int select_oldest(unsigned int setnum);

//
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set);
void write_tocache(unsigned int address, unsigned char);
unsigned char read_byte(unsigned int address);
void write_byte(unsigned int address, unsigned char value);

//Returns miss rate since cache was initialized
float get_miss_rate(void);

//Releases ADT cache
void release(void):

#endif