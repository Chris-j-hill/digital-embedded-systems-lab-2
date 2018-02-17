#ifndef SCREEN_H
#define SCREEN_H

#ifndef DATA_TYPES
#define DATA_TYPES
typedef unsigned char uint8;				// 8-bit unsigned integer
typedef unsigned short int uint16;	// 16-bit unsigned integer
typedef unsigned int uint32;		//32 bit unsigned int
#endif



void init_screen();			//initilisation bits for screen

void update_display();	      //push values to display


#endif