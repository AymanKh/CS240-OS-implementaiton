//
//  kernel.h
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#ifndef kernel_h
#define kernel_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interrupts.h"
#include "hardware_interface.h"
#include "console_interface.h"
#include "process.h"
//#include "ObjectStore.h"
//#include "BitMap.h"




void kernel_start();
extern int jiffies;
//extern char *consoleInput[100];
//extern int nbytes;
//extern void ClockInterrupt(int input);

extern int CreatePersistentObject(char * keyname);
extern void * MapPersistentObject(char * keyname, int offset, int size);
extern int GetPersistentObjectSize(char * keyname);
extern int UnMapPersistentObject(void * address);
extern void logKeyNameHashTable();
extern void logBitMap();
extern int TruncatePersistentObject(char * keyname, int offset, int length);
extern int DeletePersistentObject(char * keyname);
extern void runProcess();

//extern keynameHash *hashTable;



#endif /* kernel_h */
