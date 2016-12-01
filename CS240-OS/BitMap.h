//
//  BitMap.h
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef BitMap_h
#define BitMap_h

#include <stdio.h>
#include "process.h"

#define MAX_LLU 18446744073709551615LLU
#define Reserved_for_OS 256 // number of blocks in main memory reserved for kernel
#define NO_OF_BLOCKS 131072 // = 512MB/4096B


//#include "hardware_interface.h"

//typedef enum _storage {Memory, Disk} storage;
int Memory = 0;
int Disk = 1;

unsigned long long int BitMapDisk[2048] = {0};
unsigned long long int BitMapMemory[128] = {MAX_LLU, MAX_LLU, MAX_LLU, MAX_LLU}; // The first four entries are busy, they are mapped to kernel, rest is 0

void SetBits(int position,int dest); // set a bit to one
void ClearBits(int position,int dest); // clear a bit (or multiple) upon deletion of a persistent object
int GetBit(int position,int dest); // find if a certain position is one or zero
int SearchForAvailableBit(int dest);
int translateBitPositionToBlockNumberInDisk(int position);
void* translateBitPositionToPageNumberInMemory(int position);
void initilizeBitMap(char *logName); // assign values to bitMap from stored log upon powerup
void logBitMap();

extern int freeBlocks;
extern int usedBlocks;
extern PCB *currentPCB;


#endif /* BitMap_h */
