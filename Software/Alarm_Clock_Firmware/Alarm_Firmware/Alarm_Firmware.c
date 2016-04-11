/*
 * Alarm_Firmware.c
 *
 * Created: 3/26/2016 10:20:09 AM
 * Author : awkwardengineer
 */ 


#include <avr/io.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "..\shared\alarm_config.c"
#include "..\shared\alarm_helpers.c"

volatile int ISR_accumulator = 0;
volatile int ISR_ticks = 0;
volatile bool ISR_ticked = false;

enum Mode {MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES, MODE_WARBLE, MODE_TIME, MODE_ALARM};

struct State{
	unsigned long numTicks;
	unsigned long alarmTimeInTicks;
	
	uint8_t potReading;
	
	uint8_t cal_hours;
	uint8_t cal_minutes;
	
	uint8_t warbleLevel;
	
	int rotation_accumulator;
	
	enum Mode mode;
	
	bool isAlarmSet;
	bool isAlarmSounding;
	bool isUIActive;
	bool isAlarmSwitchOn;
	
	bool isJustEnteredMode;
	bool isJustBooted;
	
};

void pollInputs(struct State *state){
	
	state->potReading = pollPot();
}

void pollInputsWithCapSense(struct State *state){
	struct PotAndCap potAndCap;
	
	potAndCap = pollPotWithCapSense();
	state->potReading = potAndCap.pot;
	
	if (potAndCap.cap < CAP_SENSE_THRESHOLD)
	{
		state->isAlarmSwitchOn = true ; //
	}
	else{
		state->isAlarmSwitchOn = false;
	}
}

