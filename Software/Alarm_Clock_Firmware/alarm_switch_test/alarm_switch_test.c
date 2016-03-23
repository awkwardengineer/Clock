/*
 * alarm_switch_test.c
 *
 * Created: 3/13/2016 10:13:29 PM
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
	
	uint8_t oldPotReading;
	uint8_t newPotReading;
	
	uint8_t cal_hours;
	uint8_t cal_minutes;
	
	bool isAlarmSet;
	bool isAlarmSounding;
	bool isUIActive;
	bool isAlarmSwitchOn;
	
};

void pollInputs(struct State *state){
	
	state->newPotReading = pollPot();
}

void processInputs(struct State *state){
	static bool isSwitchUp;
	
	if (state->newPotReading > SWITCH_ENGAGE_POINT)
	{
		
	}
	
		/*
	
	if alarm switch is on
		
		if alarm is sounding
			disable alarm
			
		else
			if in time mode or alarm mode
				set the alarm
	
	if the knob moved, set isUIActive
	
	if isUIActive  (should also be settable by encoder interrupt)
		uiActiveCounter resets
	else
		uiActiveCounter-- until 0
	
	if uiActiveCounter is not zero
		set a short timer interrupt
	else
		set a long timer interrupt
	*/
}


void updateLEDs(struct State *state){
	if (state->isAlarmSounding){
		return;//relinquish control and do nothing if alarm is sounding
	}
	else{
		if (state->isAlarmSet){
			setAlarmLightOn();
		}
		else{
			setAlarmLightOff();
		}
		
		/*if am/pm
			setAMLightON();
		else
			setAMLightOff();*/	
	}
}

void updateSpeaker(struct State *state){
	static unsigned long lastBeepTime;
	static bool isBeeping;
	
	if ((state->numTicks - lastBeepTime) > 1){ //only run if it's been long enough (length of beep)
		lastBeepTime = state->numTicks;
	
	
		if (isBeeping){
			setAlarmLightOff();
			setPMLightOff();
			setBuzzerOff();
			isBeeping = false;
		}
		else{
			setAlarmLightOn();
			setPMLightOn();
			setPMLightOff();
			isBeeping = true;
		}
	}	
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
		
		
		pollInputs(&state);
		processInputs(&state);
		
		updateLEDs(&state);
		updateSpeaker(&state);

		
	}
		
}

