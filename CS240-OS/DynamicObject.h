//
//  DynamicObject.h
//  CS240-OS
//
//  Created by A Y M A N on 12/8/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef DynamicObject_h
#define DynamicObject_h

#include <stdio.h>
#include "hardware_interface.h"
#include "utlist.h"
#include "process.h"


#endif /* DynamicObject_h */

typedef struct mappedPages mappedPages;

struct mappedPages {
    void *addr;
    mappedPages *next; /* needed for singly- or doubly-linked lists */
};

mappedPages *head = NULL;

void * CreateMemoryObject(int size);
void * _MapContinousPagesDynamic(mappedPages *head, int offset);


extern int translateBitPositionToBlockNumberInDisk(int position);
extern void* translateBitPositionToPageNumberInMemory(int position);
extern int SearchForAvailableBit(int dest);
extern void SetBits(int position,int dest); // set a bit to one
extern void ClearBits(int position,int dest); // clear a bit (or multiple) upon deletion of a persistent object

extern PCB *currentPCB;


extern int Memory;
extern int Disk;

