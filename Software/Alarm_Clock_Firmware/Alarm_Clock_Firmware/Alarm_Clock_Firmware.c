/*
 * Alarm_Clock_Firmware.c
 *
 * Created: 9/1/2015 8:36:20 PM
 *  Author: sfeller
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <stdlib.h>

#include "..\shared\alarm_config.h"


volatile unsigned long timer_interrupts = 18000;   //time starts at 5:00
volatile uint8_t cal_hours = 240;
volatile uint8_t cal_minutes = 240;

volatile int rotation_accumulator = 0;
volatile bool time_to_check_knob;

volatile uint8_t last_reading = 0;
volatile uint8_t scans_remaining = 0;


void mode_test(void);		//the compiler is pretty dumb and needs to know about the other functions for the function pointer to work
void mode_cal_hours(void);
void mode_cal_minutes(void);
void mode_warble(void);
void mode_time(void);
void tell_time(void);
void (*mode_pointer)(void) = &mode_time; //assigns the mode_pointer initially to time mode;  //declares the pointer



void mode_test(){
	
	//rotation_accumulator = 0; // dump the contents of the quadrature input accumulator, it is not used in this mode
	
	if (ADCH > test_high){
		mode_pointer = &mode_cal_hours;
		mode_cal_hours();
	}
	else{
		OCR0A +=10 ; // cycles the registers
		OCR0B +=10 ;
	}
	
	return;
}

void mode_time(){
	if (ADCH < time_low){
		mode_pointer = &mode_test;
		mode_test();
	}
	else if(ADCH > time_high){
		mode_pointer = &mode_warble;
		mode_warble();
	}
	else{
		
		
		
		if(scans_remaining>7){
			OCR0B=cal_hours;
			OCR0A=cal_minutes;
			return;
		}
		if(scans_remaining==7)
		{
			OCR0B=0;
			OCR0A=0;
			return;
		}
		
		tell_time();
		
		timer_interrupts += 60*rotation_accumulator ;
		
	}

	return;
}

void mode_warble(){
	
	static uint8_t warble_count = 0;
	static bool isOdd = true;
	static bool minFlag = false;
	static bool hourFlag = false;
	
	
	if (ADCH < warble_low){
		OCR1B = (unsigned long)TIMER_TICKS_PER_INTERRUPT + 1; //sets OCR1B	to a point that won't be reached on exiting mode
		
		mode_pointer = &mode_time;
		mode_time();
	}
	else if (ADCH > warble_high){
		OCR1B = (unsigned long)TIMER_TICKS_PER_INTERRUPT + 1; //sets OCR1B	to a point that won't be reached on exiting mode
		
		mode_pointer = &mode_cal_hours;
		mode_cal_hours();
	}
	else{
		if(scans_remaining>5){  //will point both meters straight up while entering this mode.
			OCR0B=cal_hours/2;
			OCR0A=cal_minutes/2;
			return;
		}
		if(scans_remaining==5)  //then it flashes the meters, and then it tells time.
		{
			OCR0B=cal_hours;
			OCR0A=cal_minutes;
			return;
		}
		
		if (hourFlag){
			OCR0B += 127;
		}
		if (minFlag){
			OCR0A += 127;
		}
		if (!hourFlag && !minFlag){
			tell_time();
		}
		hourFlag = false;
		minFlag = false;
		
		
		if (warble_count==0){   //3,5,8 from the fibonaci sequeuence, multiplied by 2 to get 6,10,16
			if((timer_interrupts % 6) == 0){
				warble_count++;
				isOdd = !isOdd; // toggles which meter to flip
			}
			if((timer_interrupts % 10) == 0){
				warble_count++;
			}
			if((timer_interrupts % 16) == 0){
				warble_count++;
				isOdd = !isOdd;  // toggles which meter to flip
			}
			
			if (warble_count >= 2){
				
				OCR1B = 5000;
				
				minFlag = true;
				hourFlag = true;
				
			}
			else if (warble_count >=1){
				OCR1B = 5000;
				
				if (isOdd){
					minFlag = true;
				}
				else{
					hourFlag = true;
				}
			}
			
		}
		else{
			warble_count = 0;
			OCR1B = TIMER_TICKS_PER_INTERRUPT + 1;
			
		}
		
	}
	
	return;
}

void mode_cal_hours(){
	
	if (ADCH < cal_hours_low){
		mode_pointer = &mode_warble;
		mode_warble();
	}
	else if (ADCH > cal_hours_high){
		mode_pointer = &mode_cal_minutes;
		mode_cal_minutes();
	}
	else{
		OCR0B = cal_hours;
		OCR0A = 0;
		
		if (255 < (rotation_accumulator + (int)cal_hours)){
			cal_hours = 255;
		}
		else if (0 > (rotation_accumulator + (int)cal_hours)){
			cal_hours = 0;
		}
		else{
			cal_hours += rotation_accumulator;
		}
		
		
	}
	
	return;
}

void mode_cal_minutes(){
	
	if (ADCH < cal_minutes_low){
		mode_pointer = &mode_cal_hours;
		mode_cal_hours();
	}
	else{
		OCR0B = 0;
		OCR0A = cal_minutes; //by dividing by 2, pointer is calibrated on the 6.
		
		if (255 < (rotation_accumulator + (int)cal_minutes)){
			cal_minutes = 255;
		}
		else if (0 > (rotation_accumulator + (int)cal_minutes)){
			cal_minutes = 0;
		}
		else{
			cal_minutes += rotation_accumulator;
		}
		
		
	}
	
	return;
}


void tell_time(){
	unsigned long hours;
	unsigned long minutes;
	
	hours = (((timer_interrupts /(60* MINS_CONVERSION) % 12 ) * cal_hours) / 12) ; //order of operations matters to avoid rounding errors
	if (hours==0){
		hours=cal_hours;
	}
	OCR0B = (uint8_t)(hours );  //get the low byte of hours
	
	minutes = (((timer_interrupts / SECONDS_CONVERSION) % 3600) * cal_minutes) / 3600;
	OCR0A = (uint8_t)(minutes); //get the low byte of minutes
}



ISR (TIM1_OVF_vect){
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
	
	//volatile variable was defined in main()
	timer_interrupts += 1;  //in Compare Timer and Clear Mode, the register should have the number of Timer ticks
	if (timer_interrupts < TWELVE_HOURS){
		timer_interrupts += TWELVE_HOURS ; //prevents zero rollover bug
	}
	if (timer_interrupts > THIRTY_SIX_HOURS){
		timer_interrupts -= TWELVE_HOURS ;
	}
	
	time_to_check_knob = true;
	
	//PORTA ^= (1 << PA0);  //toggles LED on PBA/pin13   //on the breadboard, there was an LED on this pin, but it has been removed on the PCB
	
}

ISR (TIM1_COMPB_vect){
	//THIS ISR IS CALLED TO INCREASE THE FREQUENCY THAT THE KNOB IS READ
	
	if (scans_remaining > 0){
		time_to_check_knob = true;
	}
	
}



ISR (PCINT0_vect){
	//much of this code is modified from makeatronics.blogspot.com
	//the idea was to create a clever sort of "grey code" lookup table
	//wherethe previous state is left shifted two bits and added to the most recent
	//state, creating a 4 bit grey code. the code then shows the encoder direction,
	//or 0, if it was an illegal "grey code" state.

	//the static keyword allows the table to persist within the scope of the ISR
	//from one ISR call to the next
	//static int8_t lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; //saving the original table in a comment
	static int8_t lookup_table[] = {0,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};  //trying to limit the to one tick per four counts
	static uint8_t grey_code = 0;
	
	uint8_t input_status;
	
	//the inputs were on QUAD_INT1 and QUAD_INT2  (PA2 and 3), so we mask off the other inputs and then shift
	//the bits to the right
	input_status = (PINA & 0b00001100) >> 2;   //PINA is Port Input A register

	grey_code = grey_code << 2 ;  //move the old grey code over two bits
	grey_code = grey_code | input_status ; //concatenates the current input status onto the old grey code
	grey_code = grey_code & 0b00001111; // masks off the high bits to throw the old grey shifted over grey code away

	rotation_accumulator += lookup_table[grey_code];
	
}



int main (void)
{
	
	
	pwm_init();
	timer_init();
	analog_init();
	pinchange_init();
	power_register_init();
	sei(); // global set enable interrupts
	
	while(1)
	{
		if(time_to_check_knob){
			PRR &= ~(1<<PRADC); //turns on the ADC comparator
			PORTA |= (1<<MODE_SELECT_POWER); //turn on to power pot
			ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
			while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
//========> //PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
			PRR |= (1<<PRADC); //shuts down the ADC and comparator
			
			//the status of scans_remaining will change the interrupt behavior
			//of ISR timer1B. timer1B is used to poll the knobs more often
			//and also to trigger twitch behavior more often in warble mode
			if (abs( (int)ADCH - (int)last_reading ) < 5){   //if NO CHANGE
				if (scans_remaining==0 ){
					if (mode_pointer != &mode_warble){						  //don't fuck with OCR1B in warble mode
						OCR1B = (unsigned long)TIMER_TICKS_PER_INTERRUPT + 1; //no change, no scans left, sets OCR1B	to a point that won't be reached
					}
				}
				else{
					scans_remaining--;
				}
			}
			else{
				scans_remaining = 10;
				OCR1B = TIMER_TICKS_PER_INTERRUPT / 2;						 // if there WAS change, reset the scan count at higher frequency interrupts
			}
			
			
			last_reading = ADCH;  //saves the reading
			time_to_check_knob = false;
		}
		
		rotation_accumulator = 0;
		sleep_cpu();     	//just hang out and wait for interrupts
		(*mode_pointer)();  //uses a pointer to call the function for the specific mode
		
		
	}
	
}

