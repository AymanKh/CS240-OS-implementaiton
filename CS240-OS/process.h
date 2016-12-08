//
//  process.h
//  CS240-OS
//
//  Created by A Y M A N on 11/28/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef process_h
#define process_h

#include <stdio.h>
#include "Queue.h"
#include "hardware_interface.h"
#include "defines.h"
#include "utlist.h"


#define MEM_L1TABLE_SIZE 1024
#define PROCESS_MAX_PROCS 10


void ProcessModuleInit();
int CreateProcess(char * executable);
void Exit();
int DestoryProcess(int pid);
int FreezeProcess(int pid, int ticks);
int ResumeProcess(int pid);
int GetMyPid();

void runProcess(void * addr);

typedef struct PCB {
//    uint32	*currentSavedFrame; // -> current saved frame.  MUST BE 1ST!
//    uint32	*sysStackPtr;	// Current system stack pointer.  MUST BE 2ND!
//    uint32	sysStackArea;	// System stack area for this process
    /* uint32 * userStackPtr;	// user stack pointer */
//    unsigned int	flags;
    unsigned codeOffsetInRoot;
    unsigned stackOffsetInRoot;
    unsigned dataOffsetInRoot;
    unsigned stackOffsetInL2;
    unsigned codeOffsetInL2;
    unsigned dataOffsetInL2;
    unsigned stack_segment_start;
    unsigned code_segment_start;
    
    char		name[80];	// Process name
    int	L1_pagetable[MEM_L1TABLE_SIZE]; // Statically allocated page table
    int npages;
    unsigned ptbr;
    unsigned rootPagePhysAddress;
    
    Link		*l;		// Used for keeping PCB in queues
} PCB;

extern PCB	*currentPCB;
extern int Memory;
extern int Disk;
//extern int test77;
//extern int test;


extern int SearchForAvailableBit(int dest);
extern void SetBits(int position,int dest); // set a bit to one
extern void* translateBitPositionToPageNumberInMemory(int position);

//extern keynameHash *hashTable;



#endif /* process_h */
