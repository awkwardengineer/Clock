/*
 * Blinkie.c
 *
 * Created: 4/15/2014 10:45:50 PM
 *  Author: sfeller
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000

// initialize PWM
void pwm_init()
{
	//  SET WAVE GENERATION MODE
	//  ***********************************************************
	//  Timer/Counter Control Register 0
	//  Setting WaveGenerationMode bits 00:02 to 0b001 puts the counter in phase correct PWM mode
	//  and sets the TOP of the counter to the full 0xFF range. (see page83 of docs)
	TCCR0A = (1<<WGM00);
		
	//  SET OUTPUT COMPARISON MODE
	//  ***********************************************************
	//  Timer/Counter Control Register 0
	//  Setting only Compare Output Mode bit A1 (COM0A1) clears the counter when up-counting on a match and sets it
	//  when down-counting. Kinda like inverted mode. (See page 81/82). I put OC0B in non-inverting mode
	//  so it will have the opposite duty cycle. 
	TCCR0A |= (1<<COM0A1);  
	TCCR0A |= (1<<COM0B1)|(1<<COM0B0);   // I find the mishmash of output compare for Timer0/ChannelB being on register A slightly confusing
	
	// SET CLOCK SOURCE
	// Speed = 1Mhz / 256 bits / 2 (count up and down in phase correct mode) = 1953Hz
	// The smallest prescaler is 8, which would give a PWM freq of 244
	// corresponds to Clock Select bits CS02:00 of 0b010
	
	TCCR0B |= (1 << CS01);
	
	//  SET DATA DIRECTION REGISTERS
	//  ***********************************************************
	// the Output Compare 0A register (OC0A) is PortB2, and OC0B is on PortA7
	// set the Data Direction Register (either DDRA or DDRB) 
	DDRB |= (1<<PB2);
	DDRA |= (1<<PA7);
}

void timer_init()
{
	// Connect LED to PB0 on pin 2
	DDRB |= (1<<PB0);
	
	//  SET WAVE GENERATION MODE
	//  ***********************************************************
	// Timer/Conter Control Register 0A and 0B
	// Wave Generation Mode bit 13:10 need to be set to 0b0100
	// to put the timer in Clear Timer on Compare mode.
	// Sets the TOP of the register to OCR1A
	
		OCR1A = 25; // will trigger Timer1A every __ sec or so. 1A needs to be the larger number, as that does the actual Clear Timer on Compare
		OCR1B = 25; // see notes below about period. This will trigger Timer1B every __ sec or so
		
		TCCR1B = (1<<WGM12);  //only need to set WGM12 to achieve 0b0100
		
	
	// SET OUTPUT COMPARISON MODE 
	// Compare Output Mode doesn't need to be set, the output pin is disconnected by default
	
	// SET CLOCK SOURCE
	//  ***********************************************************
	//  Clock source bits CS10:12 are set to 0b101 to get a prescaler of 1024.
	//  NOTE: The default factory setting for the clock/clock prescaler is 1 Mhz
	//  1Mhz / 1024 = 976 Hz
	//  976 Hz / 65536 = .014 Hz = 67s period

		
		TCCR1B |= (1 << CS12)|(1 << CS10);
		
	// ENABLE INTERRUPT
	// Output Compare A Match Interrupt Enable
	
		TIMSK1 = (1 << OCIE1A)|(1<<OCIE1B);  // trigger interrupt when TIMER1 reaches the TOP of A or B
		sei(); // enables interrupts
}

//Interrupt Service Routine for TIMER1 Compare Match A
// NOTE: during the ISR, the compiler adds code that masks other interrupts (so I don't need to mask them manually)
ISR (TIM1_COMPB_vect){
	
	OCR0A += 1; // increase the duty cycle of the PWM on TIMER0 by 1. this should overflow and cycle through.
	OCR0B += 1;
	
}

ISR (TIM1_COMPA_vect){
	
	PORTB ^= (1 << PB0);  //toggles LED on PB0/pin2
}

int main (void)
{
	
	pwm_init();
	timer_init();
		
	while(1)
    {
	   // code is interrupt driven. so just hang out.
    }
	
	return 1;
}