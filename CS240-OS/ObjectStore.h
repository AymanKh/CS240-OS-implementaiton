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
int UnMapPersistentObject(char * address);
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
    char *keynameH;
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



#endif /* ObjectStore_h */
