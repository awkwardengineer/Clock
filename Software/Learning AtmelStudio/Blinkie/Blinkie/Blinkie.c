/*
 * Blinkie.c
 *
 * Created: 4/15/2014 10:45:50 PM
 *  Author: sfeller
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 1000000 //default
#define TIMER_PRESCALER 1024
#define TIMER_TICKS_PER_INTERRUPT 25

// conversion = 1Mhz / TIMER_PRESCALER / TIMER_TICKS_PER_INTERRUPT = 39.0625 timer interrupts/sec


#define MINS_CONVERSION 2344  //  39.0625 timer_ints/sec * 60 secs/min = 2343.75 timer_ints/minute
#define SECONDS_CONVERSION 39 


volatile unsigned long timer_interrupts = 0;
volatile uint8_t cal_hours = 240;
volatile uint8_t cal_minutes = 240;

volatile int rotation_accumulator = 0;



void (*mode_pointer)(void);  //declares the pointer
void mode_test(void);		//the compiler is pretty dumb and needs to know about the other functions for the function pointer to work
void mode_cal_hours(void);
void mode_cal_minutes(void);
void mode_warble(void);
void mode_time(void);

#define test_low 0
#define test_high 49

#define cal_hours_low 50
#define cal_hours_high 99

#define cal_minutes_low 100
#define cal_minutes_high 149

#define warble_low 150
#define warble_high 199

#define time_low 200
#define time_high 255

void mode_test(){
	
	rotation_accumulator = 0; // dump the contents of the quadrature input accumulator, it is not used in this mode
		
	if (ADCH > test_high){
		mode_pointer = &mode_cal_hours;
		mode_cal_hours();
	}
	else{
		OCR0A +=1 ; // cycles the registers
		OCR0B +=1 ;
	}
	
	return;
}

void mode_cal_hours(){
		
	if (ADCH < cal_hours_low){
		mode_pointer = &mode_test;
		mode_test();
	}
	else if (ADCH > cal_hours_high){
		mode_pointer = &mode_cal_minutes;
		mode_cal_minutes();
	}
	else{
		OCR0A = cal_hours;
		OCR0B = 0;
		
		cal_hours += rotation_accumulator;
		
		rotation_accumulator = 0;
	}
	
	return;
}

void mode_cal_minutes(){
		
	if (ADCH < cal_minutes_low){
		mode_pointer = &mode_cal_hours;
		mode_cal_hours();
	}
	else if (ADCH > cal_minutes_high){
		mode_pointer = &mode_warble;
		mode_warble();
	}
	else{
		OCR0A = 0;
		OCR0B = cal_minutes;
		
		cal_minutes += rotation_accumulator;
		
		rotation_accumulator = 0;
	}
	
	return;
}

void mode_warble(){
	if (ADCH < warble_low){
		mode_pointer = &mode_cal_minutes;
		mode_cal_minutes();
	}
	else if (ADCH > warble_high){
		mode_pointer = &mode_time;
		mode_time();
	}
	else{
		rotation_accumulator = 0;
	}
	
	return;
}

void mode_time(){
	if (ADCH < time_low){
		mode_pointer = &mode_warble;
		mode_warble();
	}
	else{
		
		unsigned long hours;
		unsigned long minutes;
		
		//hours = (timer_interrupts * cal_hours) / HOURS_CONVERSION ; //order of operations matters to avoid rounding errors
		//OCR0A = (uint8_t)(hours );  //get the low byte of hours
		
		//hours = ( ((timer_interrupts / 39) % 60)  * cal_hours) / 60 ;
		//OCR0A = (uint8_t)(hours );  //get the low byte of hours
		
		
		hours = (((timer_interrupts / MINS_CONVERSION) % 720) * cal_hours) / 720 ; //order of operations matters to avoid rounding errors
		OCR0A = (uint8_t)(hours );  //get the low byte of hours
		
		minutes = (((timer_interrupts / SECONDS_CONVERSION) % 3600) * cal_minutes) / 3600;
		OCR0B = (uint8_t)(minutes); //get the low byte of minutes
		
		timer_interrupts += rotation_accumulator * 2000; //scales the rotation accumulator so it changes time faster
		
		rotation_accumulator = 0;
	}

	
	return;
}


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
	//  when down-counting. Kinda like inverted mode. (See page 81/82). 
	TCCR0A |= (1<<COM0A1);  
	TCCR0A |= (1<<COM0B1);   // I find the mishmash of output compare for Timer0/ChannelB being on register A slightly confusing
	
	// SET CLOCK SOURCE
	// Speed = 1Mhz / 256 bits / 2 (count up and down in phase correct mode) = 1953Hz
	// The smallest prescaler is 8, which would give a PWM freq of 244Hz
	// corresponds to Clock Select bits CS02:00 of 0b010
	
	TCCR0B |= (1 << CS01);
	
	//  SET DATA DIRECTION REGISTERS
	//  ***********************************************************
	// the Output Compare 0A register (OC0A) is PortB2, and OC0B is on PortA7
	// set the Data Direction Register
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
	
		OCR1A = TIMER_TICKS_PER_INTERRUPT; // will trigger Timer1A every __ sec or so. 1A needs to be the larger number, as that does the actual Clear Timer on Compare
		
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
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
	
	//volatile variable was defined in main()
	timer_interrupts += 1;  //in Compare Timer and Clear Mode, the register should have the number of Timer ticks
	
	PORTB ^= (1 << PB0);  //toggles LED on PB0/pin2
	
	// starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
	ADCSRA |= (1<<ADSC);
	
	// loops while waiting for ADC to finish
	while(ADCSRA & (1<<ADSC));	
	(*mode_pointer)();  //uses a pointer to call the function for the specific mode
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

	rotation_accumulator += lookup_table[grey_code];  //changes the PWM duty cycle register
	
	(*mode_pointer)();  //uses a pointer to call the function for the specific mode
}



int main (void)
{
	mode_pointer = &mode_test; //assigns the mode_pointer initially to test mode
		
	pwm_init();
	timer_init();
	analog_init();
	pinchange_init();
	sei(); // global set enable interrupts
	
	//uint8_t a = 0;
		
	while(1)
    {
	   // crap for the debugger/simulator that won't get optimized away
	  // a = a + 1;
	   //PORTB |= a & (1 << PB0);
	   
	   
	   // code is interrupt driven. so just hang out.
    }
	
	return 1;
}

