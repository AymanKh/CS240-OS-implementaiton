//////
//////  ObjectStore.c
//////  CS240-OS
//////
//////  Created by A Y M A N on 11/21/16.
//////  Copyright © 2016 A Y M A N. All rights reserved.
//////

#include "ObjectStore.h"


int CreatePersistentObject(char *keyname)
{
    /* 
     1. hash the keyname
     2. the value of the key should be a number, this number represent the location in BitMap, which can then
     be translated to an actual block
     
     TODO: Error Checking: uniqueness of the key*/
    
    keynameHash *hashit = malloc(sizeof(keynameHash));
    hashit->keynameH = (void *) keyname;
    hashit->size = 0;
    
    
    /*
     1. find the value, from bitMap
     2. Connect them using the hash table
     
     TODO: if more than one block needed, then loop through this
     TODO: Error Checking
     TODO: use fseek to find size of file, and allocate the propahh number of sectors*/
    
    
    // Assumption: each key MUST be mapped to at least one block
    hashit->blockPosition = SearchForAvailableBit();
    SetBits(hashit->blockPosition);
    
    HASH_ADD_PTR(hashTable, keynameH, hashit);
    
//    write_console(30, "Done CPO\n");
    
    return 0;
}

int GetPersistentObjectSize(char * keyname)
{
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable,&keyname, getHash);
    
    char s[100];
    sprintf(s,"getHash->size == %d\n", getHash->size);
    write_console(20,s);
    
    return getHash->size;
}

int DeletePersistentObject(char * keyname)
{
    /*
     1. get struct using 'keyname' as key to the hash table
     2. Clear blockPosition in bitmap
     3. Remove struct from hash
     
     4. TODO: Unmap from main memory before deletion
     
     */
    
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable,&keyname, getHash);
    
    ClearBits(getHash->blockPosition);
    
    HASH_DEL(hashTable, getHash);
    
    return 0;
}

void * MapPersistentObject(char * keyname, int offset, int size)
{
    /*
     1. if size == 0, map virtual to physical
     2. else, read from disk and map disk content to virtual, using map_physical_page
     3. Create a new hashtable, the key is the virtual address, the value is physical address
     
     
     
     */
    
    return NULL;
    
}

int UnMapPersistentObject(char * address)
{
    /*
     1. translate the given virtual address into a physical address, using the hashtable
     2. write back to the disk using the physical address (location in disk available in BitMap)
     3. Remove virtual address from hashtable
     4. in someway, free the virtual space
     
     
     */
    return 0;
}



