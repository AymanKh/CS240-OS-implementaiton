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
    hashit->blocksHead = NULL;
    hashit->mappedFlag = 0;
    
    
    /*
     1. find the value, from bitMap
     2. Connect them using the hash table
     
     TODO: Error Checking */
    
    
    // Assumption: each key MUST be mapped to at least one block
    
    blockNode *blockNodeToAdd = malloc(sizeof(blockNode));
    blockNodeToAdd->blockPosition = SearchForAvailableBit(Disk);
    LL_APPEND(hashit->blocksHead, blockNodeToAdd);
    
    SetBits(blockNodeToAdd->blockPosition,Disk);
    
    
    HASH_ADD_PTR(hashTable, keynameH, hashit);
    
    free(hashit);
    
    return 0;
}

int GetPersistentObjectSize(char * keyname)
{
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable,&keyname, getHash);
    
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
    
    blockNode *temp;
    
    LL_FOREACH(getHash->blocksHead,temp)
    {
        ClearBits(temp->blockPosition, Disk);
    }
    
    HASH_DEL(hashTable, getHash);
    
    return 0;
}

// Assumption: if offset exceeds current allocated disk blocks, new blocks will reflect the new size
void * MapPersistentObject(char * keyname, int offset, int size)
{
    
    /*
     1. initilize a structure that maps various addresses to one virtual addresses
     2. get the value of the given keyname, to get the list of locations on Disk
     3. calculate offset in blocks unit, iterate thorugh the linked list to get the starting block
     4. start reading from disk, mapping each disk block to a memory block
     5. if request blocks is more than the previously assigned data in disk, then start allocating new disk blocks
     5a. append new disk blocks to the keyname
     6. return the first assigned virtual address
     */
    
    
    // initilize getHash
    addressHash *getHashAddresses;
    
    getHashAddresses->physicalAddresesInDiskHead = NULL;
    getHashAddresses->physicalAddresesInMemoryHead = NULL;
    getHashAddresses->keyname = keyname;
    
    int requestedBlocksInMemory = size/PAGE_SIZE + 1;
    int blocksToSkip = offset/(BLOCK_SIZE * SECTOR_SIZE);
    
    int memLocation = 0;
    void *memAddress = NULL;
    int blockPhysicalAddr = 0;
    
    int tempSize = size;
    
    
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable,&keyname, getHash);
    
    blockNode *temp = getHash->blocksHead;
    
    int i = 0;
    
    // find the first block to read taking into consideration the offset
    while(i < blocksToSkip && temp != NULL)
    {
        temp = temp->next;
        i++;
    }
    
    int returnAddressFlag = 1;
    void * returnVirtualAddress = NULL;
    
    // Allocate pages in memory, read pages in disk, copy pages in disk into memory
    while(requestedBlocksInMemory > 0 && temp != NULL)
    {
        // get first avialable location in memory
        memLocation = SearchForAvailableBit(Memory);
        SetBits(memLocation,Memory);
        memAddress = translateBitPositionToPageNumberInMemory(memLocation);
        
        
        // to return the first virtual address, susequent virtual addresses will allocated sequentially
        if (returnAddressFlag)
        {
            returnVirtualAddress = map_physical_page(memAddress);
            returnAddressFlag = 0;
        }
        else
        {
            map_physical_page(memAddress);
        }
        
        
        blockPhysicalAddr = translateBitPositionToBlockNumberInDisk(temp->blockPosition);
        
        // Update hashtable for addresses
        physicalAddresesInDiskNode *toAddDisk = malloc(sizeof(physicalAddresesInDiskNode));
        toAddDisk->physicalInDisk = blockPhysicalAddr;
        LL_APPEND(getHashAddresses->physicalAddresesInDiskHead, toAddDisk);
        free(toAddDisk);
        
        physicalAddresesInMemoryNode *toAddMemory = malloc(sizeof(physicalAddresesInMemoryNode));
        toAddMemory->physicalInMemory = memAddress;
        LL_APPEND(getHashAddresses->physicalAddresesInMemoryHead, toAddMemory);
        free(toAddMemory);
        
        // read block physical address into memAddress
        read_disk(blockPhysicalAddr, BLOCK_SIZE, memAddress);
        
        requestedBlocksInMemory--;
        temp = temp->next;
        
        // update size
        tempSize -= PAGE_SIZE;
        
        
        
        // break and continue allocating new pages in Disk in the next loop
        if (temp == NULL)
        {
            break;
        }
    }
    
    if (tempSize > 0)
    {
        getHashAddresses->size += tempSize;
    }
    
    // This loop handles all cases when map request is more than what had been allocated
    while(requestedBlocksInMemory > 0)
    {
        // Get a new disk block, and append it to the keyname
        blockNode *blockNodeToAdd = malloc(sizeof(blockNode));
        blockNodeToAdd->blockPosition = SearchForAvailableBit(Disk);
        LL_APPEND(getHash->blocksHead, blockNodeToAdd);
        SetBits(blockNodeToAdd->blockPosition,Disk);
        
        // Get a new block in memory
        memLocation = SearchForAvailableBit(Memory);
        SetBits(memLocation,Memory);
        memAddress = translateBitPositionToPageNumberInMemory(memLocation);
        
        // Update hashtable for addresses
        physicalAddresesInDiskNode *toAddDisk = malloc(sizeof(physicalAddresesInDiskNode));
        toAddDisk->physicalInDisk = translateBitPositionToBlockNumberInDisk(blockNodeToAdd->blockPosition);
        LL_APPEND(getHashAddresses->physicalAddresesInDiskHead, toAddDisk);
        free(toAddDisk);
        
        physicalAddresesInMemoryNode *toAddMemory = malloc(sizeof(physicalAddresesInMemoryNode));
        toAddMemory->physicalInMemory = memAddress;
        LL_APPEND(getHashAddresses->physicalAddresesInMemoryHead, toAddMemory);
        free(toAddMemory);
        
        
        
        // Map disk to memory
        map_physical_page(memAddress);
        
        requestedBlocksInMemory--;
    }
    
    
    getHash->mappedFlag = 1;
    // Update remaining hash structures keys & values and add it to the hashtable
    
    getHashAddresses->keyVirtualAddr = returnVirtualAddress;
    HASH_ADD_PTR(hashTableAddresses, keyVirtualAddr, getHashAddresses);
    
    return returnVirtualAddress;
    
}

