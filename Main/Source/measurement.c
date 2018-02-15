#include "measurement.h"
#include <ADUC841.H>

extern volatile uint8 mode;

void setup_timer2_freq_period_counting() {
		// Set up timer 2 in timer mode (bit1), capture mode(bit0), external control activated(bit3), timer run(bit2), serial not needed(bit4-5),then interrupt modified by hardware
    //T2CON = 0b00001101;  // all zero except run control
    T2CON = 13;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;				// enable general interrupt
	
		//uint8 nb_t2_of =0;
}


void dc_voltage_measurment(){} // functions to store measurements as required
void rms_measurment(){} 				
void p2p_measurement(){} 			
void frequency_measurement(){}
	
	/*------------------------------------------------
Interrupt service routine for timer 2 interrupt.
Called by the hardware when the interrupt occurs.
------------------------------------------------*/
void timer2 (void) interrupt 5   // interrupt vector at 002BH
{
//	static period
//	if(TF2==1)
//	{
//			nb_t2_of++;
//	}
//	else //EXF2 ==1
//	{
//			
//	}
//	
//	TF2 = 0;											// clear interrupt flag
//	EXF2 = 0;
	
	switch(mode){
		case 0:		dc_voltage_measurment(); 	break;
		case 1:		rms_measurment(); 				break;
		case 2:		p2p_measurement(); 				break;
		case 3:		frequency_measurement();	break;
	}		
	
}	// end timer2 interrupt service routine



