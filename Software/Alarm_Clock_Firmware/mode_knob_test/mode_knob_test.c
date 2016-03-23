/*
 * mode_knob_test.c
 *
 * Created: 3/20/2016 8:56:11 PM
 * Author : awkwardengineer
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "..\shared\alarm_config.c"
#include "..\shared\alarm_helpers.c"

volatile int ISR_accumulator = 0;
volatile bool isISR_ticked = false;

enum Mode {MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES, MODE_WARBLE, MODE_TIME, MODE_ALARM};

struct State{
	unsigned long numTicks;
	unsigned long alarmTimeInTicks;
	
	uint8_t oldPotReading;
	uint8_t newPotReading;
	
	uint8_t cal_hours;
	uint8_t cal_minutes;
	
	int rotation_accumulator;
	
	enum Mode mode;
	
	bool isAlarmSet;
	bool isAlarmSounding;
	bool isUIActive;
	bool isAlarmSwitchNewlyOn;
	
};

void pollInputs(struct State *state){
	
	state->newPotReading = pollPot();
}

void processPolledInputs(struct State *state){
	static bool wasSwitchReleased = false;
	
		if (state->newPotReading > SWITCH_ENGAGE_POINT)
		{
			
			if (wasSwitchReleased){
				state->isAlarmSwitchNewlyOn = true ; //
			}
			else{
				state->isAlarmSwitchNewlyOn = false ;
			}
		
			wasSwitchReleased = false;
		}
		else{
			wasSwitchReleased = true;
			state->isAlarmSwitchNewlyOn = false;
		}
		
	
		if (state->isAlarmSwitchNewlyOn){
			
			if (state->isAlarmSounding){
					state->isAlarmSounding = false ; //if alarm is sounding, turn it off
				}
			else{
				state->isAlarmSet = !state->isAlarmSet; //otherwise, toggle the alarm set status
			}
			
		}
		
		/*#define POS_test 6    //listed here for reference
		#define POS_time 25
		#define POS_alarm 50
		#define POS_warble 75
		#define POS_hours 100
		#define POS_minutes 128
		#define POS_testh 134
		#define POS_timeh 150
		#define POS_alarmh 175
		#define POS_warbleh 200
		#define POS_hoursh 225
		#define POS_minutesh 255*/
		if (state->newPotReading < POS_test){ state->mode = MODE_TEST;}
		else if (state->newPotReading < POS_time){state->mode = MODE_TIME;}
		else if (state->newPotReading < POS_alarm){state->mode = MODE_ALARM;}
		else if (state->newPotReading < POS_warble){state->mode = MODE_WARBLE;}
		else if (state->newPotReading < POS_hours){state->mode = MODE_CAL_HOURS;}
		else if (state->newPotReading < POS_minutes){state->mode = MODE_CAL_MINUTES;}
		else if (state->newPotReading < POS_testh){ state->mode = MODE_TEST;}
		else if (state->newPotReading < POS_timeh){state->mode = MODE_TIME;}
		else if (state->newPotReading < POS_alarmh){state->mode = MODE_ALARM;}
		else if (state->newPotReading < POS_warbleh){state->mode = MODE_WARBLE;}
		else if (state->newPotReading < POS_hoursh){state->mode = MODE_CAL_HOURS;}
		else if (state->newPotReading < POS_minutesh){state->mode = MODE_CAL_MINUTES;}
	
	/*	
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

void processEncoderInputs(struct State *state){
	//MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES,MODE_WARBLE, MODE_TIME, MODE_ALARM
	
	switch(state->mode){
		case MODE_TEST:
			
			break;
			
		case MODE_TIME:
			state->numTicks += 60 * state->rotation_accumulator;
		
			break;
			
		case MODE_ALARM:
		state->alarmTimeInTicks += 60*state->rotation_accumulator;
				
				break;
				
		case MODE_WARBLE:
		
			break;
				
		case MODE_CAL_HOURS:
			state->cal_hours += state->rotation_accumulator;
			
			break;
			
		case MODE_CAL_MINUTES:
			state->cal_minutes += state->rotation_accumulator;
			
			break;
		

		
		

	}
	
	state->rotation_accumulator = 0;
}

void updateMeters(struct State *state){
	
	
	switch(state->mode){
		case MODE_TEST:
			setMinutesMeter(64);
			setHoursMeter(64);
			break;
		
		case MODE_TIME:
			setMinutesMeter(calcMinsFromTics(state->numTicks, state->cal_minutes));
			setHoursMeter(calcHoursFromTics(state->numTicks, state->cal_hours));
			break;
		
		case MODE_ALARM:
			setMinutesMeter(calcMinsFromTics(state->alarmTimeInTicks, state->cal_minutes));
			setHoursMeter(calcHoursFromTics(state->alarmTimeInTicks, state->cal_hours));
			break;
			
		case MODE_WARBLE:
			setMinutesMeter(192);
			setHoursMeter(192);
			break;
		
		case MODE_CAL_HOURS:
			setMinutesMeter(0);
			setHoursMeter(state->cal_hours);
			break;
		
		case MODE_CAL_MINUTES:
			setMinutesMeter(state->cal_minutes);
			setHoursMeter(0);
			break;
		

		

	}
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
		
		//in alarm mode, the AM PM light reflects whether the alarm is set for AM or PM
		if (state->mode == MODE_ALARM){
			if (state->alarmTimeInTicks >= AMPM_LINE){
				setPMLightOn();
			}
			else{
				setPMLightOff();
			}
		}
		else{ 
			if (state->numTicks >= AMPM_LINE){   //in all other modes, the AM PM light reflects AM and PM
				setPMLightOn();
			}
			else{
				setPMLightOff();
			}
			
		}
		/*if am/pm
			setAMLightON();
		else
			setAMLightOff();*/	
	}
}

