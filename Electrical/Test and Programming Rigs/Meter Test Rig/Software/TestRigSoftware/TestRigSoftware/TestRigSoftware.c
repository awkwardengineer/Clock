/*
 * Firmware_Board1.c
 *
 * Created: 4/15/2014 10:45:50 PM
 *  Author: sfeller
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <stdlib.h>

#define F_CPU 1000000 //default internal clock speed
#define TIMER_PRESCALER 1
#define TIMER_TICKS_PER_INTERRUPT 32767
#define TWELVE_HOURS 43200 // 12hours * 60 min * 60 sec = 43200
#define THIRTY_SIX_HOURS 129600 // 36 hours * 60 min * 60 sec - 129000 timer ticks

// conversion = 32768hz / TIMER_PRESCALER / TIMER_TICKS_PER_INTERRUPT = 1 timer interrupts/sec


#define MINS_CONVERSION 60  //  1 timer_ints/sec * 60 secs/min = 60 timer_ints/minute
#define SECONDS_CONVERSION 1 


volatile unsigned long timer_interrupts = 18000;   //time starts at 5:00
volatile uint8_t cal_hours = 240;
volatile uint8_t cal_minutes = 240;

volatile int rotation_accumulator = 0;
volatile bool time_to_check_knob;

volatile uint8_t last_reading = 0;
volatile uint8_t scans_remaining = 0;

volatile bool goingup = false;


void mode_test(void);		//the compiler is pretty dumb and needs to know about the other functions for the function pointer to work
void mode_cal_hours(void);
void mode_cal_minutes(void);
void mode_warble(void);
void mode_time(void);
void tell_time(void);
void (*mode_pointer)(void) = &mode_time; //assigns the mode_pointer initially to time mode;  //declares the pointer

#define test_low 0
#define test_high 13

#define time_low 14
#define time_high 64

#define warble_low 65
#define warble_high 128

#define cal_hours_low 129
#define cal_hours_high 215

#define cal_minutes_low 216
#define cal_minutes_high 255

#define MODE_SELECT_POWER PA0
#define MODE_SELECT_INPUT PA1
#define QUAD_INT1 PA2
#define QUAD_INT2 PA3
#define MINUTES_OUT PA7
#define HOURS_OUT PB2

void mode_test(){
	
	if (!goingup)
	{
		OCR0A = 0;
		/*if (OCR0A == 255){
			OCR0A = 210;
		}
		else if (OCR0A >= 60){

			OCR0A -=60 ; // cycles the registers
			
		}
		else
		{
			OCR0A = 0;
			//OCR1A = 4000;
			goingup = true;
		}
		OCR0B = OCR0A	;*/
		OCR0B = OCR0A	;
		goingup=true;
		return;
	}
	//rotation_accumulator = 0; // dump the contents of the quadrature input accumulator, it is not used in this mode
	if (1){
		if (OCR0A <= 215){

			OCR0A +=40 ; // cycles the registers
			
		}
		
		if (OCR0A > 225)
		{
			OCR0A = 255;
			OCR1A = 4000;
			goingup = false;
		}
	}
	
	
		
	OCR0B = OCR0A	;
	
	
	return;
}

/*mode_test2(){
	
	
	timer_interrupts = 60
	
	
}*/

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

