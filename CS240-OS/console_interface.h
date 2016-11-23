//
//  console_interface.h
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef console_interface_h
#define console_interface_h

#include <stdio.h>
#include "hardware_interface.h"
#include "interrupts.h"
//#include "kernel.h"


int ReadConsole(char *buffer, int nbytes);
int WriteConsole(char *buffer, int nbytes);

//extern int nbytesInput;
extern char consoleInput[100];
//int nbytes;


#endif /* console_interface_h */
