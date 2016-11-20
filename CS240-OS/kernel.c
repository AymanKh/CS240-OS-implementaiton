//
//  kernel.c
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#include "kernel.h"
#include "interrupts.h"
#include "console_interface.h"
#include "bitmap.h"

void kernel_start()
{
    //set_debug_mode(DEBUG_ALL);
    reset_debug_mode();
    
    start_console();
    initialize_memory();
    start_disk();
    
    char a[80];
    sprintf(a, "Hello World \n");
    write_console((unsigned) strlen(a), a);
        
    // set the vector table
    SetAllHandlers();
    
    
    
    
    
    
    //TODO : Remove this block after you are done from testing
    //***************** THIS BLOCK JUST FOR TESTING ********************//
    
    //The following two is just to switch to user mode and test infinit loop for the clock interrupt
    //iret();
    //while(1);
    
    //Testing the read from the memory
    //void *address = (void *)(1<<22) ;
    //char *memoryAddress = map_physical_page(address);
    //memoryAddress = "This is readed from disk \0" ;
    //write_disk(0, 1, address) ;
    //read_disk(0, 1, address) ;
    //write_console((unsigned) strlen(memoryAddress), memoryAddress);
    
    
    //******************************************************************//
    
    
    
    
    
    
    //One of the following three options should be done upon initializing the handler vectors
    //iret();
    halt(); //TODO: should we keep this to make OS running all the time ?
    //shutdown_machine();
}





