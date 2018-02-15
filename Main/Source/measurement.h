#ifndef MEASUREMENT_H
#define MEASUREMENT_H

typedef unsigned char uint8;				// 8-bit unsigned integer
typedef unsigned short int uint16;	// 16-bit unsigned integer
typedef unsigned int uint32;		//32 bit unsigned int
	
void setup_timer2_freq_period_counting();
	
void dc_voltage_measurment(); // functions to store measurements as required
void rms_measurment(); 				
void p2p_measurement(); 			
void frequency_measurement();

void timer2 (void);

#endif