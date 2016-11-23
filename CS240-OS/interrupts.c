//
//  interrupts.c
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "interrupts.h"




int clockRunning = 0;
int jiffies = 0;
char consoleInput[100];
int noOfChars = 0;


// Clock Interrupt

void ClockInterrupt(int input)
{
    //    char *s = "Interrupt: Clock Interrupt!\n";
    //    write_console((unsigned)strlen(s),s);
    
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

void DiskInterrupt(int input)
{
    if (input == 0)
    {
        struct Temp *temp;
        void *addr = (void *)(1<<20);
        temp = (struct Temp *)map_physical_page(addr);
        read_disk(0, 1, addr);
        char b[512];
        sprintf(b, "%c", temp->array[1]);
        write_console((unsigned) strlen(b), b);
    }
    
    char s[50];
    sprintf(s,"Interrupt: Disk Interrupt! t_id = %d \n",input);
    write_console((unsigned) strlen(s), s);
    halt();
}

void MachineCheckInterrupt(int input)
{
    char s[50];
    sprintf(s,"Interrupt: Machine Check Interrupt!\n");
    write_console((unsigned) strlen(s), s);
    shutdown_machine();
}








