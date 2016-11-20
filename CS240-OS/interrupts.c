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

//Variables for the clock :-
int clockRunning = 0 ;  //To check and set if the clock is running
int currentTime = 0 ;   //To get current time for the sake of scheduling

//Variables for the console :-
int numberOfStoredLetters ;
char theUserInput[1000] ;   //TODO: Is this a good way of defining the buffer, or should we make it pointer

void ClockInterruptHandler(int input)
{
    if(clockRunning){
        //char clockMessage[80];
        //sprintf(clockMessage, "Interrupt: Clock Interrupt! Current Time = %d\n",currentTime);
        //write_console((unsigned)strlen(clockMessage),clockMessage);
        currentTime++ ;
    } else clockRunning = 1 ;
    
    //TODO: This could be halt() or iter(). Need to be modified later based on current time and waiting processes
    //halt();
    iret();
}

void DiskInterruptHandler(){
    
}

void ConsoleInterruptHandler(char charFromConsole){
    char consoleMessage[80];
    sprintf(consoleMessage, "Interrupt: Console Interrupt! Character = %c\n",charFromConsole);
    write_console((unsigned)strlen(consoleMessage),consoleMessage);
    
    //These two lines to store any characters from the console interrupt until it works.
    theUserInput[numberOfStoredLetters] = charFromConsole ; //Store the letter from the console into the buffer
    numberOfStoredLetters++ ;                               //Incremeant the location of the next character in the buffer
    
    //TODO: This could be halt() or iter(). Need to be modified later based on current time and waiting processes
    halt();
}

void TrapInterruptHandler(){
    
}

void ExceptionInterruptHandler(){
    
}

void CheckInterruptHandler(){
    char clockMessage[80];
    sprintf(clockMessage, "Interrupt: Check Exception!\nMachine Will be Shutdown...");
    write_console((unsigned)strlen(clockMessage),clockMessage);
    shutdown_machine();
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
    numberOfStoredLetters = 0 ;
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
