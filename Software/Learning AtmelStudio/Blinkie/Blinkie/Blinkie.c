/*
 * Blinkie.c
 *
 * Created: 4/15/2014 10:45:50 PM
 *  Author: sfeller
 */ 


#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000

// initialize PWM
void pwm_init()
{
	//  SET WAVE GENERATION MODE
	//  ***********************************************************
	//  Timer/Counter Control Register 0
	//  Setting WaveGenerationMode bit 0 high puts the counter in phase correct PWM mode
	//  other and sets the TOP of the counter to the full 0xFF range. (see page83 of docs)
	TCCR0A = (1<<WGM00);
	TCCR0B = (1<<WGM00);
	
	//  SET OUTPUT COMPARISON MODE
	//  ***********************************************************
	//  Timer/Counter Control Register 0
	//  Setting only Compare Output Mode bit A1 (COM0A1) clears the counter when up-counting on a match and sets it
	//  when down-counting. Kinda like inverted mode. (See page 81/82). I put OC0B in non-inverting mode
	//  so it will have the opposite duty cycle. 
	TCCR0A |= (1<<COM0A1);
	TCCR0A |= (1<<COM0B1)|(1<<COM0B0);   // I find the mishmash of output compare for Timer0/ChannelB being on register A slightly confusing
	
	//  SET DATA DIRECTION REGISTERS
	//  ***********************************************************
	// the Output Compare 0A register (OC0A) is PortB2, and OC0B is on PortA7
	// set the Data Direction Register (either DDRA or DDRB) 
	DDRB = (1<<PB2);
	DDRA = (1<<PA7);
}

void timer_init()
{
	
	
	
}

int main (void)
{
    uint8_t brightness;
    
    pwm_init();
		
	// loop
    while(1)
    {
	    // increasing brightness
	    for (brightness = 0; brightness < 255; ++brightness)
	    {
		    // set the brightness as duty cycle
		    OCR0A = brightness;
			OCR0B = brightness;
		    
		    // delay so as to make the user "see" the change in brightness
		    _delay_ms(10);
	    }
	    
	    // decreasing brightness
	    for (brightness = 255; brightness > 0; --brightness)
	    {
		    // set the brightness as duty cycle
		    OCR0A = brightness;
			OCR0B = brightness;
		    
		    // delay so as to make the user "see" the change in brightness
		    _delay_ms(10);
	    }
	    
	    // repeat this forever
    }
	
	return 1;
}