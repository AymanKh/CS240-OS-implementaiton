//
//  bitmap.c
//  CS240-OS
//
//  Created by Maryam on 11/20/16.
//  Copyright © 2016 A Y M A N. All rights reserved.
//

#define NUMBER_OF_BLOCKS            131072  //TODO : Make it calculated and hard coded like this (because block size = 8 secotrs)
#define SIZE_OF_BIT_ELEMENT         64      //TODO : This is the size of the bit element in the bit element array
#define BITMAP_NOTFOUND             -1

void bitmapGet   ();
void bitmapSet   ();
void bitmapReset ();
void bitmapSearch();

unsigned long long bitMap[2048] ;          //TODO : The array that will hold all the bits for all block -> Each elemnt responsible for 64 blocks

void bitmapGet(int blockID) {
    
}

void bitmapSet() {
    
}

void bitmapReset() {
    
}

void bitmapSearch() {
    
}

