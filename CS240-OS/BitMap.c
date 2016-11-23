//
//  BitMap.c
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "BitMap.h"


void SetBits(int position)
{
    /* 1. set bit at 'position' to one */
    
    int arrayPos = position / 64;
    unsigned long long int bitPos = position % 64;
    
    unsigned long long int *sectorGroup = &BitMap[arrayPos];
    unsigned long long int value = BitMap[arrayPos];
    
    // set bitPos to '1'
    //*sectorGroup ^= ((unsigned long long int)-1 ^ *sectorGroup) & (1 << bitPos);
    
    unsigned long long int mask = ((unsigned long long int)1 << bitPos);
    *sectorGroup ^= mask;
}

void ClearBits(int position)
{
    /* set bit at 'positon' to zero */
    
    int arrayPos = position / 64;
    int bitPos = position % 64;
    
    unsigned long long int *sectorGroup = &BitMap[arrayPos];
    
    // set bitPos to '0'
    *sectorGroup ^= ((unsigned long long int)0 ^ *sectorGroup) & ((unsigned long long int)1 << bitPos);
    
}

// Assumption: we read bits starting from the LSB, but we allocate array starting at index 0

int GetBit(int position)
{
    /* get the value in 'positon'*/
    int arrayPos = position / 64;
    int bitPos = position % 64;
    
    unsigned long long int *sectorGroup = &BitMap[arrayPos];
    
    // This is a mask with the desired bit set to '1'
    unsigned long long int mask = 1 ;
    mask = (1 << bitPos);
    
    // AND sectorGroup w/ mask
    unsigned long long int result = *sectorGroup & mask;
    
    // return '1' if bit at position is '1', '0' otherwise
    return (result > 0);
    
}


int SearchForAvailableBit()
{
    /* find the position of the first '1' bit*/
    
    unsigned long long int sectorGroup = 77;
    unsigned long long t = 1;
    int r = 1;
    int exitFlag = 0;
    int i = 0;
    
    while (i < 512)
    {
        sectorGroup = ~BitMap[i];
        
        
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
    
    if (i == 512)
    {
        // disk is full
        return -1;
    }
    else
    {
        return (r-1)+((i)*64);
    }
    
}


// TODO: LATER
void initilizeBitMap(FILE* log)
{
    
}
