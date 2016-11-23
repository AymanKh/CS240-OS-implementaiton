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
#include "hardware_interface.h"

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

typedef struct _KeynameHash {
    char *keynameH;
    int blockPosition;
    int size;
    UT_hash_handle hh;         /* makes this structure hashable */
} keynameHash ;

keynameHash *hashTable = NULL;

// Map virtual address to physical ones in memory and in disk
typedef struct _addressesHash {
    void * keyVirtualAddr;
    void * physicalInMemory;
    int physicalInDisk;
    int size;
    UT_hash_handle hh;         /* makes this structure hashable */
} addressHash ;

addressHash *hashTableAddresses = NULL;

// Flags
extern int Memory;
extern int Disk;

// BitMap functions
//extern int SearchForAvailableBit();
//extern void SetBits(int position);

#endif /* ObjectStore_h */
