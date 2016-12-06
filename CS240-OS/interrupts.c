//
//  interrupts.c
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "interrupts.h"
#include "utlist.h"




int clockRunning = 0;
int jiffies = 0;
char consoleInput[100];
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
    
    char x[30];
    sprintf(x,"jiffies = %d \n",jiffies);
    
    halt();
}

// Console Interrupt

void ConsoleInterrupt(int input)
{
    
    char s[50];
    sprintf(s,"Interrupt: Console Interrupt! Characted read: %c\n",input);
    write_console((unsigned) strlen(s), s);
    
    consoleInput[noOfChars++] = input;
    
    if (noOfChars == nbytes)
    {
        write_console(20,"Input has been read\n");
        sprintf(s,"You Entered: %s and i = %d\n",consoleInput, noOfChars);
        write_console((unsigned) strlen(s),s);
        memset(consoleInput,0,100);
        noOfChars = 0;
    }
    else if (input == '\n')
    {
        write_console(20,"Input has been read\n");
        sprintf(s,"You Entered: %s and i = %d\n",consoleInput, noOfChars);
        write_console((unsigned) strlen(s),s);
        memset(consoleInput,0,100);
        noOfChars = 0;
    }
    
    halt();
    
    
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
    
    iret();
    
}

void ExceptionInterrupt(int input)
{
    char s[50];
    sprintf(s,"Interrupt: Exception Interrupt! Exc No = %d\n",input);
    write_console((unsigned) strlen(s), s);
    
    unsigned va;
    int x;
    
    switch (input) {
        case EXCEPTION_BAD_ADDRESS:
            /*
             1. use currentPCB to allocate a new stack page
             2. iret()
             */
            //            _AddOneStackPage();
            va = e_context.a[0];
            
            
                // The user process is trying to grow the stack
                _AddOneStackPage(va);
                
        
            
            
            
            //            {
            //                char a[100];
            //                sprintf(a,"Exception Interrupt: Excessive Stack allocation (more than 4MB !!)\n");
            //                write_console((unsigned) strlen(a), a);
            //
            //                // TODO: Destroy Process here, halt() for now
            //                halt();
            //            };
            break;
    }
    
    iret();
    
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
    unsigned va = machine_context.reg[12];
    unsigned vaint = machine_context.reg[13];
    
    char s[50];
    sprintf(s,"Interrupt: Trap Interrupt! Trap Number = %d \n",trapNo);
    write_console((unsigned) strlen(s), s);
    char t[50];
    
    
    switch(trapNo)
    {
        case TRAP_EXIT:
            Exit();
            break;
        case TRAP_WRITE_CONSOLE:
            //            WriteConsole((char *)machine_context.reg[12], (int)machine_context.reg[13]);
            WriteConsole((char *)_TranslateVirtualAddressToPhysicalAddress(machine_context.reg[12]), (int)machine_context.reg[13]);
            
            
            break;
    }
    
    iret();
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
    
    return 0;
}

//int _AddOneStackPage()
//{
//
//    char s[50];
//    sprintf(s,"Adding one stack page...\n");
//    write_console((unsigned) strlen(s), s);
//
//
//    //Actual Stack Page
//    int memBlockForActualStack = SearchForAvailableBit(Memory);
//    SetBits(memBlockForActualStack, Memory);
//    unsigned stackAddrActual = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForActualStack);
//
//    // 6. populate PTEs for the 2nd level pages with control bits
//    pte stackPTE;
//    stackPTE.pte_ = (int)stackAddrActual & (int)(0xfffff000);
//    stackPTE.pte_ |= (0x00000015); // read, write, valid, execute
//
//    unsigned rootPage = currentPCB->rootPagePhysAddress;
//
//    pte* mappedRootPage = (pte*) map_physical_page((void*)rootPage);
//
//    pte stackL2Page;
//    stackL2Page.pte_ = mappedRootPage[currentPCB->stackOffsetInRoot].pte_ & 0xfffff000;
//    pte *mappedL2StackPage = (pte*) map_physical_page((void*)stackL2Page.pte_);
//
//    int i = currentPCB->stackOffsetInL2 ;
//
//
//    // outer loop for the root entries
//    for (int j = currentPCB->stackOffsetInRoot; j > currentPCB->dataOffsetInRoot;j--)
//    {
//
//
//        // inner loop for interior pages
//        for ( ; i >= 0;i--)
//        {
//            pte newPTE = mappedL2StackPage[i];
//
//
//
//            if (newPTE.pte_ == 0)
//            {
//                mappedL2StackPage[i].pte_ = stackPTE.pte_;
//                // TODO: if i == 1024, then map a new page starting from the root page
//
//                char s[50];
//                sprintf(s,"Adding the %dth stack page...\n",i);
//                write_console((unsigned) strlen(s), s);
//
//
//                return 0;
//            }
//        }
//
//        // interior page is not usable, move on the the upper entry in root
//        if (mappedRootPage[j-1].pte_ == 0)
//        {
//            // Alocate new interior page table for the stack
//            int memBlockForStack = SearchForAvailableBit(Memory);
//            SetBits(memBlockForStack, Memory);
//            unsigned stackAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForStack);
//
//            pte rootStackPTE;
//            rootStackPTE.pte_ = stackAddr;
//            rootStackPTE.pte_ |= (0x00000015); // read, write, valid, execute
//            mappedRootPage[j-1].pte_ = rootStackPTE.pte_;
//
//            mappedL2StackPage = (pte*) map_physical_page((void*)stackAddr);
//            memset((void*)mappedL2StackPage, 0, 4096);
//
//        }
//        else
//        {
//            stackL2Page.pte_ = mappedRootPage[j-1].pte_ & 0xfffff000;
//            mappedL2StackPage = (pte*) map_physical_page((void*)stackL2Page.pte_);
//
//        }
//
//
//
//
//
//        i = 1023 ;
//
////        pte *stackMappedPage = (pte*)map_physical_page((void*) stackAddr);
////        memset(stackMappedPage, 0, 4096);
////        stackMappedPage[j] = stackPTE2;
//
//
//
//        
//        
//    }
//    
//    return -1;
//
//    
//}




