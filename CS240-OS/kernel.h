//
//  kernel.h
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#ifndef kernel_h
#define kernel_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interrupts.h"
#include "hardware_interface.h"
#include "console_interface.h"
//#include "BitMap.h"




void kernel_start();
extern int jiffies;
//extern char *consoleInput[100];
//extern int nbytes;
//extern void ClockInterrupt(int input);


#endif /* kernel_h */
