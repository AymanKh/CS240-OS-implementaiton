//
//  interrupts.c
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "interrupts.h"
#include "utlist.h"


cont *hashTablePid = NULL;

int clockRunning = 0;
int jiffies = 0;
char consoleInput[256];
int noOfChars = 0;



// Clock Interrupt

void ClockInterrupt(int input)
{
    
    if(!clockRunning)
    {
        clockRunning = 1;
    }
    else
    {
        jiffies++;
    }
    
    char x[100];
    sprintf(x,"jiffies = %d \n",jiffies);
    
    unsigned testsp = machine_context.sp;
    unsigned testpc = machine_context.pc;
    sprintf(x,"testsp = %x, testpc = %x\n",testsp,testpc);
    //    write_console((unsigned)strlen(x), x);
    
    if (processSchedule() == OS_OK)
    {
        iret();
    }
    else
    {
        halt();
    }
    
}

// Console Interrupt

void ConsoleInterrupt(int input)
{
    
    char s[50];
    sprintf(s,"Interrupt: Console Interrupt! Characted read: %c\n",input);
    write_console((unsigned) strlen(s), s);
    
    consoleInput[noOfChars++] = input;
    
//    memset(consoleInput,0,100);
//    noOfChars = 0;


    
    if (noOfChars == nbytes)
    {
//        write_console(20,"Input has been read\n");
//        sprintf(s,"You Entered: %s and i = %d\n",consoleInput, noOfChars);
//        write_console((unsigned) strlen(s),s);
//        memset(consoleInput,0,100);
        noOfChars = 0;
    }
    else if (input == '\n')
    {
        
        write_console(20,"Input has been read\n");
        sprintf(s,"You Entered: %s and i = %d\n",consoleInput, noOfChars);
        write_console((unsigned) strlen(s),s);
        activateBlockedProcess();
        
        cont *tempCont;
        PCB *headQueue = _getHeadofReadyQueue();
        HASH_FIND_INT(hashTablePid, &(headQueue->PID), tempCont);
        
        (tempCont->func)(tempCont->arg1, tempCont->arg2);
        noOfChars = 0;
        
        

    }
    
    if (currentPCB != NULL)
    {
        iret();
    }
    else
    {
        halt();
    }
    
    
}

void DiskInterrupt(int tid)
{
    
    char s[50];
    sprintf(s,"Interrupt: Disk Interrupt! t_id = %d \n",tid);
    write_console((unsigned) strlen(s), s);
    
    
    if (HASH_COUNT(hashTableTid) == 0)
    {
        halt();
    }
    
    //    cont *test = malloc(sizeof(cont));
    cont *test;
    HASH_FIND_INT(hashTableTid, &tid, test);
    
    if (test->func != &halt)
    {
        if (test->arg1 == NULL)
        {
            (test->func);
        }
        else if (test->arg2 == NULL)
        {
            (test->func)(test->arg1);
        }
        else
        {
            (test->func)(test->arg1, test->arg2);
            
        }
        
    }
    else
    {
        (test->func)();
    }
    
    //    iret();
    
    
}

void ExceptionInterrupt(int input)
{
    char s[50];
    sprintf(s,"Interrupt: Exception Interrupt! Exc No = %d\n",input);
    write_console((unsigned) strlen(s), s);
    
    unsigned va;
    int x;
    char z[200];
    
    
    switch (input) {
        case EXCEPTION_BAD_ADDRESS:
            
            va = e_context.a[0];
            
            unsigned diff = (unsigned)currentPCB->stack_segment_start - va;
            
            // The user process is trying to grow the stack
            if ((unsigned) diff < 4096)
            {
                _AddOneStackPage(va);
                currentPCB->stack_segment_start -= 4096;
            }
            else
            {
                sprintf(z,"The process is accessing a bad address, terminating...\n");
                write_console((unsigned) strlen(z), z);
                DestoryCurrentPCB();
            }
            
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            
            sprintf(z,"Illiegal Exception Interrupt,  e0 = %d, e1 = %d, a[0] = %d, a\n",e_context.e0,e_context.e1,e_context.a[0]);
            write_console((unsigned) strlen(z), z);
            
    }
    
    if (currentPCB != NULL)
    {
        iret();
    }
    else
    {
        halt();
    }
}

void MachineCheckInterrupt(int input)
{
    char s[50];
    sprintf(s,"Interrupt: Machine Check Interrupt!\n");
    write_console((unsigned) strlen(s), s);
    shutdown_machine();
}

