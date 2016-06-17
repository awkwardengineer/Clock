/*
 * alarm_config.c
 *
 * Created: 3/12/2016 8:36:20 PM
 *  Author: sfeller
 */ 

#ifndef _ALARM_CONFIG_C_
#define _ALARM_CONFIG_C_



//#include "alarm_config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

 
 /**********************************
 * pinouts for ATtiny24/44/84 family
 ***********************************/
 
#define MODE_SELECT_POWER PA0
#define MODE_SELECT_INPUT PA1
#define QUAD_INT1 PA2
#define QUAD_INT2 PA3
#define MINUTES_OUT PA7
#define HOURS_OUT PB2
 
#define ALARM_LIGHT PA4
#define ALARM_BUZZER PA5
#define PM_LIGHT PA6

#define HOURS_PWM OCR0B
#define MINS_PWM OCR0A
  
 
 /**********************************
 * time related constants
 ***********************************/
 
#define F_CPU 32768 //default internal clock speed, actual speed will be changed to 32.768 clock crystal
#define TIMER_PRESCALER 1

//#define TIMER_TICKS_PER_INTERRUPT 32767
#define TIMER_TICKS_PER_INTERRUPT 8191  //4Hz interrupt rate
//#define TIMER_TICKS_PER_INTERRUPT 4095
#define TICKS_4HZ 8191
#define TICKS_2HZ 16383
#define TICKS_1HZ 32767

#define TWELVE_HOURS 43200 // 12hours * 60 min * 60 sec = 43200
#define TWENTY_FOUR_HOURS 86400
#define THIRTY_SIX_HOURS 129600 // 36 hours * 60 min * 60 sec - 129000 timer ticks
#define AMPM_LINE 86400 // 24 hrs * 60 min * 60 sec
#define FIVE_OCLOCK_SOMEWHERE 61200 //5pm in seconds
#define NINE_AM 118800// 9*60min*60sec + 86400 
//12 - 24 hrs is am
//24 - 36 hrs is pm


 // conversion = 32768hz / TIMER_PRESCALER / TIMER_TICKS_PER_INTERRUPT = 1 timer interrupts/sec

#define MINS_CONVERSION 60  //  1 timer_ints/sec * 60 secs/min = 60 timer_ints/minute
#define SECONDS_CONVERSION 1
 
#define INITIAL_HOURS_CAL 240
#define INITIAL_MINUTES_CAL 240
 
 
 /**********************************
 * constants for knob "mode" positions
 ***********************************/

#define CAP_SENSE_THRESHOLD 20
#define SWITCH_THRESHOLD 64
#define USB_POWER_THRESHOLD 128
/*
**************************
*the following were the original knob position settings from the plain
*AWK-105 clock. they are here for reference, but will need to be updated.
**************************
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
*/

#define POS_test 10
#define POS_minutes 45
#define POS_hours 97
#define POS_warble 173
#define POS_time 225
#define POS_alarm 255



#define CLOCK_POS_test 10
#define CLOCK_POS_minutes 65
#define CLOCK_POS_hours 129
#define CLOCK_POS_warble 216
#define CLOCK_POS_time 255







void indicator_init();
void pwm_init(void);
void timer_init(void);
void analog_init(void);
void pinchange_init(void);
void power_register_init(void);
void init_all(void);


void init_all(){
	indicator_init();
	pwm_init();
	timer_init();
	analog_init();
	pinchange_init();
	power_register_init();
	sei();
}

void indicator_init(){
	PORTA &= ~ (1 << ALARM_BUZZER)|(1 << ALARM_LIGHT)|(1 <<PM_LIGHT);	//disables pullup / turn outputs to zero when the DDRA is flipped
	DDRA |= (1 << ALARM_BUZZER)|(1 << ALARM_LIGHT)|(1 <<PM_LIGHT);		//switch pins from inputs to outputs
	
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

void power_register_init(){
	
	//MCUCR: MCU Control Register
	
	MCUCR |= (1<<BODS)|(1<<SE)|(0<<SM1)|(0<<SM0);;
	
	//BODS disable brown out detector
	//SE sleep enable
	//SM1, SM0  - sleepmode select 0b00; will enter IDLE mode with CPU shut down
	
	//PRR: Power Reduction Register
	
	PRR |= (1<<PRUSI); // shuts down the USI clock
	
}

#endif