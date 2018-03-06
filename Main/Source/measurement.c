#include "config.h"
#include "measurement.h"

#ifndef ADCUC841_H
#define ADCUC841_H
#include <ADUC841.H>
#endif



extern volatile uint8 mode;

// misc variables
#ifdef USE_CIRCULAR_BUFFER
#define BUFSIZE 16
volatile uint16 circular_buffer[BUFSIZE] ={0}; // array to hold values
#else
volatile uint16 block_buffer =0;								// simple value to store sum

#endif
#define DC_AVG_NUM_SAMPLES BUFSIZE						// can afford to take more samples if block averaging

uint8 buff_index_counter =0;		//indexing variable for doing running sums/filtering
uint8 rms_buff_index_counter =0;



//dc averaging variables
#define DC_TIMER_OVERFLOWS 8		// n*5.9 milliseconds between readings. NOTE: must be a power of 2
#define DC_TIMER_INCREMENT 256/DC_TIMER_OVERFLOWS
uint16 dc_sum=0;
volatile uint16 dc_avg =0;		// value to be displayed, stored as 12bit number, convert to volts before display


// rms measurement variables
#define RMS_TIMER_OVERFLOWS 2		// n*5.9 milliseconds between readings. NOTE: must be a power of 2
#define RMS_TIMER_INCREMENT 256/RMS_TIMER_OVERFLOWS
#define RMS_MEASUREMENT_NUM_SAMPLES 128		//number of samples taken before reporting result
uint32 rms_sum;		//sum needs to be 32bit since sum is of squared readings
volatile uint16 rms_avg =0;

//p2p measurement variables

volatile uint16 p2p_value;



//freq measurement variables
extern volatile uint32 avg_freq;
extern volatile uint8 nb_overflow;
uint16 pulses_in_interval=0;
extern uint8 freq_method;
extern uint32 displayVariable;

void setup_timers_dc_averaging(){
		
		T2CON = 4;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;
	
	//disable other timers...

}
	
void setup_timers_rms_measurment(){		//same as dc averaging...
		T2CON = 4;  // all zero except run control
    ET2   = 1;     // enable timer 2 interrupt
		EA = 1;
}	

void setup_timers_freq_period_counting() {
		// Set up timer 2 in timer mode (bit1), capture mode(bit0), external control activated(bit3), timer run(bit2), serial not needed(bit4-5),then interrupt modified by hardware
//    T2CON = #00001101b;  // all zero except run control
	
	

	//choosing the frequency measurement method
//		if(displayVariable > 10000)
//		{
			freq_method = 1; //High frequency measurement
			setup_Timer0();   // call the function to set up timer 0 to count the number of falling edges 	
			
			T2CON = 4;  // all zero except run control T2CON = #00000100b
							// bit0 LOW to reload(timer 2 should have a smaller period than timer0)
							// bit1 LOW timer mode
							//bit2	HIGH	timer ON
							//bit3 LOW 	no external event
							//bit4-7 we don't care
			
			//Preload timer2
			RCAP2L=0x00;
			RCAP2H=0x28;
			
		
//		}
//		else
//		{
//				freq_method = 0; 
//				TR0 = 0; // stop 	Timer 0
//			
//				T2CON = 13;  // all zero except run control
//									// bit0 HIGH no reload neededa
//									// bit1 LOW timer mode
//									//bit2	HIGH	timer ON
//									//bit3 HIGH  external event needed
//									//bit4-7 we don't care
//		}
		ET2   = 1;     // enable timer 2 interrupt
		EA = 1;				// enable general interrupt
}
							
									
		
	
	


void setup_Timer0()
{
	//TCON = 17; //set up Timer 0 to count xxx1xx01
	// TMOD = 5; //xxxx 0101
	TCON = 0x44;
	TMOD = 0x50; 
}





void dc_voltage_measurment(){ // functions to store measurements as required
	
	static uint8 dc_voltage_num_timer_overflows=0; //counter for the number of timer overflows
	
	if (TF2 == 1){	//if counter overflow, increment counter
		// todo make the overflow happen in hardware
	dc_voltage_num_timer_overflows = dc_voltage_num_timer_overflows+DC_TIMER_INCREMENT;  // increment chosen to cause hardware overflow
	TF2 = 0;

		if(dc_voltage_num_timer_overflows ==0){	
			
			uint16 val = read_analog_input_pin();
//			dc_avg = read_analog_input_pin();
		#ifdef USE_CIRCULAR_BUFFER		// will update avg every time a value is read
			
			//circular buffer implementation
			
			buff_index_counter = (buff_index_counter + 1) % BUFSIZE;
			dc_sum = dc_sum - circular_buffer[buff_index_counter];				//subtract the old value from the running sum
			circular_buffer[buff_index_counter] = val;
			dc_sum = dc_sum + circular_buffer[buff_index_counter];				//add new value to sum
			dc_avg = dc_sum>>4;
//			

			
		#else													// update avg every after DC_AVG_NUM_SAMPLES readings
			
			//block buffer implementation 
			
			buff_index_counter = (buff_index_counter + 1) % DC_AVG_NUM_SAMPLES;
			dc_sum = dc_sum+val;
			if (buff_index_counter==0){	//after DC_AVG_NUM_SAMPLES readings, calcualte avg and reset sum
				dc_avg= dc_sum/DC_AVG_NUM_SAMPLES;
				dc_sum=0;	
			}

			
		#endif
			
		}
	}
	
	else if(EXF2==1)		//if the pin input triggered interrupt, reset
		EXF2 =0;
}	

