//
//  kernel.c
//  Test240
//
//  Created by Mootaz N. Elnozahy on 11/12/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#include "kernel.h"

void kernel_start()
{
    start_console();
    initialize_memory();
    start_disk();
    
    char a[80];
    sprintf(a, "Hello World\n");
    write_console((unsigned) strlen(a), a);
    
    shutdown_machine();
}
