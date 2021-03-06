
#include "config.h"
#include "measurement.h"
#include "screen.h"


// global variables
uint8 mode = 3;                 // mode of operation as read from pins
uint32 avg_freq = 0;
uint16 nb_overflow =0;					//to make a 32bits counter of clock cycles in the freq calculation
uint8 freq_method = 0; // 1 = high frequency method,  0 = low frequency method



// function prototypes
void init_pins();
void get_mode_from_pins();		//read mode from port
void delay(uint16 period);		


void main (void) {
	// setup
  init_pins();		//set switches and analog pins as input, leds as output and enable screen pins
	init_screen();
	
	//loop
	while(1){
		update_display();
		get_mode_from_pins();
		//delay(SCREEN_UPDATE_DELAY_TIME);
		delay(500000);
	}
}

void init_pins(){
  //switch_pins

	SWITCH_PORT = SWITCH_PORT_MASK;	//explicitely set these pins leaving rest of port alone
	
	ADCCON1 = 0x8c;	//1000 1100	: bit7=ADC on, bit6 no external reference,bit5-6 LOW biggest divider=lowest frequency to sample, bit3-2 means 4 clocks cycle to get the conversion done, bit0-1 LOW to disable external trigerring of the ADC 
	ADCCON2 = 0x22;	//0010 0010 : bit7 LOW not us = interupt from hardware, bit 6 LOW not using DMA mode,bit5 :continuous conversion = always working, bit4 LOW not Single Conversion Bit , nit 3-2-1-0 channel 2 selected
	ADCCON3 = 0x00;
	
	T2EX =0;
	
	//init output pins for screen
	switch(mode){		//setup hardware based on new mode
			case 0: 	setup_timers_dc_averaging();			break;
			case 1:		setup_timers_rms_measurment();		break;
			case 2:					break;
			case 3:		setup_timers_freq_period_counting();			break;		
		}
}


void get_mode_from_pins(){		//read mode from port
// we don't care about the time effiency of that function since we delay it later
	// so it iplies switch case is fine and not comparison to previous state of mode variable

	mode = SWITCH_PORT & SWITCH_PORT_MASK;
	
//		uint8 pin_state;
//	pin_state = SWITCH_PORT & SWITCH_PORT_MASK;
//	if (pin_state != mode){	
//		mode = pin_state;
		//init output pins for screen
	switch(mode){		//setup hardware based on new mode
			case 0: 	setup_timers_dc_averaging();			break;
			case 1:		setup_timers_rms_measurment();		break;
			case 2:					break;
			case 3:		setup_timers_freq_period_counting();			break;		
		}
		
	//}
}		

void delay(uint32 period){
	
	uint32 i;		// counting variable
//	period = period*CLOCK_CYCLES_IN_ONE_MS/CLOCK_CYCLES_IN_FOR_LOOP;
	 
	for (i = 0; i < period; i++)    // count clock cycles
	// Nb: the for loop above takes 22 clock cycles to execute as assembly
    {}

}		

