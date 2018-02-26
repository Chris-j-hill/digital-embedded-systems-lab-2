#include "screen.h"
#include <ADUC841.H>
#include "config.h"

extern uint8 mode;


extern volatile uint16 dc_avg;		// <<<  use these value when displaying
extern volatile uint16 rms_avg;
extern volatile uint16 p2p_value;
extern volatile uint32 avg_freq;

void init_screen(){}			//initilisation bits for screen

void update_display(){
	uint32 frequency;
	uint32 clock;
	switch(mode){		//setup hardware based on new mode
		case 0:
		case 1:			break;
		case 2:			break;
		case 3:  // frequency
			
		//Compute the frequency
//		frequency =((uint32) CLOCK_SPEED)/avg_freq; //Hz
//			
		clock=119526;
		frequency = clock/avg_freq; //Hz
		
		break;

					
	}

}	      //push values to display


