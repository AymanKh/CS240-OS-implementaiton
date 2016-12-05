//
//  process.c
//  CS240-OS
//
//  Created by A Y M A N on 11/28/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "process.h"

static PCB	pcbs[PROCESS_MAX_PROCS];
PCB		*currentPCB;

static Queue	freepcbs;
static Queue	runQueue;
static Queue	waitQueue;

void runProcess()
{
    /*
     1. get physical addresses in memory for code and stack from header, by adding their size to the header
     2. Allocate two pages in memory for each
     3. get the two virtual addresses for the code and the stack
     4. allocate one page for the root table (store physical address for later)
     5. get the offset of the page table using the 2st 10 bits of VAs from step 3.
     6. fill these offsets by the PTEs two allocated PAs from step 2. and set the status bits appropriatley
     7. use 1nd 10 bits from VA to find the offset in the first level pages
     8. fill the offest in the root with the first 10 bits of the PA from step 2.
     9. set control bits
     10. set_ptbr to the physical address of the root page from step 4.
     11. setup machine_context
     12. iret()
     */
    
//    int x = test;
    
    char a[60];
    sprintf(a,"in runProcess\n");
    write_console((unsigned)strlen(a),"in runProcess\n");

    
    // TODO: hard coded for now, replace (1<<20) with a page in memory
    
    
    
    
    unsigned hardCoded = (unsigned)(1<<20);
    header * vaddr = (header *)map_physical_page((void * )hardCoded);
    
    
    
    // 2. read the header file
    unsigned code = vaddr->code;
    unsigned code_segment_size = vaddr->code_segment_size;
    unsigned code_segment_start = vaddr->code_segment_start;
    unsigned data_segment_size = vaddr->data_segment_size;
    unsigned data_segment_start = vaddr->data_segment_start;
    unsigned stack_segment_start = vaddr->stack_segment_start;
    
    // Allocate pages that copies 'code' and 'data' segments content
    int memBlockForCodeContent = SearchForAvailableBit(Memory);
    SetBits(memBlockForCodeContent, Memory);
    unsigned codeAddrContent = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForCodeContent);
    void *mappedCodeContent = map_physical_page((void*)codeAddrContent);
    memcpy(mappedCodeContent,(void*)((void*)vaddr+sizeof(header)),code_segment_size);
    
    
    int memBlockForDataContent = SearchForAvailableBit(Memory);
    SetBits(memBlockForDataContent, Memory);
    unsigned dataAddrContent = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForDataContent);
    void *mappedDataContent = map_physical_page((void*)dataAddrContent);
    memcpy(mappedDataContent,(void*)((void*)vaddr+sizeof(header)+code_segment_size),data_segment_size);
    
    // 3. Allocate page table nodes for code, data and stack
    int memBlockForCode = SearchForAvailableBit(Memory);
    SetBits(memBlockForCode, Memory);
    unsigned codeAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForCode);
    
    int memBlockForData = SearchForAvailableBit(Memory);
    SetBits(memBlockForData, Memory);
    unsigned dataAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForData);
    
    // 2nd level page for the stack
    int memBlockForStack = SearchForAvailableBit(Memory);
    SetBits(memBlockForStack, Memory);
    unsigned stackAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForStack);
    
    //Actual Stack Page
    int memBlockForActualStack = SearchForAvailableBit(Memory);
    SetBits(memBlockForActualStack, Memory);
    unsigned stackAddrActual = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForActualStack);
    
    // 4. Allocate memory for the root page
    int memBlockForRootPage = SearchForAvailableBit(Memory);
    SetBits(memBlockForRootPage, Memory);
    unsigned rootPageAddr = (unsigned) translateBitPositionToPageNumberInMemory(memBlockForRootPage);
    
    // 5. decipher the virtual addresses
    v_address vAddrCodeSegment;
    vAddrCodeSegment.composite.root_level = (code_segment_start >> 22);   // = 0b00000000001111111111000000000000
    vAddrCodeSegment.composite.interior_level = (code_segment_start  & 0x003ff000);
    vAddrCodeSegment.composite.interior_level = (vAddrCodeSegment.composite.interior_level >> 12);
    vAddrCodeSegment.composite.offset = (code_segment_start  & 0x00000fff);
    
    v_address vAddrDataSegment;
    vAddrDataSegment.composite.root_level = (data_segment_start >> 22);   // = 0b00000000001111111111000000000000
    vAddrDataSegment.composite.interior_level = (data_segment_start  & 0x003ff000 );
    vAddrDataSegment.composite.interior_level = (vAddrDataSegment.composite.interior_level >> 12);
    vAddrDataSegment.composite.offset = (data_segment_start  & 0b00000000000000000000111111111111);
    
    v_address vAddrStackSegment;
    vAddrStackSegment.composite.root_level = (stack_segment_start >> 22);   // = 0b00000000001111111111000000000000
    vAddrStackSegment.composite.interior_level = (stack_segment_start  & 0x003ff000 );
    vAddrStackSegment.composite.interior_level = (vAddrStackSegment.composite.interior_level >> 12);
    vAddrStackSegment.composite.offset = (stack_segment_start  & 0b00000000000000000000111111111111);
    
    
    // 6. populate PTEs for the 2nd level pages with control bits
    pte codePTE;
    codePTE.pte_ = (int)codeAddrContent & (int)(0xfffff000);
    codePTE.pte_ |= (0x00000015); // read, write, execute, valid
    
    
    pte dataPTE;
    dataPTE.pte_ = (int)dataAddrContent & (int)(0xfffff000);
    dataPTE.pte_ |= (0x00000015); // read, valid, *write
    
    pte stackPTE;
    stackPTE.pte_ = (int)stackAddrActual & (int)(0xfffff000);
    stackPTE.pte_ |= (0x00000013); // read, write, valid

    
    
    // Map the 2nd level pages for code, stack, and segments
    pte *codeMappedPage = (pte*)map_physical_page((void*) codeAddr);
    codeMappedPage[vAddrCodeSegment.composite.interior_level].pte_ = codePTE.pte_;
    
    pte *dataMappedPage = (pte*)map_physical_page((void*) dataAddr);
    dataMappedPage[vAddrDataSegment.composite.interior_level].pte_ = dataPTE.pte_;
    
    pte *stackMappedPage = (pte*)map_physical_page((void*) stackAddr);
    stackMappedPage[vAddrStackSegment.composite.interior_level] = stackPTE;
    
    // 7. and 8. write PTEs to the first root level
    pte *rootMappedPage = (pte*)map_physical_page((void*) rootPageAddr);
    pte rootCodePTE;
    rootCodePTE.pte_ = (int)codeAddr & (int)(0xfffff000);
    rootCodePTE.pte_ |= (0x00000011); // read, valid
    
    pte rootDataPTE;
    rootDataPTE.pte_ = (int)dataAddr & (int)(0xfffff000);
    rootDataPTE.pte_ |= (0x00000011); // read, valid
    
