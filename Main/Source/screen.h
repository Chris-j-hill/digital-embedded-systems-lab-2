#ifndef SCREEN_H
#define SCREEN_H



void init_screen();			//initilisation bits for screen

void update_display();	      //push values to display
void sendByteToSPI(uint8);

#endif