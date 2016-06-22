/*
 * voltage_scan_test.c
 *
 * Created: 4/25/2016 8:35:44 PM
 * Author : awkwardengineer
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "..\shared\alarm_config.c"
#include "..\shared\alarm_helpers.c"

int main(void)
{
  
	init_all();
  
	 ADMUX &= ~(0b00011111); //set the MUX bits to 00000, this selects ADC0 (MODE POWER pin as the AtoD source)
  
  	//turn pot power back off
  	PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
  				
  	//make MODE POWER pin  an input
  	DDRA &= ~(1<<MODE_SELECT_POWER); //makes pot power pin an input
  
  
    /* Replace with your application code */
    while (1) 
    {
				
				//read residual capacitive charge on input
				ADCSRA |= (1<<ADSC);
				while(ADCSRA & (1<<ADSC));
				
				OCR0A = ADCH;
				OCR0B = ADCH;
    }
}

