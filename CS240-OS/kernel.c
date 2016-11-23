//
//  kernel.c
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#include "kernel.h"


void kernel_start()
{
//    set_debug_mode(DEBUG_ALL);
    reset_debug_mode();
    start_console();
    initialize_memory();
    start_disk();
    
    
    char a[80];
    sprintf(a, "Hello World %d \n",jiffies);
    write_console((unsigned) strlen(a), a);

    // get the address of the clock interrupt
    void (*clockInterruptPointer)(int) = &ClockInterrupt;
    void (*consoleInterruptPointer)(int) = &ConsoleInterrupt;
    void (*diskInterruptPointer)(int) = &DiskInterrupt;
    void (*machineCheckInterruptPointer)(int) = &MachineCheckInterrupt;
    
    // set the vector table
    set_ivec(I_CLK, clockInterruptPointer);
    set_ivec(I_CNSL, consoleInterruptPointer);
    set_ivec(I_DSK, diskInterruptPointer);
    set_ivec(I_CHECK, machineCheckInterruptPointer);
    
    
    // Testing POS
    CreatePersistentObject("Monkey Name");
    GetPersistentObjectSize("Monkey Name");
    DeletePersistentObject("Monkey Name");
    
    
//    
//    char *addr = (char *) (1<<20);
//    struct Temp *temp;
//    temp = (struct Temp *)map_physical_page(addr);
//    temp->array[0] = 'c';
//    temp->array[1] = 'b';
//    temp->array[2] = '\0';
//    write_disk(0, 1, addr);
    
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
    
  
    
    
    halt();
//    shutdown_machine();
}


















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
