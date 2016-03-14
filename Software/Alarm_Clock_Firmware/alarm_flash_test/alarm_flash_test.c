/*
 * alarm_flash_test.c
 *
 * Created: 3/12/2016 2:55:51 PM
 * Author : awkwardengineer
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "..\shared\alarm_config.c"
#include "..\shared\alarm_helpers.c"

struct State{
	unsigned long numTicks;
	unsigned long alarmTimeInTicks;
	
	
	uint8_t cal_hours;
	uint8_t cal_minutes;
	
	bool isAlarmSet;
	bool isAlarmSounding;
	bool isUIActive;
	bool isAlarmSwitchOn;
	
};


void updateLEDs(struct State *state){
	
/*		if am/pm
			setAMLightON();
		else
			setAMLightOff();
	
		if isAlarmSounding
			don't do anything  // relinquish controll to speaker/alarm
		else
			if alarmIsSet
				setAlarmLightOn();
			else
				setAlarmLightOff(); */

}

ISR (PCINT0_vect){
}

ISR (TIM1_COMPB_vect){	
}

ISR (TIM1_OVF_vect){
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
	
	//volatile variable was defined in main()
	/*timer_interrupts += 1;  //in Compare Timer and Clear Mode, the register should have the number of Timer ticks
	if (timer_interrupts < TWELVE_HOURS){
		timer_interrupts += TWELVE_HOURS ; //prevents zero rollover bug
	}
	if (timer_interrupts > THIRTY_SIX_HOURS){
		timer_interrupts -= TWELVE_HOURS ;
	}
	
	time_to_check_knob = true;*/
	
	//PORTA ^= (1 << PA0);  //toggles LED on PBA/pin13   //on the breadboard, there was an LED on this pin, but it has been removed on the PCB
	
}

int main(void)
{


	struct State state;
	
	init_all();
		
	state.numTicks = TWELVE_HOURS;
	state.alarmTimeInTicks = THIRTY_SIX_HOURS;
	state.cal_hours = INITIAL_HOURS_CAL;
	state.cal_minutes = INITIAL_MINUTES_CAL;
	state.isAlarmSet = false;
	state.isUIActive = false;
	state.isAlarmSwitchOn = false;
	
	state.isAlarmSounding = true;  //for my test, the alarm will automagically sound
	
	
    while (1) 
    {
		
		setAlarmLightOn();
		setPMLightOn();
		setBuzzerOn();
				
		sleep_cpu();
		
		setAlarmLightOff();
		setPMLightOff();
		setBuzzerOff();
				
		sleep_cpu();
		

				

		
	}
		
}

