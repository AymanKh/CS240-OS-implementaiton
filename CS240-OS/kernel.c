//
//  kernel.c
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#include "kernel.h"
#include <assert.h>


void kernel_start()
{
//    set_debug_mode(DEBUG_ALL);
    reset_debug_mode();
    start_console();
    initialize_memory(); // Keep track of available pages in main memory
    start_disk();
        
    char a[80];
    sprintf(a, "Hello World %d \n",jiffies);
    write_console((unsigned) strlen(a), a);
    
    // get the address of the clock interrupt
    void (*clockInterruptPointer)(int) = &ClockInterrupt;
    void (*consoleInterruptPointer)(int) = &ConsoleInterrupt;
    void (*diskInterruptPointer)(int) = &DiskInterrupt;
    void (*machineCheckInterruptPointer)(int) = &MachineCheckInterrupt;
    void (*TrapInterruptPointer)(int) = &TrapInterrupt;
    void (*ExceptionInterruptPointer)(int) = &ExceptionInterrupt;

    
    
    // set the vector table
    set_ivec(I_CLK, clockInterruptPointer);
    set_ivec(I_CNSL, consoleInterruptPointer);
    set_ivec(I_DSK, diskInterruptPointer);
    set_ivec(I_CHECK, machineCheckInterruptPointer);
    set_ivec(I_TRAP, TrapInterruptPointer);
    set_ivec(I_EXCEPT, ExceptionInterruptPointer);
    
    // read logs
    initilizeBitMap("DiskBitMapLog");
    initilizeKeyNameHashTable("HashTableKeyNameLog");
    
    // Process initlization module
//    ProcessModuleInit();
    
    
    
//    CreateProcess("file0.b");
    CreateProcess("shutdown.b");

//    CreateProcess("compute.b");

    
//    CreateProcess("file0.b");

    
    
//    int x = HASH_COUNT(hashTable);
    
    // Test running hello.b
    

    
    
    // Testing POS
    
//    int c1 = CreatePersistentObject("Monkey Name1");
//    int c2 = CreatePersistentObject("Monkey Name2");
//    int c3 = CreatePersistentObject("Monkey Name3");
//    int c4 = CreatePersistentObject("Monkey Name4");
//    int c5 = CreatePersistentObject("Monkey Name5");
//    
//    void *p1 = MapPersistentObject("Monkey Name1",0, 10);
//    void *p2 = MapPersistentObject("Monkey Name1",0, 6999);
//    void *p3 = MapPersistentObject("Monkey Name2",0, 10000);
//    void *p4 = MapPersistentObject("Monkey Name3",0, 50000);
//    void *p5 = MapPersistentObject("Monkey Name4",0, 5000);
//
//    
//    
//    int g1 = GetPersistentObjectSize("Monkey Name1");
//    int g2 = GetPersistentObjectSize("Monkey Name2");
//    int g3 = GetPersistentObjectSize("Monkey Name2");
//    int g4 = GetPersistentObjectSize("Monkey Name4");
//    
//    int u1 = UnMapPersistentObject(p1);
//    int u2 = UnMapPersistentObject(p2);
//    int u3 = UnMapPersistentObject(p3);
//    int u4 = UnMapPersistentObject(p4);
//    
//    int t1 = TruncatePersistentObject("Monkey Name1", 3000, 1000);
//    int t2 = TruncatePersistentObject("Monkey Name3", 10000, 3000);
//    int t3 = TruncatePersistentObject("Monkey Name4", 0, 1);
//
//    int d1 = DeletePersistentObject("Monkey Name1");
//    int d2 = DeletePersistentObject("Monkey Name2");
//    int d3 = DeletePersistentObject("Monkey Name3");
    
//    int warn = c1+c2+c3+c4+c5+g1+u1;
    


    
    halt();

    
    //        shutdown_machine();
}
















//    void *addr2 = (void *) (1<<21);
//    struct Temp *temp2;
//    temp2 = (struct Temp *)map_physical_page(addr2);
//    temp2->array[0] = 'c';
//    temp2->array[1] = 'b';
//    temp2->array[2] = '\0';
//    write_disk(0, 1, addr2);
//
//    void *addr3 = (void *) (1<<22);
//    struct Temp *temp3;
//    temp3 = (struct Temp *)map_physical_page(addr3);
//    temp3->array[0] = 'c';
//    temp3->array[1] = 'b';
//    temp3->array[2] = '\0';
//    write_disk(0, 1, addr3);


//    char *s = malloc(5);
//    sprintf(a, "s = %p",s);
//    write_console((unsigned) strlen(a), a);


// print characters to screen, just to see if clock interrupt will actually interrupt the kernel (it should!)
//    for (int i = 0; i < 100; i++)
//    {
//        sprintf(s,"%d ",i);
//        write_console((unsigned)strlen(s),s);
//    }

// Examine current value of jiffies
//    sprintf(s,"in Kernel: jiffies = %d \n",jiffies);
//    write_console((unsigned)strlen(s), s);
//
