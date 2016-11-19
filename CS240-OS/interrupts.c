//
//  interrupts.c
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interrupts.h"
#include "hardware_interface.h"

void ClockInterruptHandler(int input)
{
    char *s = "Interrupt: Clock Interrupt!\n";
    write_console((unsigned)strlen(s),s);
    
    //TODO: This could be halt() or iter(). Need to be modified later
    halt();
}

void DiskInterruptHandler(){
 
}

void ConsoleInterruptHandler(){
    
}

void TrapInterruptHandler(){
    
}

void ExceptionInterruptHandler(){
    
}

void CheckInterruptHandler(){
    
}


/*
 * This method will define all the handler methods
 */

void SetAllHandlers(){
    //These two lines define the clock interrupt handler
    void (*clockInterruptPointer)(int) = &ClockInterruptHandler;
    set_ivec(I_CLK, clockInterruptPointer);
    
    //These two lines define the disk interrupt handler
    void (*diskInterruptPointer)(int) = &DiskInterruptHandler;
    set_ivec(I_DSK, diskInterruptPointer);
    
    //These two lines define the console interrupt handler
    void (*consoleInterruptPointer)(int) = &ConsoleInterruptHandler;
    set_ivec(I_CNSL, consoleInterruptPointer);
    
    //These two lines define the trap interrupt handler
    void (*trapInterruptPointer)(int) = &TrapInterruptHandler;
    set_ivec(I_TRAP, trapInterruptPointer);
    
    //These two lines define the exception interrupt handler
    void (*exceptionInterruptPointer)(int) = &ExceptionInterruptHandler;
    set_ivec(I_EXCEPT, exceptionInterruptPointer);
    
    //These two lines define the check interrupt handler
    void (*checkInterruptPointer)(int) = &CheckInterruptHandler;
    set_ivec(I_CHECK, checkInterruptPointer);
    
    char *s = "Progress: Set All Handlers Successfully!\n";
    write_console((unsigned)strlen(s),s);
}
