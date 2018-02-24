#include <ADUC841.H>

#include "config.h"
#include "measurement.h"
#include "screen.h"


// global variables
uint8 mode = 0;                 // mode of operation as read from pins
uint32 avg_freq = 0;
uint8 nb_overflow =0;



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
		delay(SCREEN_UPDATE_DELAY_TIME);
	}
}

void init_pins(){
  //switch_pins

	SWITCH_PORT = SWITCH_PORT_MASK;	//explicitely set these pins leaving rest of port alone
	
	ADCCON1 = 0x8c;		//init adc
	ADCCON2 = 0x22;
	ADCCON3 = 0x00;
	
	
	
	//init output pins for screen

}


void get_mode_from_pins(){		//read mode from port

	uint8 pin_state;
	//read pins...
	pin_state = SWITCH_PORT & SWITCH_PORT_MASK
	//if pin state != mode, setup hardware
	if (pin_state != mode){	
		mode = pin_state;
		
		switch(mode){		//setup hardware based on new mode
			case 0: 	setup_timers_dc_averaging();			break;
			case 1:					break;
			case 2:					break;
			case 3:		setup_timers_freq_period_counting();			break;
			
		}
	}
}		

void delay(uint32 period){
	
unsigned int i;		// counting variable
	period = period*CLOCK_CYCLES_IN_ONE_MS/CLOCK_CYCLES_IN_FOR_LOOP;
	 
	for (i = 0; i < period; i++)    // count clock cycles
	// Nb: the for loop above takes 22 clock cycles to execute as assembly
    {}

}		

