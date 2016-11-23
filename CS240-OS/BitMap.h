//
//  BitMap.h
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef BitMap_h
#define BitMap_h

#include <stdio.h>

//#include "hardware_interface.h"


unsigned long long int BitMap[512] = {0};
void SetBits(int position); // set a bit to one

void ClearBits(); // clear a bit (or multiple) upon deletion of a persistent object
int GetBit(int position); // find if a certain position is one or zero

int SearchForAvailableBit();

void initilizeBitMap(FILE* log); // assign values to bitMap from stored log upon powerup



#endif /* BitMap_h */
