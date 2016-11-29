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
        (test->func)(test->arg1, test->arg2);
    }
    else
    {
        (test->func)();
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
    
    switch(trapNo)
    {
        case TRAP_EXIT:
            Exit();
            break;
        case TRAP_WRITE_CONSOLE:
            WriteConsole((char *)machine_context.reg[12], (int)machine_context.reg[13]);
            break;
    }
    
    
    
    
}






