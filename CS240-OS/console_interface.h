//
//  console_interface.h
//  CS240-OS
//
//  Created by Maryam on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef console_interface_h
#define console_interface_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interrupts.h"
#include "hardware_interface.h"


int ReadConsole(char * buffer, int nbytes);
int WriteConsole(char * buffer, int nbytes);

#endif /* console_interface_h */
