//
//  defines.h
//  CS240-OS
//
//  Created by A Y M A N on 12/5/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#ifndef defines_h
#define defines_h

#include "uthash.h"


typedef struct blockNode blockNode;

struct blockNode {
    int blockPosition;
    blockNode *next; /* needed for singly- or doubly-linked lists */
};


typedef struct _cont {
    void (* func)();
    signed int tid;
    void *arg1;
    int arg2;
    UT_hash_handle hh;         /* makes this structure hashable */
} cont;

typedef struct _KeynameHash {
    void *keynameH;
    int size;
    int mappedFlag;
    blockNode *blocksHead;
    UT_hash_handle hh;         /* makes this structure hashable */
} keynameHash ;

int test;



#endif /* defines_h */
