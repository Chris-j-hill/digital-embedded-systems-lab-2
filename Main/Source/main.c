
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
		delay(SCREEN_UPDATE_DELAY_TIME);
	}
}

void init_pins(){
  //switch_pins

	SWITCH_PORT = SWITCH_PORT_MASK;	//explicitely set these pins leaving rest of port alone
	
	ADCCON1 = 0x8c;		//init adc
	ADCCON2 = 0x22;
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
		
		
	//}
}		

void delay(uint32 period){
	
unsigned int i;		// counting variable
	period = period*CLOCK_CYCLES_IN_ONE_MS/CLOCK_CYCLES_IN_FOR_LOOP;
	 
	for (i = 0; i < period; i++)    // count clock cycles
	// Nb: the for loop above takes 22 clock cycles to execute as assembly
    {}

}		

