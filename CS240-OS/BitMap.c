//
//  BitMap.c
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "BitMap.h"



void SetBits(int position,int dest)
{
    /* 1. set bit at 'position' to one */
    unsigned long long int *sectorGroup;
    
    int arrayPos = position / 64;
    unsigned long long int bitPos = position % 64;
    
    if (dest == Memory)
    {
        sectorGroup = &BitMapMemory[arrayPos];
    }
    else if (dest == Disk)
    {
        sectorGroup = &BitMapDisk[arrayPos];
        usedBlocks++;
        freeBlocks--;
    }
    
    // set bitPos to '1'    
    unsigned long long int mask = ((unsigned long long int)1 << bitPos);
    *sectorGroup ^= mask;
}

void ClearBits(int position,int dest)
{
    /* set bit at 'positon' to zero */
    
    unsigned long long int *sectorGroup;
    
    int arrayPos = position / 64;
    unsigned long long int bitPos = position % 64;
    
    if (dest == Memory)
    {
        sectorGroup = &BitMapMemory[arrayPos];
    }
    else if (dest == Disk)
    {
        sectorGroup = &BitMapDisk[arrayPos];
        usedBlocks--;
        freeBlocks++;
    }
    
    // set bitPos to '0'
    *sectorGroup ^= ((unsigned long long int)0 ^ *sectorGroup) & ((unsigned long long int)1 << bitPos);
    
}

// Assumption: we read bits starting from the LSB, but we allocate array starting at index 0

int GetBit(int position,int dest)
{
    /* get the value in 'positon'*/
    unsigned long long int *sectorGroup;
    
    int arrayPos = position / 64;
    unsigned long long int bitPos = position % 64;
    
    if (dest == Memory)
    {
        sectorGroup = &BitMapMemory[arrayPos];
    }
    else if (dest == Disk)
    {
        sectorGroup = &BitMapDisk[arrayPos];
    }
    
    // This is a mask with the desired bit set to '1'
    unsigned long long int mask = 1 ;
    mask = (1 << bitPos);
    
    // AND sectorGroup w/ mask
    unsigned long long int result = *sectorGroup & mask;
    
    // return '1' if bit at position is '1', '0' otherwise
    return (result > 0);
    
}


int SearchForAvailableBit(int dest)
{
    int limit;
    
    if (dest == Memory)
    {
        limit = 128;
    }
    else if (dest == Disk)
    {
        limit = 2048;
    }
    else
    {
        return -1;
    }
    
    /* find the position of the first '1' bit*/
    
    unsigned long long int sectorGroup = 77;
    unsigned long long t = 1;
    int r = 1;
    int exitFlag = 0;
    int i = 0;
    
    while (i < limit)
    {
        sectorGroup = ~BitMapDisk[i];
        
        
        t = 1;
        r = 1;
        
        // From Wikipedia
        exitFlag = 1;
        while ((sectorGroup & t) == 0)
        {
            t = t << 1;
            r++; // r is the first '0' in the BitMap
            
            if (r == 65)
            {
                exitFlag = 0;
                i++;
                break;
            }
        }
        
        // break out of loop if we found the position of a '1' NOW, so that we don't increment i and return a wrong value
        if(exitFlag)
        {
            break;
        }
        
    }
    
    if (i == limit)
    {
        // disk is full
        return -1;
    }
    else
    {
        return (r-1)+((i)*64);
    }
    
}

int translateBitPositionToBlockNumberInDisk(int position)
{
    // Multiply by 8 since each blocks spans 8 sectors
    return position * 8;
}


void* translateBitPositionToPageNumberInMemory(int position)
{
    // since each bit position corresopnds to a 4KB page in memory
    void *addr = (void *) ((long int)position * 4096);
    return addr;
}

void logBitMap()
{
    FILE *fp = fopen( "DiskBitMapLog","w+");

    
    for (int i = 0; i < 2048; i++)
    {
        fprintf(fp,"%llu\n", BitMapDisk[i]);
    }
    
    fclose(fp);
    
}

void initilizeBitMap(char *logName)
{
    FILE *fp = fopen(logName,"r");
    
    int i = 0;
    while (!feof (fp))
    {
        
        fscanf(fp, "%llu", &BitMapDisk[i]);
        i++;
    }
    
    fclose (fp);
    
//    char b[60];
//    sprintf(b,"value = %llu\n", BitMapDisk[5]);
//    write_console(20,b);
    
}

