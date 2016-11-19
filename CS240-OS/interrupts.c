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


int clockRunning = 0;
int jiffies = 0;


// Clock Interrupt

void ClockInterrupt(int input)
{
    char *s = "Interrupt: Clock Interrupt!\n";
    write_console((unsigned)strlen(s),s);
    
    if(!clockRunning)
    {
        clockRunning = 1;
    }
    else
    {
        jiffies++;
    }
    
    //    iret();
    //    currTime++;
    
}
