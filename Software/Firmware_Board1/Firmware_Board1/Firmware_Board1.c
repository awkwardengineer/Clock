/*
 * Firmware_Board1.c
 *
 * Created: 4/15/2014 10:45:50 PM
 *  Author: sfeller
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 1000000 //default
#define TIMER_PRESCALER 1
#define TIMER_TICKS_PER_INTERRUPT 32000

// conversion = 32768hz / TIMER_PRESCALER / TIMER_TICKS_PER_INTERRUPT = 1 timer interrupts/sec


#define MINS_CONVERSION 60  //  1 timer_ints/sec * 60 secs/min = 60 timer_ints/minute
#define SECONDS_CONVERSION 1 


volatile unsigned long timer_interrupts = 18000;
volatile uint8_t cal_hours = 240;
volatile uint8_t cal_minutes = 240;

volatile int rotation_accumulator = 0;



void (*mode_pointer)(void);  //declares the pointer
void mode_test(void);		//the compiler is pretty dumb and needs to know about the other functions for the function pointer to work
void mode_cal_hours(void);
void mode_cal_minutes(void);
void mode_warble(void);
void mode_time(void);
void tell_time(void);

#define test_low 0
#define test_high 106

#define cal_hours_low 107
#define cal_hours_high 128

#define cal_minutes_low 129
#define cal_minutes_high 150

#define warble_low 151
#define warble_high 172

#define time_low 173
#define time_high 255

#define MODE_SELECT_POWER PA0
#define MODE_SELECT_INPUT PA1
#define QUAD_INT1 PA2
#define QUAD_INT2 PA3
#define MINUTES_OUT PA7
#define HOURS_OUT PB2


void mode_test(){
	
	rotation_accumulator = 0; // dump the contents of the quadrature input accumulator, it is not used in this mode
		
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
	
	static uint8_t prime_table[] = {53, 67, 197, 43, 157, 97, 83, 179};
	
	// {197, 179, 157, 97, 83, 59, 29, 17 }
	
	static int8_t max_warble_level = 1;
	static uint8_t warble_count = 0;
	static int8_t warbling = 0;
	static uint8_t warble_randomizer = 0;
		
	
	if (ADCH < warble_low){
		rotation_accumulator = 0;
		mode_pointer = &mode_cal_minutes;
		mode_cal_minutes();
	}
	else if (ADCH > warble_high){
		rotation_accumulator = 0;
		mode_pointer = &mode_time;
		mode_time();
	}
	else{
		if (warbling==0){
			tell_time();
		}
		else{
			warble_randomizer++;			//rotates the warble randomizer
		}
		
		
		if (timer_interrupts % prime_table[warble_randomizer & 0b00000111] == 0){
			warbling = 1;
			warble_count = max_warble_level;
			OCR0A = 0;
			
		}
		if (timer_interrupts % prime_table[(warble_randomizer + 1) & 0b00000111] == 0){
			warbling = 1;
			warble_count = max_warble_level;
			OCR0A = 255;
			
		}
		if (timer_interrupts % prime_table[(warble_randomizer + 2) & 0b00000111] == 0){
			warbling = 1;
			warble_count = max_warble_level;
			OCR0B = 0;
			
		}
		if (timer_interrupts % prime_table[(warble_randomizer + 3) & 0b00000111] == 0){
			warbling = 1;
			warble_count = max_warble_level;
			OCR0B = 255;
			
			
		}
		
		
		if (warble_count > 0){
			warble_count--;	
		}
		else{
			warbling = 0;
		}
		
		
 		
		if (rotation_accumulator > 5 || rotation_accumulator < -5){
			max_warble_level += rotation_accumulator / 5;
			
			if (max_warble_level < 0){
				max_warble_level = 0;
			}
			else if (max_warble_level > 30){
				max_warble_level = 30;
			}
			
			rotation_accumulator = 0; // only clears the accumulator if had a large enough value
		}
		
	}
	
	return;
}

void mode_time(){
	if (ADCH < time_low){
		mode_pointer = &mode_warble;
		mode_warble();
	}
	else{
		
		timer_interrupts += rotation_accumulator * 32; //scales the rotation accumulator so it changes time faster
		rotation_accumulator = 0;
		
		tell_time();	
		
		
		
		
	}

	
	return;
}

void tell_time(){
	unsigned long hours;
	unsigned long minutes;
	
	hours = (((timer_interrupts / MINS_CONVERSION) % 720) * cal_hours) / 720 ; //order of operations matters to avoid rounding errors
	OCR0B = (uint8_t)(hours );  //get the low byte of hours
	
	minutes = (((timer_interrupts / SECONDS_CONVERSION) % 3600) * cal_minutes) / 3600;
	OCR0A = (uint8_t)(minutes); //get the low byte of minutes
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
	// No Clock prescaler will be necessary for the PWM
	// System clock is at 32khz
	// 32khz / 256bits*2 for counting up AAAND down = 64Hz
	 	
	TCCR0B |= (1 << CS00); // set clock source to 0b001 for on with no prescaler
	
	//  SET DATA DIRECTION REGISTERS
	//  ***********************************************************
	// the Output Compare 0A register (OC0A) is PortB2, and OC0B is on PortA7
	// set the Data Direction Register
	DDRB |= (1<<HOURS_OUT);
	DDRA |= (1<<MINUTES_OUT);
}

void timer_init()
{
	// Connect LED to PA0 on pin 13
	// DDRA |= (1<<PA0);				//on the breadboard, there was an LED on this pin, but it has been removed on the PCB
	
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
		
		
		TCCR1B |= (1 << CS10);  //Set Clock Source Bits CS12:10 to 0b001 to turn timer on with no prescaler 
		
	// ENABLE INTERRUPT
	// Output Compare A Match Interrupt Enable
	
		TIMSK1 = (1 << OCIE1A);  // trigger interrupt when TIMER1 reaches the TOP of A (I previously had it trigger on B as well, but turned that off)
}

ISR (TIM1_COMPA_vect){
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
	
	//volatile variable was defined in main()
	timer_interrupts += 1;  //in Compare Timer and Clear Mode, the register should have the number of Timer ticks
	
	//PORTA ^= (1 << PA0);  //toggles LED on PBA/pin13   //on the breadboard, there was an LED on this pin, but it has been removed on the PCB
	
}

void analog_init(){
	
	//Voltage Reference is already set to Vcc by default (pg 145)
	
	// set PORTA1 (MODE_SELECT) as the input to the ADC (Pin #12) by setting MUX5:0 to 0b00001
	//ADC Multiplexer Selection Register
	ADMUX |= (1<<MUX0);
	
	//ADC Control and Status Register A
	ADCSRA |= (1<<ADEN); // enables the ADC
	ADCSRA |= (1<<ADPS0); //prescaler divides clock by 1, ADPS2:0 = 0b001
	
	//ADC Control and Status Register B
	ADCSRB |= (1 <<ADLAR); //left adjust result, so of the 10 bits, the high 8 bit register has the data I need.
	
	DDRA |= (1<<MODE_SELECT_POWER); //used to select the potentiometer as an output
	PORTA &= ~(1<<MODE_SELECT_POWER); //turn power to potentiometer off
}

void pinchange_init(){
	
	// General Interrupt Mask Register
	GIMSK |= (1<<PCIE0);  //Pin Change Interrupt Enable 0, turns on interrupts for Pin Change bits 7:0 
	
	//Pin Change Mask Register
	PCMSK0 |= (1<<QUAD_INT1)|(1<<QUAD_INT2);  //turns on Pin Change interupts on QUAD_INT1 and QUAD_INT2
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
	
	//the inputs were on QUAD_INT1 and QUAD_INT2  (PA2 and 3), so we mask off the other inputs and then shift
	//the bits to the right
	input_status = (PINA & 0b00001100) >> 2;   //PINA is Port Input A register

	grey_code = grey_code << 2 ;  //move the old grey code over two bits
	grey_code = grey_code | input_status ; //concatenates the current input status onto the old grey code
	grey_code = grey_code & 0b00001111; // masks off the high bits to throw the old grey shifted over grey code away

	rotation_accumulator += lookup_table[grey_code];  
	
	//if (mode_pointer != &mode_warble)
	//{
	//	(*mode_pointer)();  //uses a pointer to call the function for the specific mode
	//}
	
}

void power_register_init(){
	
	//MCUCR: MCU Control Register
	
	MCUCR |= (1<<BODS); //disable brown out detector
	MCUCR |= (1<<SE); //sleep enable
	
	MCUCR |= (0<<SM1)|(0<<SM0); //sleepmode select 0b00; will enter IDLE mode with CPU shut down
	//MCUCR |= (1<<SM1)|(1<<SM0);//sleepmode select 0b11, will enter STANDBY mode with CPU shutdown and crystal still on
	
	//PRR: Power Reduction Register
	
	PRR |= (1<<PRUSI); // shuts down the USI clock
	
}

int main (void)
{
	mode_pointer = &mode_time; //assigns the mode_pointer initially to time mode
		
	pwm_init();
	timer_init();
	analog_init();
	pinchange_init();
	power_register_init();
	sei(); // global set enable interrupts
		
	while(1)
    {
		PRR &= ~(1<<PRADC); //turns on the ADC comparator
		PORTA |= (1<<MODE_SELECT_POWER); //turn on to power pot
		ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
		while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
		PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
		PRR |= (1<<PRADC); //shuts down the ADC and comparator
		
		
		
		(*mode_pointer)();  //uses a pointer to call the function for the specific mode
		
		sleep_cpu();
		//just hang out and wait for interrupts
		
    }
	
	return 1;
}

