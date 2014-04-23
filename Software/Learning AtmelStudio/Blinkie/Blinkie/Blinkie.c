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
	
		TIMSK1 = (1 << OCIE1A);  // trigger interrupt when TIMER1 reaches the TOP of A (I previously had it trigger on B as well, but turned that off)
}

ISR (TIM1_COMPA_vect){
	
	PORTB ^= (1 << PB0);  //toggles LED on PB0/pin2
	
	// starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
	ADCSRA |= (1<<ADSC);
	
	// loops while waiting for ADC to finish
	while(ADCSRA & (1<<ADSC));
	
	OCR0A = ADCH; //sets the PWM output compare (duty cycle) to high register from AtoD
}

void analog_init(){
	
	//Voltage Reference is already set to Vcc by default (pg 145)
	
	// set PORTA3 as the input to the ADC (Pin #10) by setting MUX5:0 to 0b00011
	//ADC Multiplexer Selection Register
	ADMUX |= (1<<MUX1)|(1<<MUX0);
	
	//ADC Control and Status Register A
	ADCSRA |= (1<<ADEN); // enables the ADC
	ADCSRA |= (1<<ADPS1)|(1<<ADPS0); //prescaler divides clock by 8, ADPS2:0 = 0b011
	
	//ADC Control and Status Register B
	ADCSRB |= (1 <<ADLAR); //left adjust result, so of the 10 bits, the high 8 bit register has the data I need.
	
}

void pinchange_init(){
	
	// General Interrupt Mask Register
	GIMSK |= (1<<PCIE0);  //Pin Change Interrupt Enable 0, turns on interrupts for Pin Change bits 7:0 
	
	//Pin Change Mask Register
	PCMSK0 |= (1<<PCINT2)|(1<<PCINT1);  //turns on Pin Change 1 & 2 on pins 12 and 11
}

ISR (PCINT0_vect){
	//much of this code is modified from makeatronics.blogspot.com
	//the idea was to create a clever sort of "grey code" lookup table
	//wherethe previous state is left shifted two bits and added to the most recent
	//state, creating a 4 bit grey code. the code then shows the encoder direction,
	//or 0, if it was an illegal "grey code" state.

	//the static keyword allows the table to persist within the scope of the ISR
	//from one ISR call to the next
    static int8_t lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
	static uint8_t grey_code = 0;
	
	uint8_t input_status;
	
	//the inputs were on PA1 and PA2, so we mask off the other inputs and then shift
	//the bits to the right
	input_status = (PINA & 0b00000110) >> 1;   //PINA is Port Input A register

	grey_code = grey_code << 2 ;  //move the old grey code over two bits
	grey_code = grey_code | input_status ; //concatenates the current input status onto the old grey code
	grey_code = grey_code & 0b00001111; // masks off the high bits to throw the old grey shifted over grey code away

	OCR0B = OCR0B + lookup_table[grey_code];  //changes the PWM duty cycle register
}

int main (void)
{
	
	pwm_init();
	timer_init();
	analog_init();
	pinchange_init();
	sei(); // global enables interrupts
		
	while(1)
    {
	   // code is interrupt driven. so just hang out.
    }
	
	return 1;
}