void updateSpeaker(struct State *state){
	static unsigned long lastBeepTime;
	static bool isBeeping = false;
	
	if(state->isAlarmSounding){
	
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
}


void handleTimeRollover (struct State *state){
	if (state->numTicks < TWELVE_HOURS){
		state->numTicks += TWENTY_FOUR_HOURS ; //prevents zero rollover bug
	}
	if (state->numTicks > THIRTY_SIX_HOURS){
		state->numTicks -= TWENTY_FOUR_HOURS ;
	}
	if (state->alarmTimeInTicks < TWELVE_HOURS){
		state->numTicks += TWENTY_FOUR_HOURS ; //prevents zero rollover bug
	}
	if (state->alarmTimeInTicks > THIRTY_SIX_HOURS){
		state->numTicks -= TWENTY_FOUR_HOURS ;
	}
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

		ISR_accumulator += lookup_table[grey_code];
		
}

ISR (TIM1_COMPB_vect){	
}

ISR (TIM1_OVF_vect){
	isISR_ticked = true;
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
	
	//volatile variable was defined in main()
	/*timer_interrupts += 1;  //in Compare Timer and Clear Mode, the register should have the number of Timer ticks
	if (timer_interrupts < TWELVE_HOURS){
		timer_interrupts += TWELVE_HOURS ; //prevents zero rollover bug
	}
	if (timer_interrupts > THIRTY_SIX_HOURS){
		timer_interrupts -= TWELVE_HOURS ;
	}
	*/
}

int main(void)
{


	struct State state;
	
	init_all();
		
	state.numTicks = FIVE_OCLOCK_SOMEWHERE;
	state.alarmTimeInTicks = FIVE_OCLOCK_SOMEWHERE;
	state.cal_hours = INITIAL_HOURS_CAL;
	state.cal_minutes = INITIAL_MINUTES_CAL;
	state.isAlarmSet = false;
	state.isUIActive = false;
	state.isAlarmSwitchNewlyOn = false;
	state.mode = MODE_TIME;
	
	state.isAlarmSounding = false;  	
	
    while (1) 
    {
		//************
		//*emptying ISRs
		//*************
		
		state.rotation_accumulator += ISR_accumulator;
		ISR_accumulator = 0;
		
		if (isISR_ticked){
			isISR_ticked = false;
			state.numTicks += 1;
		}
		
		pollInputs(&state);
		processPolledInputs(&state);
		processEncoderInputs(&state);
		
		handleTimeRollover(&state);
		
		updateLEDs(&state);
		updateSpeaker(&state);
		updateMeters(&state);

		sleep_cpu();
	}
		
}