void rms_measurment(){
	static uint32 sumSquare=0;
	static uint16 sumSample=0;
	static uint8 rms_measurement_num_timer_overflows=0;
	uint16 val=0;
	if (TF2 == 1){	//if counter overflow, increment counter
		
		rms_measurement_num_timer_overflows = rms_measurement_num_timer_overflows+RMS_TIMER_INCREMENT;
		
		TF2 = 0;
				
		val = read_analog_input_pin();
		
		sumSquare=sumSquare+(val*val);
		sumSample=sumSample+val;
		
		if (rms_measurement_num_timer_overflows==0){	
				dc_avg=sumSample/RMS_TIMER_OVERFLOWS;
				rms_avg=sumSquare/RMS_TIMER_OVERFLOWS-((uint32)(dc_avg*dc_avg));
				sumSquare=0;
				sumSample=0;
		}
		
		
		
//		buff_index_counter = (buff_index_counter + 1) % DC_AVG_NUM_SAMPLES;
//		dc_sum = dc_sum+val;
//		if (buff_index_counter==0){	//after DC_AVG_NUM_SAMPLES readings, calcualte avg and reset sum
//			dc_avg= dc_sum/DC_AVG_NUM_SAMPLES;
//			dc_sum=0;	
//			}
//		
//		
//			if(rms_measurement_num_timer_overflows ==0){	
//				
//				//uint16 val = read_analog_input_pin();
//				val = val-dc_avg;
//				buff_index_counter = (rms_buff_index_counter + 1) % RMS_MEASUREMENT_NUM_SAMPLES;
//				rms_sum = rms_sum+(val*val);		//running sum of squared readings
//				
//				if (buff_index_counter==0){	//after DC_AVG_NUM_SAMPLES readings, calculate rms and reset sum
//					rms_avg = my_sqrt((rms_sum/RMS_MEASUREMENT_NUM_SAMPLES));
//					rms_sum=0;
//				}	
			//}
		}

	else if(EXF2==1)		
		EXF2 =0;
} 				

void p2p_measurement(){} 			

void frequency_measurement() {
	//Setup the initial values of the static variables to ZERO
	static uint32 new_sample=0;
	static uint16 old_sample=0;
	
//	if (freq_method){	//time interval elapsed, read value in 16 bit timer 1 register
	
		TF2=0;
		new_sample = ((uint16) TH1<<8)| ((uint16) TL1);
		if (new_sample<=old_sample){ //overflow occured, account for this
		pulses_in_interval = (uint16)((uint32)(new_sample+65536)-old_sample);

		}
		else {
			pulses_in_interval = new_sample-old_sample;
		}
		old_sample = new_sample;// log value fro next interrupt

//	}
//		
//	else{
//			
//		if(EXF2==1){ // new edges incoming (of the periodic signal we want to measure) => end of the a period
//			//What is the new sample ?
//			
//			new_sample = ((uint32)nb_overflow<<16)	| ((uint32)RCAP2H<<8) | ((uint32)RCAP2L); //concatenate the 3 bytes
//			new_sample -=old_sample;
//			
//			//Update the average using IIR filter
//			avg_freq=(new_sample*3)/20 + (avg_freq*17)/20;   //alpha chosen 0.15=3/20
//			
//			//Prepare the next interruption
//			old_sample= ((uint16)RCAP2H)<<8 | ((uint16)RCAP2L);

//			nb_overflow=0;
//			EXF2=0;//clear the flag
//		}
//			else	//EXF2==0 and TF2==1
//		{
//				nb_overflow++;
//				TF2=0; //clear the flag
//		} 
//	}
}
	
uint16 read_analog_input_pin(){
//CS0 = 0;	//confirm analog pin
//CS1 = 1;
//CS2 = 0;
//CS3 = 0;
uint8 val_LSB = ADCDATAL;
uint8 val_MSB = ADCDATAH & 0x0F;
uint16 val = (val_MSB<<8)+val_LSB;
	return val;
}	


/*------------------------------------------------
Interrupt service routine for timer 2 interrupt.
Called by the hardware when the interrupt occurs.
------------------------------------------------*/
void timer2 (void) interrupt 5   // interrupt vector at 002BH
{
	// time effiency requires bitwise operation with if/else statements
	switch(mode){
		case 0:		dc_voltage_measurment(); 	break;
		case 1:		rms_measurment(); 				break;
		case 2:		p2p_measurement(); 				break;
		case 3:		frequency_measurement();	break;
	}		
	
}	// end timer2 interrupt service routine



uint16 analog_reading_to_voltage(uint16 value){
	return ((value*VOLTAGE_RANGE)/4096);
}

uint8 my_sqrt(uint16 squared_val){
	
	/* method:
	sum odd numbers up to squared_val
	number of values used in sum is root
	
	1+3 = 4 				-> 2 values in sum, 2 is root of 4
	1+3+5 = 9				-> 3 is root 9
	1+3+5+7 = 16		-> 4 is root 16
	1+3+5+7+9 = 25	-> 5 is root 25
	...
	
	this method should be fast since no multiplication required
	to identify the value
	
	*/
	
	uint16 i=0;		//odd numbers to be added to sum
	uint16 x=0;		//running total
	uint16 count=0;
	for(i=1;x<squared_val;i+=2)
        {
            x = x + i;
            count++;

        }
	if (count*count == squared_val)	
		return count;
	
	else 
		return (count-1);
}


 



