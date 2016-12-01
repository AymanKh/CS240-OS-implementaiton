//
//  console_interface.c
//  CS240-OS
//
//  Created by A Y M A N on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "console_interface.h"
//#include "interrupts.h"

int nbytes;


int WriteConsole(char *buffer, int nbytes)
{
    void *p = map_physical_page((void*)buffer);
    write_console(nbytes, p);
    
    return 0;
}

int ReadConsole(char *buffer, int nbytes)
{
    nbytes = nbytes;
    char *x = &consoleInput[0];
    write_console(1, x);
    
    return 0;
}
