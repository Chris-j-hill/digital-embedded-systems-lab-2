#include "screen.h"

#ifndef ADCUC841_H
#define ADCUC841_H
#include <ADUC841.H>
#endif

#include "config.h"
#include "measurement.h"
//#include <math.h>

extern uint8 mode;


extern volatile uint16 dc_avg;		// <<<  use these value when displaying
extern volatile uint16 rms_avg;
extern volatile uint16 p2p_value;
extern volatile uint32 avg_freq;
extern uint8 freq_method;
extern uint16 pulses_in_interval;
uint32 displayVariable=0;


const uint8 displaytable[]={	// 0     1     2     3     4     5     6     7     8     9
														0x6F, 0x28, 0x5D, 0x7C, 0x3a, 0x76, 0x77, 0x2c, 0x7f, 0x3e};

void init_screen(){
		uint8 i;
    SPICON=0x33; //initilisation bits for screen
		Slave_select = 0;

	//reset screen display to zeros
		sendByteToSPI(1);
	for(i=0;i<5;i++)
		{
			sendByteToSPI(0);
		}
		
}			

void update_display(){


	uint8 index;
	
	switch(mode){		//setup hardware based on new mode
		case 0:

			displayVariable = (((uint32)dc_avg)*VOLTAGE_RANGE)>>12;

		break;
		case 1:
			displayVariable =rms_avg;
			//displayVariable = my_sqrt(rms_avg);
			//displayVariable = (((uint32)displayVariable)*VOLTAGE_RANGE)>>12;
		break;
		case 2:			break;
		case 3:  // frequency
		
	
//		//Compute the frequency
//		if(freq_method==0) //we are measuring a low frequency
//		{
//			displayVariable=CLOCK_SPEED/avg_freq; //Hz		
//			//displayVariable = clock/avg_freq; //Hz
//		}
//		else //  freq_method = 1  , we are measuring a high frequency
			displayVariable = ((uint32)pulses_in_interval)*200;
			
		if (displayVariable>9999)
			displayVariable/=1000; //so freq is in kHz

		break;
	}	
		
		sendByteToSPI(1);
		for(index=0;index<4;index++) //startint with the least significant number
		{	
			sendByteToSPI(displaytable[displayVariable%10]);
			displayVariable=displayVariable/10;
		}
		sendByteToSPI(0);
}	

void sendByteToSPI(uint8 byte)
{
	SPIDAT=byte;
	while(!ISPI);
	ISPI=0;
}
//}	      //push values to display




 

//sbit Slave_select = P3_6;  // initialize Slave Select bit to ENABL

//SPICON=0x33;  //configure the SPICON

 

//change the data to DEC, and use a table to find the HEX to display



//transmit the data to SPIDAT in 4 bytes

