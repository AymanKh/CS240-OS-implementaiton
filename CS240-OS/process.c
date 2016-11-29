//
//  process.c
//  CS240-OS
//
//  Created by A Y M A N on 11/28/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "process.h"
#include "hardware_interface.h"


void Exit()
{
    char b[60];
    sprintf(b,"a process called Exit()!\n");
    write_console(40,b);
    
    halt();
    
}

