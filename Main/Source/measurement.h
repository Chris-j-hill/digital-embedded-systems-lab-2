#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "config.h"

	
void setup_timers_dc_averaging();	
void setup_timers_freq_period_counting();
	
void dc_voltage_measurment(); // functions to store measurements as required
void rms_measurment(); 				
void p2p_measurement(); 			
void frequency_measurement();
uint16 read_analog_input_pin();		//return voltage from analog pin
uint8 my_sqrt(uint16 squared_val);	//square root calculater
uint8 analog_reading_to_voltage(uint16 value);	//convert a value from 12bit reading to voltage
void timer2 (void);


#endif