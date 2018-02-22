#include "measurement.h"
#include <ADUC841.H>
#include "config.h"

extern volatile uint8 mode;


//dc averaging variables
#ifdef USE_CIRCULAR_BUFFER
#define BUFSIZE 16
volatile uint16 circular_buffer[BUFSIZE] ={0}; // array to hold values
#else
volatile uint16 block_buffer =0;			// simple value to store sum
#define DC_AVG_NUM_SAMPLES 128
#endif

#define DC_TIMER_OVERFLOWS 10		// n*5.9 milliseconds between readings
uint8 dc_voltage_num_timer_overflows =0;

uint8 dc_avg_counter =0;
uint16 dc_sum;
volatile uint16 dc_avg;		// <<<  display this value

//freq measurment
extern volatile uint32 avg_freq;
extern volatile uint8 nb_overflow;


void setup_timers_dc_averaging(){
		
		T2CON = 7;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;
	
	//disable other timers...

}
	
	

void setup_timers_freq_period_counting() {
		// Set up timer 2 in timer mode (bit1), capture mode(bit0), external control activated(bit3), timer run(bit2), serial not needed(bit4-5),then interrupt modified by hardware
    //T2CON = 0b00001101;  // all zero except run control
    T2CON = 13;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;				// enable general interrupt
	
		//uint8 nb_t2_of =0;
	
	
	//disable other timers...
	
}






void dc_voltage_measurment(){ // functions to store measurements as required

	if (TF2 == 1){	//if counter overflow, increment counter
	dc_voltage_num_timer_overflows = (dc_voltage_num_timer_overflows+1) % DC_TIMER_OVERFLOWS;
	TF2 = 0;

		if(dc_voltage_num_timer_overflows ==0){	
			
			uint16 val = read_analog_input_pin();
			
		#ifdef USE_CIRCULAR_BUFFER		// will update avg every time a value is read
			
			//circular buffer implementation
			
			dc_avg_counter = (dc_avg_counter + 1) % BUFSIZE;
			dc_sum = dc_sum - circular_buffer[dc_avg_counter];				//subtract the old value from the running sum
			circular_buffer[dc_avg_counter] = val;
			dc_sum = dc_sum + circular_buffer[dc_avg_counter];				//add new value to sum
			dc_avg = dc_sum/BUFSIZE;
			
			
			
		#else													// update avg every after DC_AVG_NUM_SAMPLES readings
			
			//block buffer implementation 
			
			dc_avg_counter = (dc_avg_counter + 1) % DC_AVG_NUM_SAMPLES;
			if (dc_avg_counter==0){	//after DC_AVG_NUM_SAMPLES readings, reset sum
				dc_avg= dc_sum/DC_AVG_NUM_SAMPLES;	//calculate avg
				dc_sum=0;	//reset_sum		
			}
			dc_sum = dc_sum+val;
			
		#endif
			
		}
	}
	
	else if(EXF2==1)		//if for some reason this triggered interrupt, reset to stop continual interrupts
		EXF2 =0;
}	

void rms_measurment(){} 				

void p2p_measurement(){} 			

void frequency_measurement() 
{
	//Setup the initial values of the static variables to ZERO
	static uint32 new_sample=0;
	static uint8 past_RCAP2H=0;
	static uint8 past_RCAP2L=0;
	
	
	if(EXF2==1){ // new edges incoming (of the periodic signal we want to measure) => end of the a period
		//What is the new sample ?
		new_sample = (nb_overflow<<16)	+ ((RCAP2H-past_RCAP2H)<<8) + (RCAP2L-past_RCAP2L); //concatenate the 3 bytes
		//Update the average using IIR filter
		avg_freq=(new_sample*3)/20 + (avg_freq*17)/20;   //alpha chosen 0.15=3/20
		
		//Prepare the next interruption
		past_RCAP2H=RCAP2H;
		past_RCAP2L=RCAP2L;
		nb_overflow=0;
		EXF2=0;//clear the flag
	}
		else	//EXF2==0 and TF2==1
	{
			nb_overflow++;
			TF2=0; //clear the flag
	} 
	
}
	
uint16 read_analog_input_pin(){
uint16 val =0;
	
	return val;
}	
	

/*------------------------------------------------
Interrupt service routine for timer 2 interrupt.
Called by the hardware when the interrupt occurs.
------------------------------------------------*/
void timer2 (void) interrupt 5   // interrupt vector at 002BH
{
	switch(mode){
		case 0:		dc_voltage_measurment(); 	break;
		case 1:		rms_measurment(); 				break;
		case 2:		p2p_measurement(); 				break;
		case 3:		frequency_measurement();	break;
	}		
	
}	// end timer2 interrupt service routine



