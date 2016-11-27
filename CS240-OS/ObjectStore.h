//
//  ObjectStore.h
//  CS240-OS
//
//  Created by A Y M A N on 11/21/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef ObjectStore_h
#define ObjectStore_h

#include <stdio.h>
#include "uthash.h"
#include "utlist.h"
#include "hardware_interface.h"
//#include "BitMap.h"

#define BLOCK_SIZE 8
//#include "BitMap.h"

#define NO_OF_BLOCKS 131072 // = 512MB/4096B

int CreatePersistentObject(char * keyname);
int DeletePersistentObject(char * keyname);
int GetPersistentObjectSize(char * keyname);
void * MapPersistentObject(char * keyname, int offset, int size);
int UnMapPersistentObject(void * address);
int TruncatePersistentObject(char * keyname, int offset, int length);
int FreePersistentStoreSpace();
int UsedPersistentStoreSpace();
int NumOfPersistentObjects();
char * GetPersistentObjectKey(int i);


typedef struct blockNode blockNode;

struct blockNode {
    int blockPosition;
    blockNode *next; /* needed for singly- or doubly-linked lists */
};

/* 
 This structure stores the addresses that need to be written to disk in pairs(disk address & memory address)
 will be used in a function that reads disk after the logic/calculations of the mapping is complete
*/
typedef struct addressesToReadNode addressToReadNode;

struct addressesToReadNode {
    int blockPhysicalAddr;
    void* memAddress;
    addressToReadNode *next; /* needed for singly- or doubly-linked lists */
};


typedef struct physicalAddresesInDiskNode physicalAddresesInDiskNode;

struct physicalAddresesInDiskNode {
    int physicalInDisk;
    physicalAddresesInDiskNode *next;
};

typedef struct physicalAddresesInMemoryNode physicalAddresesInMemoryNode;

struct physicalAddresesInMemoryNode {
    void * physicalInMemory;
    physicalAddresesInMemoryNode *next;
};


typedef struct _KeynameHash {
    void *keynameH;
    int size;
    int mappedFlag;
    blockNode *blocksHead;
    UT_hash_handle hh;         /* makes this structure hashable */
} keynameHash ;

keynameHash *hashTable = NULL;

int keyname_sorti(keynameHash *a, keynameHash *b);


// Map virtual address to physical ones in memory and in disk
typedef struct _addressesHash {
    char * keyname;
    void * keyVirtualAddr;
    physicalAddresesInDiskNode *physicalAddresesInDiskHead;
    physicalAddresesInMemoryNode *physicalAddresesInMemoryHead;
    int size;
    UT_hash_handle hh;         /* makes this structure hashable */
} addressHash;

addressHash *hashTableAddresses = NULL;



// Flags
extern int Memory;
extern int Disk;

// BitMap functions
//extern int SearchForAvailableBit();
//extern void SetBits(int position);

// Track Used and Free Space, to facilitate life
int usedBlocks = 0;
int freeBlocks = NO_OF_BLOCKS;



// This structure will be used when calling actual reads to the disk
typedef struct _cont {
    void (* func)();
    signed int tid;
    void *arg1;
    int arg2;
    UT_hash_handle hh;         /* makes this structure hashable */
} cont;

cont *hashTableTid = NULL;


void readDiskWrapper(addressToReadNode *head, int index);
void writeDiskWrapper(addressToReadNode *head, int index);


extern int translateBitPositionToBlockNumberInDisk(int position);
extern void* translateBitPositionToPageNumberInMemory(int position);
extern int SearchForAvailableBit(int dest);
extern void SetBits(int position,int dest); // set a bit to one
extern void ClearBits(int position,int dest); // clear a bit (or multiple) upon deletion of a persistent object



#endif /* ObjectStore_h */
