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
    
    char b[30];
    sprintf(b, "read from disk: %d\n", Disk);
    write_console((unsigned) strlen(b), b);

    /*
     1. find the value, from bitMap
     2. Connect them using the hash table
     
     TODO: if more than one block needed, then loop through this
     TODO: Error Checking
     TODO: use fseek to find size of file, and allocate the propahh number of sectors*/
    
    
    // Assumption: each key MUST be mapped to at least one block
    
    hashit->blockPosition = SearchForAvailableBit(Disk);
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


// TODO: the elephent in the room, implement a linked list to deal w/ offset, and size
// calling map_physical_page more than once
void * MapPersistentObject(char * keyname, int offset, int size)
{
    /*
     1. if size == 0, map virtual to physical
     2. else, read from disk and map disk content to virtual, using map_physical_page
     3. Create a new hashtable, the key is the virtual address, the value is physical address

     */
    
    if (GetPersistentObjectSize(keyname) == 0)
    {
        /*
         1. find the first available bit in memory
         2. translate the position of the bit, to an actual physical address
         3. Hash as described below
         4. This address will be mapped using map_physical_page
         */
        // TODO: get the first free bit in main memory
        int memLocation = SearchForAvailableBit(Memory);
        SetBits(memLocation,Memory);
        void *addr = translateBitPositionToPageNumberInMemory(memLocation);
        
        keynameHash *getHash;
        HASH_FIND_PTR(hashTable,&keyname, getHash);
        int blockPhysicalAddr = translateBitPositionToBlockNumberInDisk(getHash->blockPosition);

        
        addressHash *hashit = malloc(sizeof(addressHash));
        hashit->keyVirtualAddr = map_physical_page(addr);
        hashit->physicalInMemory = addr;
        hashit->physicalInDisk = blockPhysicalAddr;
        hashit->size = size;

        
        HASH_ADD_PTR(hashTableAddresses, keyVirtualAddr, hashit);

        
        
        return map_physical_page(addr);
        
    }
    
    /*
     1. use key to get block position from the hashtable
     2. transfer the block position to an actual physical address
     3. find first available bit in memory, translate into actual physical address
     4. read_disk using block physical address from step 2. and store data in physical address from step 3.
     5. fill up the hasthtable using virtual as key, and physicall addresses as values
     6. map the physical address from step 3. to a page in memory using map_physical_page
     */
    
    // 1.
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable,&keyname, getHash);
    
    // 2. 3.
    int blockPhysicalAddr = translateBitPositionToBlockNumberInDisk(getHash->blockPosition);
    int memLocation = SearchForAvailableBit(Memory);
    SetBits(memLocation,Memory);
    void *addr = translateBitPositionToPageNumberInMemory(memLocation);
    
    // 4.
    read_disk(blockPhysicalAddr, BLOCK_SIZE, addr);
    
    // 5.
    addressHash *hashit = malloc(sizeof(addressHash));
    hashit->keyVirtualAddr = map_physical_page(addr);
    hashit->physicalInMemory = addr;
    hashit->physicalInDisk = blockPhysicalAddr;
    hashit->size = size;
    
    HASH_ADD_PTR(hashTableAddresses, keyVirtualAddr, hashit);

    
    // 6.
    return map_physical_page(addr);
    
}

int UnMapPersistentObject(char * address)
{
    /*
     1. translate the given virtual address into a physical address, using the hashtable
     2. write back to the disk using the physical address (location in disk available in BitMap)
     3. Remove virtual address from hashtable
     4. clear bits in BitMapMemory
     5. in someway, free the virtual space
     */
    
    addressHash *getHash;
    HASH_FIND_PTR(hashTable,&address, getHash);
    
    int numOfSectors = (getHash->size/SECTOR_SIZE) + 1;
    int numOfBlocks = (numOfBlocks/BLOCK_SIZE) * 8;
    
    write_disk((void *)(getHash->physicalInDisk/8), numOfBlocks , (void *)getHash->physicalInMemory);

    
    
    return 0;
}