// initialize PWM
void pwm_init()
{
	// Timer/Counter Control  Register 0A
	//  SET WAVE GENERATION MODE
	//  ***********************************************************
	//  Setting WaveGenerationMode bits 00:02 to 0b001 puts the counter in phase correct PWM mode
	//  and sets the TOP of the counter to the full 0xFF range. (see page83 of docs)
	
	//  SET OUTPUT COMPARISON MODE
	//  ***********************************************************
	//  Setting only Compare Output Mode bit A1 (COM0A1) clears the counter when up-counting on a match and sets it
	//  when down-counting. Kinda like inverted mode. (See page 81/82).
	
	TCCR0A = (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1); ;
			
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
	// Wave Generation Mode bit 13:10 need to be set to 0b1111
	// to put the timer in Fast PWM mode.
	// Sets the TOP of the register to OCR1A
	
	
		OCR1A = TIMER_TICKS_PER_INTERRUPT; // will trigger Timer1A every __ sec or so. 1A needs to be the larger number, as that does the actual Clear Timer on Compare
		OCR1B = TIMER_TICKS_PER_INTERRUPT + 1;
		//OCR1B = TIMER_TICKS_PER_INTERRUPT / 2;
		
		TCCR1A |= (1<<WGM11)|(1<<WGM10);					//set WGM to 0b1111 for fastpwm with OCR1A as the top
		TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1 << CS10);;		//set WGM to 0b1111 for fastpwm with OCR1A as the top
															// SET CLOCK SOURCE  
															//CS10 Set Clock Source Bits CS12:10 to 0b001 to turn timer on with no prescaler

	// SET OUTPUT COMPARISON MODE 
	// Compare Output Mode doesn't need to be set, the output pin is disconnected by default
	
		
	// ENABLE INTERRUPT
	// Output Compare A Match Interrupt Enable
	
		TIMSK1 |= (1 << TOIE1)|(1 << OCIE1B);;  // TOIE1 trigger interrupt when TIMER1 reaches the TOP of A
												 // OCIE1B  trigger interrupt at halfway to top of A as well.
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

void analog_init(){
	
	//Voltage Reference is already set to Vcc by default (pg 145)
	
	// set PORTA1 (MODE_SELECT) as the input to the ADC (Pin #12) by setting MUX5:0 to 0b00001
	//ADC Multiplexer Selection Register
	ADMUX |= (1<<MUX0);
	
	//ADC Control and Status Register A
	ADCSRA |= (1<<ADEN)|(1<<ADPS0);; // ADEN enables the ADC
									 //ADPS0 prescaler divides clock by 1, ADPS2:0 = 0b001
	
	//ADC Control and Status Register B
	ADCSRB |= (1 <<ADLAR); //left adjust result, so of the 10 bits, the high 8 bit register has the data I need.
	
	DDRA |= (1<<MODE_SELECT_POWER); //used to select the potentiometer as an output
	//PORTA &= ~(1<<MODE_SELECT_POWER); //turn power to potentiometer off
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

void power_register_init(){
	
	//MCUCR: MCU Control Register
	
	MCUCR |= (1<<BODS)|(1<<SE)|(0<<SM1)|(0<<SM0);; 
	
								//BODS disable brown out detector
								//SE sleep enable
								//SM1, SM0  - sleepmode select 0b00; will enter IDLE mode with CPU shut down
	
	//PRR: Power Reduction Register
	
	PRR |= (1<<PRUSI); // shuts down the USI clock
	
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
		
		
		PRR &= ~(1<<PRADC); //turns on the ADC comparator
		PORTA |= (1<<MODE_SELECT_POWER); //turn on to power pot
		ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
		while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
		PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
		PRR |= (1<<PRADC); //shuts down the ADC and comparator
		
		//if (ADCH<64){
			OCR1A = 32000;
		//*}
		/*
		else if (ADCH<128){
			OCR1A = 28000;
		}
		else if (ADCH<192){
			OCR1A = 26000;
		}
		else{
			OCR1A = 24000;
		}*/
		
		/*if(time_to_check_knob){			
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
	//				scans_remaining--;
				}
			}
			else{
	//			scans_remaining = 10;
				OCR1B = TIMER_TICKS_PER_INTERRUPT / 4;						 // if there WAS change, reset the scan count at higher frequency interrupts
			}
			
			
			last_reading = ADCH;  //saves the reading
			time_to_check_knob = false;
		}*/
		
		rotation_accumulator = 0;
		sleep_cpu();     	//just hang out and wait for interrupts
		mode_test();
		//(*mode_pointer)();  //uses a pointer to call the function for the specific mode	
		
		
    }
	
}

