; Demo Program - using timer interrupts.
; Written for ADuC841 evaluation board, with UCD extras.
; Brian Mulkeen, September 2016

; Include the ADuC841 SFR definitions
$NOMOD51
$INCLUDE (MOD841)

LED     EQU     P3.4      ; P3.4 is red LED on eval board
	
CSEG			  ; working in code segment - program memory

		ORG		0000h		; set origin at start of code segment
		JMP		SETUP		; jump to main program
		
		ORG		002Bh		; timer 0 interrupt address
		JMP		T2ISR		; jump to interrupt handler

		ORG		0060h		; set origin above interrupt addresses	

PRELOAD:DW 10240,54476,57233,60006,62771,63692,64153,64430  		 ; Preload values to ring the buzzer at given frequencies
																	 ; ! Values given as groups of 2 bytes
;____________________________________________________________________; MAIN PROGRAM
SETUP:
; ------ Setup part - happens once only ----------------------------

; Timer2
		MOV		T2CON, #04h	; timer 2 is set up to run (TR2=1) as a timer (CNT2=0) with automatic reload (CAP2=0)
							; the rest of the bits of T2CON is either set by the hardware, or the none of our interest (serial port)
		MOV		IE, #0A0h 	; enable the timer 2 interruptions, meaning EA and ET2 are set to 1

; Preload values
		MOV DPTR, #PRELOAD	; setup the data pointer to access program memory by preloading the adress of the beginning of the table
	
; Let's start 	
		JMP 	LOOP	

;____________________________________________________________________
                                                        ; SUBROUTINES
LOOP:					
			CPL   LED     	; change state of red LED		
; Grab the state of switch state to enable/disable the interruptions on the timer 2
			MOV   C,P2.4	; storing the bit value into the carry	
			MOV   ET2,C  	; off sw => on interruption
						
			MOV A, #7h		; define the AND mask
			ANL A, P2		; logical and operation between the mask and the switch values of interest
			RL A 			; multiplication by 2 because each preload value takes 2 bytes
			MOV R1,A		
			
; preload the timer
			MOVC A,@A+DPTR	; Grab the MSB preload value from the program memory into the accumulator
			MOV RCAP2H,A	; Give it to the timer2
			
			MOV A,R1		; Get the adress of the preload value considered
			INC A			; Get the adress of the LSB preload value
			
			MOVC A,@A+DPTR  ; Get the LSB preload value 
			MOV RCAP2L,A	; Give it to the timer2
			
			MOV	  A,  #020	; set delay length for 200 ms
			CALL  DELAY   	; call software delay routine
			JMP   LOOP   	; repeat indefinately
			

DELAY:    ; delay for time A x 10 ms.  A is not changed. 
			MOV	  R5, A			; set number of repetitions for outer loop
DLY2:   	MOV   R7, #144		; middle loop repeats 144 times         
DLY1:   	MOV   R6, #255   	; inner loop repeats 255 times      
        	DJNZ  R6, $			; inner loop 255 x 3 cycles = 765 cycles            
        	DJNZ  R7, DLY1		; + 5 to reload, x 144 = 110880 cycles
			DJNZ  R5, DLY2		; + 5 to reload = 110885 cycles = 10.0264 ms
        	RET					; return from subroutine

;____________________________________________________________________


; ------ Interrupt service routine ---------------------------------	
T2ISR:		; timer 2 overflow interrupt service routine
			CPL	  P3.6			; flip bit to generate output i.e. square-wave 	
			CLR   TF2			; clear the interrupt flag because the hardware is not doing it automatically
			RETI				; return from interrupt
; ------------------------------------------------------------------		
END