//
//  interrupts.h
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef interrupts_h
#define interrupts_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware_interface.h"
#include "console_interface.h"
#include "uthash.h"
//#include "HandleDisk.h"
//#include "BitMap.h"

struct Temp {
    char array[512];
};


void ClockInterrupt(int input);
void ConsoleInterrupt(int input);
void DiskInterrupt(int input);
void MachineCheckInterrupt(int input);

extern int nbytes;

typedef struct _cont {
    void (* func)();
    int tid;
    void *arg1;
    int arg2;
    UT_hash_handle hh;         /* makes this structure hashable */
} cont;
//

extern cont *hashTableTid;

//extern char consoleInput[100];

#endif /* interrupts_h */
