
#ifndef DATA_TYPES
#define DATA_TYPES

#ifndef ADCUC841_H
#define ADCUC841_H
#include <ADUC841.H>
#endif

typedef unsigned char uint8;				// 8-bit unsigned integer
typedef unsigned short int uint16;	// 16-bit unsigned integer
typedef unsigned long int uint32;		//32 bit unsigned int

#define SCREEN_UPDATE_DELAY_TIME 500	//delay for screen update in milliseconds
#define SWITCH_PORT  P2			// switches are connected to Port 2
#define SWITCH_PORT_MASK 0x3;	//00000011
#define SWITCH_PORT_MODE 0xFF	// 00 = all output FF = all input


#define ANALOG_INPUT_PORT P1
		
#define CLOCK_SPEED	 11059200L //unit important Hz
#define CLOCK_CYCLES_IN_ONE_MS = CLOCK_SPEED/1000
#define CLOCK_CYCLES_IN_FOR_LOOP 22	// value from counting assembled code
#define USE_CIRCULAR_BUFFER		// comment out to use block buffering

#define VOLTAGE_RANGE 2500
#define DC_MEAN_VOLTAGE 
#define highestDisplayableNumberHz 9999;

sbit Slave_select = 0xB6;  // initialize Slave Select bit to ENABLE from the bit on P3.6

#endif