//
//  interrupts.h
//  CS240-OS
//
//  Created by A Y M A N on 11/17/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef interrupts_h
#define interrupts_h

#include <stdio.h>

void ClockInterruptHandler();
void DiskInterruptHandler();
void ConsoleInterruptHandler(char charFromConsole);
void TrapInterruptHandler();
void ExceptionInterruptHandler();
void CheckInterruptHandler();
void SetAllHandlers();

#endif /* interrupts_h */