void processPolledInputs(struct State *state){
		static int oldPotReading;
		static bool isAlarmLatched = false;
		static enum Mode lastMode = MODE_TIME;
		
		//If the pot moved, the UI should be active		
		if ((int)state->potReading - oldPotReading > 5 ||
			(int)state->potReading - oldPotReading < -5){
			state->isUIActive = true;
			//state->isJustBooted = false;
		}
		
		oldPotReading = state->potReading;
		
		//if the switch is on, and the latch is not set, this is a new trigger
		if (state->isAlarmSwitchOn &&
			!isAlarmLatched){
				
				isAlarmLatched = true; //set a latch so it has to be turned off
				
				if (state->isAlarmSounding){
					state->isAlarmSounding = false ; //if alarm is sounding, turn it off
					state->isAlarmSet = false;
				}
				else{
					state->isAlarmSet = !state->isAlarmSet; //otherwise, toggle the alarm set status
						
						chirp();

				}		
		}
		
		if (!state->isAlarmSwitchOn){isAlarmLatched = false;} 
		

		
		/* for reference:
		#define POS_test 10
		#define POS_time 50
		#define POS_alarm 100
		#define POS_warble 150
		#define POS_hours 200
		#define POS_minutes 255*/
	
		if (state->potReading < POS_test){ state->mode = MODE_TEST;}
		else if (state->potReading < POS_time){state->mode = MODE_TIME;}
		else if (state->potReading < POS_alarm){state->mode = MODE_ALARM;}
		else if (state->potReading < POS_warble){state->mode = MODE_WARBLE;}
		else if (state->potReading < POS_hours){state->mode = MODE_CAL_HOURS;}
		else {state->mode = MODE_CAL_MINUTES;}
	
		if (state->mode != lastMode){
			state->isJustEnteredMode = true;
			chirp();
		}
		
		lastMode = state->mode;
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
//state->isJustBooted = false;
void processEncoderInputs(struct State *state){
	//MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES,MODE_WARBLE, MODE_TIME, MODE_ALARM
	int holder;
	
	switch(state->mode){
		case MODE_TEST:
			
			break;
			
		case MODE_TIME:
			state->numTicks += 60 * state->rotation_accumulator;
			
			if (state->rotation_accumulator != 0){
				state->isJustBooted = false;
			}
			break;
			
		case MODE_ALARM:
			state->alarmTimeInTicks += 60*state->rotation_accumulator;				
			break;
				
		case MODE_WARBLE:
			
			holder = (int)state->warbleLevel;
			holder -= state->rotation_accumulator;
			
			if (holder > 5){holder = 5;}
			if (holder < 1){holder = 1;}
			
			state->warbleLevel = holder;
			
			break;
				
		case MODE_CAL_HOURS:
			holder = (int)state->cal_hours + (int)state->rotation_accumulator;
			
			if (holder > 255){holder = 255;}
			if (holder < 0){holder = 0;}
			state->cal_hours = (uint8_t)holder;
			
			
			break;
			
		case MODE_CAL_MINUTES:
			holder = (int)state->cal_minutes + (int)state->rotation_accumulator;
						
			if (holder > 255){holder = 255;}
			if (holder < 0){holder = 0;}
			state->cal_minutes = (uint8_t)holder;
			
			break;
	}
	
	state->rotation_accumulator = 0;
}

void updateMeters(struct State *state){
	static int newModeCounter = 0;
	
	if (state->isJustEnteredMode){
		state->isJustEnteredMode = false;
		newModeCounter = 10;
	}
	
	if (newModeCounter > 0){
		newModeCounter--;
	}
	
	switch(state->mode){
		case MODE_TEST:
			setMinutesMeter(64);
			setHoursMeter(64);
			break;
		
		case MODE_TIME:
			if (newModeCounter > 4){
				setMinutesMeter(state->cal_minutes);
				setHoursMeter(state->cal_hours);
			}
			else if (newModeCounter > 3){
				setMinutesMeter(0);
				setHoursMeter(0);
			}
			else{
				setMinutesMeter(calcMinsFromTics(state->numTicks, state->cal_minutes));
				setHoursMeter(calcHoursFromTics(state->numTicks, state->cal_hours));
			}
			break;
		
		case MODE_ALARM:
			setMinutesMeter(calcMinsFromTics(state->alarmTimeInTicks, state->cal_minutes));
			setHoursMeter(calcHoursFromTics(state->alarmTimeInTicks, state->cal_hours));
			
			if (newModeCounter == 9 ){ setAlarmLightOff();}
			else if (newModeCounter == 8 ){ setAlarmLightOn();}
			else if (newModeCounter == 7 ){ setAlarmLightOff();}
			else if (newModeCounter == 6 ){ setAlarmLightOn();}
			else if (newModeCounter == 5 ){ setAlarmLightOff();}
			else if (newModeCounter == 4 ){ setAlarmLightOn();}
			else if (newModeCounter == 3 ){ setAlarmLightOff();}
			else if (newModeCounter == 0){newModeCounter = 10;}
			else{}
			break;
			
		case MODE_WARBLE:
			if (newModeCounter > 6){
				setMinutesMeter(state->cal_minutes);
				setHoursMeter(0);	
			}
			else if (newModeCounter > 3){
				setHoursMeter(state->cal_hours);
				setMinutesMeter(0);
			}
			else if (newModeCounter > 0){
				setMinutesMeter(state->cal_minutes);
				setHoursMeter(0);
			}
			else{
				//this is where the warbling happens
				
				static uint8_t warble_tics = 0;
				static bool isOdd = true;
				static bool minFlag = false;
				static bool hoursFlag = false;
				int direction = 128;
				
				int numTriggers = 0;
				
				warble_tics++;
				
				setMinutesMeter(calcMinsFromTics(state->numTicks, state->cal_minutes));
				setHoursMeter(calcHoursFromTics(state->numTicks, state->cal_hours));
				
				if (warble_tics % 3 == 0){
					direction = -32;	
				}
				else{
					direction = 128;
				}
				
				if (hoursFlag){
					HOURS_PWM += direction;
					hoursFlag = false;
				}
				if (minFlag){
					MINS_PWM += direction;
					minFlag = false;
				}
				
				
				//started out as 3,5,8 as the pseudorandom generators,
				//multiplied by 2 to get 6,10,16
				//multiplied again by 3/2 to get 9,15,24
				//trying by 2 for 12,20,32
				if (warble_tics % (3 * state->warbleLevel) == 0){
					numTriggers++;
					isOdd = !isOdd;
				}
				if (warble_tics % (5 * state->warbleLevel) == 0){
					numTriggers++;
				}
				if (warble_tics % (8 * state->warbleLevel) == 0){
					numTriggers++;
					isOdd = !isOdd;
				}
				
				if (numTriggers == 3){
					//least likely, sets up full wiggle
					newModeCounter = 10;
				}
				if (numTriggers == 2){
					//wiggles both
					hoursFlag = true;
					minFlag = true;
				}
				if (numTriggers == 1){
					//wiggles one
					if (isOdd)     { hoursFlag = true;}
					else           { minFlag = true;}
				}
				
				
			}
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
	//in alarm mode, the AM PM light reflects whether the alarm is set for AM or PM
	static bool flashOnBoot=true;
	
	if(state->isJustBooted){
		if(flashOnBoot){setPMLightOn();
						flashOnBoot=false;}
		else           {setPMLightOff();
						flashOnBoot=true;}
	}
	else{
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
	}
	
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
		
	}
}

void updateSpeaker(struct State *state){
	static unsigned long lastBeepTime;
	static bool isBeeping = false;
	
	if(state->isAlarmSounding){
	
	//	if ((state->numTicks - lastBeepTime) >= 1){ //only run if it's been long enough (length of beep)
	//		lastBeepTime = state->numTicks;
	
	
			if (isBeeping){
				setAlarmLightOff();
				setBuzzerOff();
				isBeeping = false;
			}
			else{
				setAlarmLightOn();
				setBuzzerOn();
				isBeeping = true;
			}
	//	}
	}
	else{
		setBuzzerOff();
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

void shouldAlarmSound(struct State *state){
	bool isLatchReleased = true;
	
	if (state->isAlarmSet &&
		isLatchReleased &&
		state->numTicks - state->alarmTimeInTicks <= 60 &&
		state->numTicks - state->alarmTimeInTicks >= 0
		){
		//the clock should fire the alarm if it's within a minute. also important not to accidentally
		//refire, hence the latch release
		
		state->isAlarmSounding = true;
		isLatchReleased = false;
	}
	
	if ((state->numTicks - state->alarmTimeInTicks) > 60){ isLatchReleased = true;}
	
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
	ISR_ticks++;
	ISR_ticked= true;
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
}

void plainClockLoop(struct State *state){
}

void alarmClockLoop(struct State *state){
	while (1) 
    {
		//************
		//*emptying ISRs
		//*************
		
		state->rotation_accumulator += ISR_accumulator;
		ISR_accumulator = 0;		
		
		if (ISR_ticked){
			ISR_ticked = false;
			
			//once a second
			if (ISR_ticks % 4 == 0){
				state->numTicks += 1;
				handleTimeRollover(state);
			}
		
			//4x per second
					
			shouldAlarmSound(state);  //this should run early to ensure it doesn't miss a time overlap
			
			pollInputsWithCapSense(state);
			processPolledInputs(state);
			
		}
		
		updateLEDs(state);
		updateSpeaker(state);

		processEncoderInputs(state);
		
		updateMeters(state);

		sleep_cpu();
	}
}

int main(void)
{
	struct State state;
	
	init_all();
		
	state.numTicks = FIVE_OCLOCK_SOMEWHERE;
	state.alarmTimeInTicks = NINE_AM;
	state.cal_hours = INITIAL_HOURS_CAL;
	state.cal_minutes = INITIAL_MINUTES_CAL;
	state.warbleLevel = 5;
	state.isAlarmSet = false;
	state.isUIActive = false;
	state.isAlarmSwitchOn = false;
	state.mode = MODE_TIME;
	state.isJustBooted = true;
	
	state.isAlarmSounding = false;  	
	
	struct PotAndCap potAndCap;
	
	potAndCap = pollPotWithCapSense();
	
	if (potAndCap.cap > CAP_SENSE_THRESHOLD ){
		alarmClockLoop(&state);
	}
	else{
		plainClockLoop(&state);
	}
		
}