void TrapInterrupt(int input)
{
    
    int trapNo = machine_context.reg[11];
    unsigned arg1 = machine_context.reg[12];
    unsigned arg2 = machine_context.reg[13];
    unsigned arg3 = machine_context.reg[14];
    unsigned arg4 = machine_context.reg[15];
    
    char s[300];
    sprintf(s,"Interrupt: Trap Interrupt! Trap Number = %d \n",trapNo);
    write_console((unsigned) strlen(s), s);
    char t[50];
    
    
    switch(trapNo)
    {
        case TRAP_EXIT:
            //TODO: destroy the calling process
            Exit();
            
            // get the tail of the ready queue and remove it
            void *testAPCB = currentPCB;
            DestoryCurrentPCB();
            
            
            halt();
            break;
            
        case TRAP_WRITE_CONSOLE:
            
            sprintf(s,"TRAP_WRITE_CONSOLE in progress...\n");
            write_console((unsigned) strlen(s), s);
            unsigned va = machine_context.reg[12];
            
            int outsideDataSegment = (va > (currentPCB->data_segment_start+currentPCB->data_segment_size) || va < (currentPCB->data_segment_start));
            
            int outsideDynamicSegment = (va > (currentPCB->dynamic_mem_start + currentPCB->dynamic_mem_size) || va < (currentPCB->dynamic_mem_start));
            
            if (outsideDataSegment)
            {
                if(outsideDynamicSegment)
                {
                    sprintf(s,"This is not the process's terrirory, destroying process...\n");
                    write_console((unsigned) strlen(s), s);
                    DestoryCurrentPCB();
                }
                else
                {
                    WriteConsole((char *)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]), (int)machine_context.reg[13]);
                }
            }
            else
            {
                WriteConsole((char *)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]), (int)machine_context.reg[13]);
            }
            
            /*
             1. check if current process is in hashTablePid
             
             */
            
            
            
            break;
            
        case TRAP_CREATE_PERSISTENT_OBJECT:
            
            sprintf(s,"TRAP_CREATE_PERSISTENT_OBJECT in progress...\n");
            write_console((unsigned) strlen(s), s);
            machine_context.reg[12] = CreatePersistentObject((char*)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]));
            
            if (machine_context.reg[12] == OS_OK)
            {
                machine_context.reg[11] = 0;
                logKeyNameHashTable();
                logBitMap();
            }
            else
            {
                machine_context.reg[11] = -1;
            }
            break;
            
        case TRAP_DELETE_PERSISTENT_OBJECT:
            
            sprintf(s,"TRAP_DELETE_PERSISTENT_OBJECT in progress...\n");
            write_console((unsigned) strlen(s), s);
            
            machine_context.reg[12] = DeletePersistentObject((char*)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]));
            if (machine_context.reg[12] == OS_OK)
            {
                machine_context.reg[11] = 0;
                logKeyNameHashTable();
                logBitMap();
            }
            else
            {
                machine_context.reg[11] = -1;
            }
            break;
            
        case TRAP_GET_PERSISTENT_OBJECT_SIZE:
            
            sprintf(s,"TRAP_GET_PERSISTENT_OBJECT_SIZE in progress...\n");
            write_console((unsigned) strlen(s), s);
            
            machine_context.reg[11] = GetPersistentObjectSize((char*)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]));
            if (machine_context.reg[11] >= OS_OK)
            {
                machine_context.reg[12] = OS_OK;
                logKeyNameHashTable();
                logBitMap();
            }
            else
            {
                machine_context.reg[12] = OS_KN;
            }
            break;
            
        case TRAP_CREATE_MEMORY_OBJECT:
            
            sprintf(s,"TRAP_CREATE_MEMORY_OBJECT in progress...\n");
            write_console((unsigned) strlen(s), s);
            //void * returnToUser = CreateMemoryObject((char*)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]));
            void * returnToUser = _TranslateVirtualAddressToPhysicalAddressDynamic();
            
            if (returnToUser != NULL)
            {
                machine_context.reg[11] = returnToUser;
                
            }
            else
            {
                machine_context.reg[11] = NULL;
            }
            
            break;
            
            
        case TRAP_SHUTDOWN:
            shutdown_machine();
            break;
            
        case TRAP_READ_CONSOLE:
            
            sprintf(s,"TRAP_READ_CONSOLE in progress...\n");
            write_console((unsigned) strlen(s), s);
            
            /*
             1. create the cont. structure, its function pointer should the function readConsole. 
             2. block the currently running PCB
             3. in console interrupt, put the data in the buffer -> DONE
             4. call the continutation function
             5. run the process
             */
            
            int nbytesTrap = machine_context.reg[13];
            cont *readPCBcont = malloc(sizeof(*readPCBcont));
            readPCBcont->func = &ReadConsole;
            readPCBcont->arg1 = (void *)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]);
            readPCBcont->arg2 = nbytesTrap;
            readPCBcont->tid = currentPCB->PID;
            
            HASH_ADD_INT(hashTablePid, tid, readPCBcont);
            
            addCurrentPCBtoBlockedQueue();
        

            break;
            
        case TRAP_CREATE_PROCESS:
            sprintf(s,"TRAP_CREATE_PROCESS in progress...\n");
            write_console((unsigned) strlen(s), s);
            
            unsigned vaCP = machine_context.reg[12];
            int ret = CreateProcess((char*)_TranslateVirtualAddressToPhysicalAddress(vaCP));
            if (ret == OS_OE)
            {
                sprintf(s,"You are having way too much fun (processes), and you are being cavalier about it, ABORTING!!\n");
                write_console((unsigned) strlen(s), s);
                shutdown_machine();
            }
            
            break;
            
            
            
            
    }
    
    if (currentPCB != NULL)
    {
        iret();
    }
    else
    {
        halt();
    }
    
}