//    pte rootStackPTE;
//    rootStackPTE.pte_ = (int)stackAddr & (int)(0xfffff000);
//    rootStackPTE.pte_ |= (0x00000011);// read, valid
    
    // fill up the root page with the PTEs
    rootMappedPage[vAddrCodeSegment.composite.root_level].pte_ = rootCodePTE.pte_;
    rootMappedPage[vAddrDataSegment.composite.root_level].pte_ = rootDataPTE.pte_;
//    rootMappedPage[vAddrStackSegment.composite.root_level] = rootStackPTE;
    
    // set_ptbr and machine context
    set_ptbr((void*)rootPageAddr);
    machine_context.pc = code_segment_start;
    machine_context.sp = stack_segment_start;
    
    
    //TODO: move to somwhere more elegent (i.e process schedlur)
    PCB *runningPCB = malloc(sizeof(*runningPCB));
    runningPCB->rootPagePhysAddress = rootPageAddr;
    currentPCB = runningPCB;
    
//    return 0;

    
}


void ProcessModuleInit()
{
    //    QueueInit(&freepcbs);
    //    QueueInit(&runQueue);
    //    QueueInit(&waitQueue);
    
    //    for (int i = 0; i < PROCESS_MAX_PROCS; i++) {
    ////        pcbs[i].flags = PROCESS_STATUS_FREE;
    //        QueueLinkInit(&(pcbs[i].l), (void *)&pcbs[i]);
    //        QueueInsertFirst(&freepcbs, &(pcbs[i].l));
    //    }
    //    currentPCB = NULL;
}

//int CreateProcess(char * executable)
//{
//    
//    
//    return 0;
//}


void Exit()
{
    char b[60];
    sprintf(b,"a process called Exit()!\n");
    write_console((unsigned) strlen(b),b);
    
    halt();
    
}

