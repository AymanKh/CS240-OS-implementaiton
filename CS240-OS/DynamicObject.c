//
//  DynamicObject.c
//  CS240-OS
//
//  Created by A Y M A N on 12/8/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "DynamicObject.h"

void * CreateMemoryObject(int size)
{
    /*
     1. search for available bit in memory, set it
     2. search for available space in the page table
     3. walk the table and put the physical address from step1
     4. assemble the VA using the page table
     */
    
    int memLocation = 0;
    unsigned memAddress;
    
    // get first avialable location in memory
    memLocation = SearchForAvailableBit(Memory);
    SetBits(memLocation,Memory);
    memAddress = translateBitPositionToPageNumberInMemory(memLocation);
    
    mappedPages *thePage = malloc(sizeof(*thePage));
    thePage->addr = (void*) memAddress;
    
    LL_APPEND(head, thePage);

    return _MapContinousPagesDynamic(head, 0);
}


void * _MapContinousPagesDynamic(mappedPages *head, int offset)
{
    /*
     Task: the list 'mappedPages' contain physical addresses that need to be inserted into
     the interioir level of the currently running process
     
     1.
     */
    
    v_address returnVirtual;
    returnVirtual.address = 0;
    int noOfBlocks;
    mappedPages *temp;
    LL_COUNT(head, temp, noOfBlocks);
    //    void *rootPage = (void*)currentPCB->ptbr;
    char s[50];
    sprintf(s,"first map is bad...\n");
    write_console((unsigned) strlen(s), s);
    pte *mappedRootPage = (pte*)map_physical_page((void*)currentPCB->rootPagePhysAddress);
    
    pte *tempPTE = malloc(sizeof(*tempPTE));
    
    memcpy((void*)tempPTE,(void*) mappedRootPage, sizeof(pte));
    //    tempPTE = mappedRootPage[2];
    
    int size = 0;
    
    
    int i = 0;
    
    while (i < 1024)
    {
        memcpy((void*)tempPTE, (void*)mappedRootPage+size, sizeof(pte));
        
        if (i == currentPCB->codeOffsetInRoot || i == currentPCB->dataOffsetInRoot || i == currentPCB->stackOffsetInRoot)
        {
            size += sizeof(pte);
            i++;
            continue;
        }
        
        if(tempPTE->pte_ == 0)
        {
            /*
             1. allocate a new page
             2. fill up a PTE to be put in root
             3. put given phys. addresses in the first blocks in interior level
             
             TODO: what if we need to map more than 1024 pages ?
             */
            
            int blockInMemoryBit = SearchForAvailableBit(Memory);
            SetBits(blockInMemoryBit, Memory);
            void * blockInMemoryPhysicalAddress = translateBitPositionToPageNumberInMemory(blockInMemoryBit);
            
            pte newPTE;
            newPTE.pte_ = ( (unsigned) blockInMemoryPhysicalAddress << 12);
            newPTE.pte_ |= 0x00000013;
            
            mappedRootPage[i].pte_ = newPTE.pte_;
            
            pte * newInteriorLevel = (pte*) map_physical_page(blockInMemoryPhysicalAddress);
            
            for (int j = 0; j < noOfBlocks; j++)
            {
                pte actualMappedObjectPTE;
                actualMappedObjectPTE.pte_ = ((unsigned)head->addr << 12);
                actualMappedObjectPTE.pte_ |= 0x00000013;
                
                newInteriorLevel[j].pte_ = actualMappedObjectPTE.pte_;
                
                head = head->next;
            }
            
            
            returnVirtual.address = (i << 22);
            returnVirtual.address |= offset;
            break;
            
        }
        else
        {
            /*
             1. map the address in PTE, get the virtual address back of the interior level
             2. loop and count, stop when you find contigous blocks
             */
            
            unsigned physicalAddressInteriorPage = tempPTE->pte_ & 0xfffff000;
            pte* mappedInteriorPage = (pte*)map_physical_page((void*)physicalAddressInteriorPage);
            
            pte tempPTE;
            
            int count = 0;
            int j = 0;
            
            for (j = 0; j < 1024 && count < noOfBlocks;j++)
            {
                if (mappedInteriorPage[j].pte_ == 0)
                {
                    count++;
                }
                else
                {
                    count = 0;
                }
                
            }
            
            if (j == 1024)
            {
                size += sizeof(pte);
                i++;
                continue;
            }
            
            
            
            int interiorOffsetFound = j - count - 1;
            
            j = 0;
            for(j = 0; j < count; j++)
            {
                pte actualMappedObjectPTE;
                actualMappedObjectPTE.pte_ = ((unsigned)head->addr << 12);
                actualMappedObjectPTE.pte_ |= 0x00000013;
                
                mappedInteriorPage[interiorOffsetFound+j].pte_ = actualMappedObjectPTE.pte_;
                
                head = head->next;
                
            }
            
            returnVirtual.address = (i << 22);
            returnVirtual.address |= (interiorOffsetFound << 12);
            returnVirtual.address |= offset;
            break;
            
        }
        
        
    }
    
    
    
    return (void*)returnVirtual.address;
    
}