unsigned _TranslateVirtualAddressToPhysicalAddress(unsigned va)
{
    /*
     1. need the following:
     - the virtual address 'given'
     - the root page table of the currently running process
     */
    
    unsigned activeRootPagePhysAddre = currentPCB->rootPagePhysAddress;
    
    v_address virtualAddress;
    virtualAddress.composite.root_level = (va >> 22);   // = 0b00000000001111111111000000000000
    virtualAddress.composite.interior_level = (va  & 0x003ff000);
    virtualAddress.composite.interior_level = (virtualAddress.composite.interior_level >> 12);
    virtualAddress.composite.offset = (va & 0x00000fff);
    
    
    pte *mappedRootPage = (pte*)map_physical_page(activeRootPagePhysAddre);
    pte *pteFirstLevel = malloc(sizeof(*pteFirstLevel));
    memcpy((void*)pteFirstLevel, (void*)((void*)mappedRootPage+virtualAddress.composite.root_level*sizeof(pte)), sizeof(pte));
    pteFirstLevel->pte_ &= 0xfffff000;
    
    
    pte *mappedL1Page = (pte*)map_physical_page(pteFirstLevel->pte_);
    pte *pteSecondLevel = malloc(sizeof(*pteSecondLevel));
    memcpy((void*)pteSecondLevel, (void*)((void*)mappedL1Page+virtualAddress.composite.interior_level*sizeof(pte)), sizeof(pte));
    pteSecondLevel->pte_ &= 0xfffff000;
    
    return (pteSecondLevel->pte_ + virtualAddress.composite.offset);
    
}

int _AddOneStackPage(unsigned va)
{
    unsigned activeRootPagePhysAddre = currentPCB->rootPagePhysAddress;
    unsigned x;
    v_address virtualAddress;
    virtualAddress.composite.root_level = (va >> 22);   // = 0b00000000001111111111000000000000
    virtualAddress.composite.interior_level = (va  & 0x003ff000) >> 12;
    virtualAddress.composite.offset = (va & 0x00000fff);
    
    pte *mappedRootPage = (pte*)map_physical_page((void*)activeRootPagePhysAddre);
    pte newPTE;
    newPTE.pte_= mappedRootPage[virtualAddress.composite.root_level].pte_;
    
    // check it current PTE is empty, if it is, then allocate new stack pages
    if (newPTE.pte_ == 0)
    {
        
        char s[50];
        sprintf(s,"Allocating a new stack page...\n");
        write_console((unsigned) strlen(s), s);
        
        //Alocate new interior page table for the stack and map it for later use
        int memBlockForStack = SearchForAvailableBit(Memory);
        SetBits(memBlockForStack, Memory);
        unsigned stackAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForStack);
        pte* mappedInteriorStackPage = (pte*) map_physical_page((void*)stackAddr);
        
        // put its physical address in the root page
        pte rootStackPTE;
        rootStackPTE.pte_ = stackAddr & (int)(0xfffff000);
        rootStackPTE.pte_ |= (0x00000013); // read, write, valid, execute
        mappedRootPage[virtualAddress.composite.root_level].pte_ = rootStackPTE.pte_;
        
        // Allocate new actual stack page
        int memBlockForActualStack = SearchForAvailableBit(Memory);
        SetBits(memBlockForActualStack, Memory);
        unsigned stackAddrActual = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForActualStack);
        
        // prepare the interior level PTE
        pte interiorStackPTE;
        interiorStackPTE.pte_ = stackAddrActual & (int)(0xfffff000);
        interiorStackPTE.pte_ |= (0x00000013);
        
        // put the PTE in interior stack page
        mappedInteriorStackPage[virtualAddress.composite.interior_level].pte_ = interiorStackPTE.pte_;
        
    }
    else
    {
        // Allocate new actual stack page
        int memBlockForActualStack = SearchForAvailableBit(Memory);
        SetBits(memBlockForActualStack, Memory);
        unsigned stackAddrActual = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForActualStack);
        
        // prepare PTE to be put in the interior level
        pte interiorStackPTE;
        interiorStackPTE.pte_ = stackAddrActual & (int)(0xfffff000);
        interiorStackPTE.pte_ |= (0x00000013);
        
        // map the current interior page
        unsigned cuurentMappedStack = mappedRootPage[virtualAddress.composite.root_level].pte_ & 0xfffff000;
        pte* mappedInteriorStackPage = map_physical_page((void*)cuurentMappedStack);
        
        mappedInteriorStackPage[virtualAddress.composite.interior_level] = interiorStackPTE;
        
        
    }
    
    return 0;
}

