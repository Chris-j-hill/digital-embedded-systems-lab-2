#include <ADUC841.H>

// define some useful variable types
typedef unsigned char uint8;				// 8-bit unsigned integer
typedef unsigned short int uint16;	// 16-bit unsigned integer

/*------------------------------------------------
Interrupt service routine for timer 2 interrupt.
Called by the hardware when the interrupt occurs.
------------------------------------------------*/
void timer2 (void) interrupt 5   // interrupt vector at 002BH
{
	static period
	if(TF2==1)
	{
			nb_t2_of++;
	}
	else //EXF2 ==1
	{
			
	}
	
	TF2 = 0;											// clear interrupt flag
	EXF2 = 0;
	
}	// end timer2 interrupt service routine

void setup_timer2 (void) {
		// Set up timer 2 in timer mode (bit1), capture mode(bit0), external control activated(bit3), timer run(bit2), serial not needed(bit4-5),then interrupt modified by hardware
    T2CON = 0b00001101;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;				// enable general interrupt
	
		uint8 nb_t2_of =0;
	
	
}

void main (void) {
	
		period=0;
	
	setup_timer2();
}
