//
//  kernel.c
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//  

#include "kernel.h"
#include "interrupts.h"

void kernel_start()
{
    set_debug_mode(DEBUG_ALL);
    reset_debug_mode();
    start_console();
    initialize_memory();
    start_disk();
    
    
    char a[80];
    sprintf(a, "Hello World \n");
    write_console((unsigned) strlen(a), a);
    
    // set the vector table
<<<<<<< HEAD
    set_ivec(I_CLK, ClockInterruptHandler);
    
    char s[20];
    
    // print characters to screen, just to see if clock interrupt will actually interrupt the kernel (it should!)
//    for (int i = 0; i < 10000; i++)
//    {
//        sprintf(s,"%d ",i);
//        write_console((unsigned)strlen(s),s);
//    }
    
    // Examine current value of jiffies
    sprintf(s,"jiffies = %d \n",jiffies);
    write_console((unsigned)strlen(s), s);
    
    
    
    
    
    
    shutdown_machine();
=======
    SetAllHandlers();
    
    halt(); //This halt is just to test the clock interrupt handler
    //shutdown_machine();
>>>>>>> origin/Ayman
}