int UnMapPersistentObject(char * address)
{
    /*
     1. translate the given virtual addresses into a physical addresses, using the hashtable
     2. write back to the disk using the physical addresses (location in disk available in BitMap)
     3. Remove virtual address from hashtable
     4. clear bits in BitMapMemory
     5. in someway, free the virtual space --> NO NEED
     */
    
    addressHash *getHashAddress;
    HASH_FIND_PTR(hashTableAddresses,&address, getHashAddress);
    
    // find the keyname hash instance and set its 'mapped' flag to 0
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable, &(getHashAddress->keyname), getHash);
    getHash->mappedFlag = 0;
    
    int numOfSectors = (getHashAddress->size/SECTOR_SIZE) + 1;
    int numOfBlocks = (numOfSectors/BLOCK_SIZE) * 8;
    
    physicalAddresesInDiskNode *physicalDiskNode = getHashAddress->physicalAddresesInDiskHead;
    physicalAddresesInMemoryNode *physicalMemoryNode = getHashAddress->physicalAddresesInMemoryHead;
    
    while (physicalDiskNode)
    {
        
        write_disk((void *)(physicalDiskNode->physicalInDisk), numOfBlocks , (void *)physicalMemoryNode->physicalInMemory);
        
        ClearBits((int)physicalMemoryNode->physicalInMemory/PAGE_SIZE,Memory);
        
        physicalDiskNode = physicalDiskNode->next;
        physicalMemoryNode = physicalMemoryNode->next;
    }
    
    HASH_DEL(hashTableAddresses, getHashAddress);
    
    return 0;
}

