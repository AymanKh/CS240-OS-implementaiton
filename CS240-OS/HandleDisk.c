////
////  HandleDisk.c
////  CS240-OS
////
////  Created by A Y M A N on 11/25/16.
////  Copyright © 2016 A Y M A N. All rights reserved.
////
//
//#include "HandleDisk.h"
//
//cont *hashTableTid = NULL;
//
//
//void readDiskWrapper()
//{
////    int x = 10;
//    int *limit = malloc(sizeof(int));
//    *limit = 10;
//    
//    void *addr2 = (void *)(1<<20);
//    cont *hashTest = malloc(sizeof(cont));
//    hashTest->func = &printTheStuff;
//    hashTest->arg1 = limit;
//    hashTest->tid = read_disk(0,8,addr2);
//    
//    HASH_ADD_INT(hashTableTid, tid, hashTest);
//    
//    
//    //        char b[30];
//    //        sprintf(b, "kernel: read from disk: %c\n", temp2->array[1]);
//    //        write_console((unsigned) strlen(b), b);
//    
//    
//    
//    
//    
//}
//
//void printTheStuff(int *limit)
//{
//    char s[60];
//    
//    int x = *limit;
//    
//    for(int i = 0;i < x; i++)
//    {
//        sprintf(s,"i = %d\n", i);
//        write_console(7,s);
//    }
//    
//}
