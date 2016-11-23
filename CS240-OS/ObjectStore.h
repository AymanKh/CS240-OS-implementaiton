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
//#include "BitMap.h"


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


// BitMap functions
extern int SearchForAvailableBit();
extern void SetBits(int position);

#endif /* ObjectStore_h */
