//
//  console_interface.c
//  CS240-OS
//
//  Created by Maryam on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#include "console_interface.h"


extern int numberOfStoredLetters ;
extern char theUserInput[1000] ;   //TODO: Is this a good way of defining the buffer, or should we make it pointer

int ReadConsole(char * buffer, int nbytes){
    int numberOfReadedCharacters = 0 ;
    
    while(numberOfStoredLetters == 0) ;                                                      //Wait at least for one character
    while(numberOfStoredLetters < nbytes && theUserInput[numberOfStoredLetters-1] != '\n') ; //Busy waiting for all characters to be printed or terminated
    memcpy( buffer, &theUserInput[nbytes+1], nbytes );
    buffer[nbytes] = '\0' ;
    numberOfReadedCharacters = (unsigned)strlen(buffer) ;
    
    //Test the message been printed
    char theReadedMessage[80];
    sprintf(theReadedMessage, "(console_interface) Readed Message: %s\n",buffer);
    write_console(numberOfReadedCharacters,theReadedMessage);
    
    return numberOfReadedCharacters ;
}

int WriteConsole(char * buffer, int nbytes){
    return -1 ;
}