void * _TranslateVirtualAddressToPhysicalAddressDynamic()
{
    /*
     1. find the first available entry in the root page
     2. allocate a new interior page (search -> set)
     3. link them
     4. allocate an actual 'DynamicObject' page
     5. put its address in the first avaialable entry in interior level
     */
    
    unsigned activeRootPagePhysAddre = currentPCB->rootPagePhysAddress;
    pte *mappedRootPage = (pte*)map_physical_page(activeRootPagePhysAddre);
    pte *pteFirstLevel = malloc(sizeof(*pteFirstLevel));
    
    pte tempPTE;
    
    
    
    int i = 270;
    
    
    // find the first avaialble root entry
    while (i < 1024)
    {
        tempPTE.pte_ = mappedRootPage[i].pte_;
        
        if (i == currentPCB->codeOffsetInRoot || i == currentPCB->dataOffsetInRoot || i == currentPCB->stackOffsetInRoot)
        {
            i++;
            continue;
        }
        
        if (tempPTE.pte_ == 0)
        {
            break;
        }
        
    }
    
    // allocate a new interior level page
    int memBlockForInteriorPage = SearchForAvailableBit(Memory);
    SetBits(memBlockForInteriorPage, Memory);
    unsigned memBlockForInteriorPageActual = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForInteriorPage);
    pte * mappedInterior = (pte*) map_physical_page(memBlockForInteriorPageActual);
    
    
    // put PTE in root
    pte tobePutInRoot;
    tobePutInRoot.pte_ = memBlockForInteriorPageActual;
    tobePutInRoot.pte_ |= 0x00000013;
    mappedRootPage[i].pte_ = tobePutInRoot.pte_;
    
    // allocate an actual object page
    int memBlockForObject = SearchForAvailableBit(Memory);
    SetBits(memBlockForObject, Memory);
    unsigned memBlockForObjectAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForObject);
    
    pte tobeputinInterior;
    tobeputinInterior.pte_ = memBlockForObjectAddr;
    tobeputinInterior.pte_ |= 0x00000013;
    
    
    mappedInterior[0].pte_ = tobeputinInterior.pte_;
    
    v_address returnVA;
    returnVA.composite.root_level = (i << 22);
    returnVA.composite.interior_level = 0;
    returnVA.composite.offset = 0;
    
    
    
    //return returnVA.address;
    currentPCB->dynamic_mem_start = (i << 22);
    currentPCB->dynamic_mem_size = 4096;
    
    return (void *)(i << 22) ;
    
}


//            unsigned va = machine_context.reg[12];
//            int diff = (currentPCB->data_segment_start+currentPCB->data_segment_size) - va;
//            int diff2 = va - (currentPCB->data_segment_start+currentPCB->data_segment_size);
//
//            if (va > (currentPCB->data_segment_start+currentPCB->data_segment_size) || va < (currentPCB->data_segment_start))
//            {
//                sprintf(s,"This is not the process's terrirory, destroying process...\n");
//                write_console((unsigned) strlen(s), s);
//                DestoryCurrentPCB();
//
//            }
//            else
//            {
//                WriteConsole((char *)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]), (int)machine_context.reg[13]);
//            }

