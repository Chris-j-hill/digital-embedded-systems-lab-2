#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#ifndef DATA_TYPES
#define DATA_TYPES
typedef unsigned char uint8;				// 8-bit unsigned integer
typedef unsigned short int uint16;	// 16-bit unsigned integer
typedef unsigned int uint32;		//32 bit unsigned int
#endif


	
void setup_timers_dc_averaging();	
void setup_timers_freq_period_counting();
	
void dc_voltage_measurment(); // functions to store measurements as required
void rms_measurment(); 				
void p2p_measurement(); 			
void frequency_measurement();
uint16 read_analog_input_pin();		//return voltage from analog pin


void timer2 (void);

#endif