int TruncatePersistentObject(char * keyname, int offset, int length)
{
    /*
     1. get block positions in disk using bitmap from the keyname as key to the hashtable
     2. check for error conditions
     3. calculate offset in blocks
     4. clear bits as long as the difference between the position of the length and
     the position of the offset is greater than zero
     5. Update the linked list of the block position appropriatley
     
     */
    
    // get object with the given keyname
    keynameHash *getHash;
    HASH_FIND_PTR(hashTable, &keyname, getHash);
    
    // return error if the object is not currently mapped, or offset/length are not valid
    if((!getHash->mappedFlag) || (offset >= getHash->size) || (offset + length) >= getHash->size )
    {
        return -1;
    }
    
    
    getHash->size -= length;
    
    if (length < PAGE_SIZE)
    {
        return 0;
    }
    
    int offsetInBlocks = offset/BLOCK_SIZE;
    int spaceInFirstBlock = (offsetInBlocks+1) * PAGE_SIZE - offset;
    int newLength = length - spaceInFirstBlock;
    
    blockNode *tempBlockNode = getHash->blocksHead;
    blockNode *storeBlockNode = tempBlockNode;
    blockNode *temp2BlockNode = tempBlockNode;
    
    // iterate through the list to get to the propahh starting block
    while(offsetInBlocks > 0)
    {
        tempBlockNode = tempBlockNode->next;
        offsetInBlocks--;
    }
    
    
    while (newLength > PAGE_SIZE)
    {
        // remove block by flipping the bit in bitmap to 0
        ClearBits(tempBlockNode->blockPosition,Disk);
        temp2BlockNode = tempBlockNode->next;
        LL_DELETE(getHash->blocksHead, tempBlockNode);
        free(tempBlockNode);
        tempBlockNode = temp2BlockNode;
        newLength -= PAGE_SIZE;
        
    }
    
    // update the linked list
    storeBlockNode->next = tempBlockNode;
    
    
    return 0;
}

int FreePersistentStoreSpace()
{
    
    return freeBlocks * SECTOR_SIZE * BLOCK_SIZE;
    //    return 0;
}

int UsedPersistentStoreSpace()
{
    return usedBlocks * SECTOR_SIZE * BLOCK_SIZE;
    //    return 0;
}

int NumOfPersistentObjects()
{
    return HASH_COUNT(hashTable);
}

int keyname_sorti(keynameHash *a, keynameHash *b)
{
    int result = strcmp(a->keynameH, b->keynameH);
    return (result > 0);
}

char * GetPersistentObjectKey(int i)
{
    if (i > NumOfPersistentObjects())
    {
        return NULL;
    }
    
    
    int j = 0;
    
    HASH_SORT( hashTable, keyname_sorti);
    keynameHash *s = hashTable;
    
    for(s = hashTable; j < i ; s=s->hh.next)
    {
        j++;
    }
    
    return s->keynameH;
    
}

void logKeyNameHashTable()
{
    FILE *fp = fopen( "HashTableKeyNameLog","w+");
    
    keynameHash *s = hashTable;
    blockNode *tempBlock = NULL;
    
    for(s = hashTable; s != NULL ; s=s->hh.next)
    {
        fprintf(fp,"%s\n", s->keynameH);
        fprintf(fp,"%d\n", s->size);
        
        tempBlock = s->blocksHead;
        
        while (tempBlock != NULL)
        {
            fprintf(fp,"%d ",tempBlock->blockPosition);
            tempBlock = tempBlock->next;
        }
        
        fprintf(fp,"-1\n");
    }
    
    fclose(fp);

}

void initilizeKeyNameHashTable(char *logName)
{
    FILE *fp = fopen(logName,"r");
    char keyname[256];
    int size = 0;
    int blockPosition = 0;
    
    
    
    while (!feof (fp))
    {
        blockNode *blocksHandler = NULL;
        fscanf(fp, "%s", keyname);
        fscanf(fp, "%d", &size);
        
        while (1)
        {
            blockNode *tempBlockNode = malloc(sizeof(blockNode));
            fscanf(fp,"%d",&blockPosition);
            
            if (blockPosition == -1)
            {
                break;
            }
            
            tempBlockNode->blockPosition = blockPosition;
            LL_APPEND(blocksHandler, tempBlockNode);
            //free(tempBlockNode);
        }
        
        keynameHash *tempToHash = malloc(sizeof(keynameHash));
        tempToHash->keynameH = keyname;
        tempToHash->mappedFlag = 0;
        tempToHash->size = size;
        tempToHash->blocksHead = blocksHandler;
        
        HASH_ADD_PTR(hashTable, keynameH, tempToHash);

        free(tempToHash);

    }
    
    fclose (fp);
    
}
/* (4) for (all phjysical disks)
 read_ disks --> you will get four transcations ids
 
 
 
 
 */

