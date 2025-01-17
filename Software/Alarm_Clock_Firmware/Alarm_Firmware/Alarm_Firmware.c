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
volatile uint16_t ISR_ticks = 0;
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

void pollInputsWithPulldown(struct State *state){
	
	state->potReading = pollPot();
	state->isAlarmSwitchOn = isSwitchUp();
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

void processAlarmPolledInputs(struct State *state){
		static int oldPotReading;
		static bool isAlarmLatched = false;
		static enum Mode lastMode = MODE_TIME;
		
		/*
		
		//	this block not needed for Alarm, as it runs at a 4hz loop and is sensitive enough	
		
		//If the pot moved, the UI should be active		
		if ((int)state->potReading - oldPotReading > 5 ||
			(int)state->potReading - oldPotReading < -5){
			state->isUIActive = true;
			//state->isJustBooted = false;
		}
		*/
		
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
		
		
		/* for reference
		#define POS_test 10
		#define POS_minutes 45
		#define POS_hours 97
		#define POS_warble 170
		#define POS_time 225
		#define POS_alarm 255*/

	
		if (state->potReading < POS_test){ state->mode = MODE_TEST;}
		else if (state->potReading < POS_minutes){state->mode = MODE_CAL_MINUTES;}
		else if (state->potReading < POS_hours){state->mode = MODE_CAL_HOURS;}
		else if (state->potReading < POS_warble){state->mode = MODE_WARBLE;}
		else if (state->potReading < POS_time){state->mode = MODE_TIME;}
		else {state->mode = MODE_ALARM;}
	
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

int controlSweep(struct State *state){
		static long oldnumticks = 0;
		static int super_accumulator = 0;
		static int super_accumulator2 = 0;
		static int super_accumulator3 = 0;
		static int super_accumulator4 = 0;
		static int sweep = 0;
		
		super_accumulator += state->rotation_accumulator;
		
		if (state->isAlarmSounding){
			sweep = 0;
			return sweep;
		}
		
		//if in sweep mode and input comes in the other direction, stop sweeping
		if ((sweep > 0) && 	(state->rotation_accumulator < 0)){
			sweep = 0;
			super_accumulator = 0;
			super_accumulator2 = 0;
			super_accumulator3 = 0;
			super_accumulator4 = 0;
			chirp();
		}
		else if ((sweep < 0) && (state->rotation_accumulator > 0)){
			sweep = 0;
			super_accumulator = 0;
			super_accumulator2 = 0;
			super_accumulator3 = 0;
			super_accumulator4 = 0;
			chirp();
		}
		
		int total;
		total = super_accumulator + super_accumulator2 + super_accumulator3 + super_accumulator4;
		//if enough turns are accumulated in 1 second, start sweeping
		if (sweep == 0){
			if (total > 14){sweep = 600; chirp();}
			else if (total < -14){sweep = -600; chirp();}
		}
		
		//if you switched modes while sweeping, stop sweeping
		if (state->isJustEnteredMode){
			sweep = 0;
			super_accumulator = 0;
			super_accumulator2 = 0;
			super_accumulator3 = 0;
			super_accumulator4 = 0;
		}
		
		//if the clock has ticked 2 seconds, flush the superaccumulator
		if (ISR_ticks > oldnumticks ){
			super_accumulator4 = super_accumulator3;
			super_accumulator3 = super_accumulator2;
			super_accumulator2 = super_accumulator;
			super_accumulator = 0;
			oldnumticks = ISR_ticks;
		}
		
		return sweep;
}


void processAlarmEncoderInputs(struct State *state){
	//MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES,MODE_WARBLE, MODE_TIME, MODE_ALARM
	int holder;
	int sweep;
	
		
	switch(state->mode){
		case MODE_TEST:
			
			break;
			
		case MODE_TIME:
			sweep = controlSweep(state);
		
			state->numTicks += 60 * state->rotation_accumulator + sweep;
			
			if (state->rotation_accumulator != 0){
				state->isJustBooted = false;
			}
			break;
			
		case MODE_ALARM:
			sweep = controlSweep(state);
		
			state->alarmTimeInTicks += 60*state->rotation_accumulator + sweep;				
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
		newModeCounter = 10;
	}
	
	if (newModeCounter > 0){
		newModeCounter--;
	}
	
	switch(state->mode){
		case MODE_TEST:
			OCR0A += 10;
			OCR0B += 10;
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

void updateClockMeters(struct State *state){
	static int newModeCounter = 3;
	
	if (state->isJustEnteredMode){
		turnOffTimerB(); //timerB makes the loop run faster. it's useful for warble mode, but should be off everywhere else
		newModeCounter = 4;
	}
	
	if (newModeCounter > 0){
		newModeCounter--;
	}
	
	switch(state->mode){
		case MODE_TEST:
			OCR0A += 10;
			OCR0B += 10;
		break;
		
		case MODE_TIME:
		if (newModeCounter > 2){
			setMinutesMeter(state->cal_minutes);
			setHoursMeter(state->cal_hours);
		}
		else if (newModeCounter > 1){
			setMinutesMeter(0);
			setHoursMeter(0);
		}
		else{
			setMinutesMeter(calcMinsFromTics(state->numTicks, state->cal_minutes));
			setHoursMeter(calcHoursFromTics(state->numTicks, state->cal_hours));
		}
		break;
		
		//there should be no MODE_ALARM in clock mode, but I'm trying not to
		//touch too many things
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
		if (newModeCounter > 2){
			setHoursMeter(state->cal_hours);
			setMinutesMeter(0);
		}
		else if (newModeCounter > 1){
			setHoursMeter(0);
			setMinutesMeter(state->cal_minutes);
		}
		else if (newModeCounter > 0){
			setHoursMeter(state->cal_hours);
			setMinutesMeter(0);
			turnOnTimerB();			//makes the warbles faster
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
				newModeCounter = 3;
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
		state->alarmTimeInTicks += TWENTY_FOUR_HOURS ; //prevents zero rollover bug
	}
	if (state->alarmTimeInTicks > THIRTY_SIX_HOURS){
		state->alarmTimeInTicks -= TWENTY_FOUR_HOURS ;
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
	ISR_ticked = ISR_ticked;
}

ISR (TIM1_OVF_vect){
	ISR_ticks++;
	ISR_ticked= true;
	//THIS ISR IS CALLLED WHEN THE 16 BIT TIMER HITS IT'S MATCH POINT
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
			
			//the available options for reading the pot are
			//    -just check the pot: pollInputs(state);
			//    -just check the pot and read residual capacitance: pollInputsWithCapSense(state);
			//    -check the pot and check the switch pulldown
			pollInputsWithPulldown(state);
			processAlarmPolledInputs(state);
			
		}
		processAlarmEncoderInputs(state);
		
		

		updateLEDs(state);
		updateSpeaker(state);	
		updateMeters(state);
		
		state->isJustEnteredMode = false;
		sleep_cpu();
	}
}

void powerONST(void){
/*	for( int i=0; i<32; i++){
		if(i<4){	HOURS_PWM = 64;	}
		else if (i<8){	HOURS_PWM = 128;	}
		else if (i<12){	HOURS_PWM = 192;	}
		else if (i<16){	HOURS_PWM = 255;	}
		else if (i<20){	HOURS_PWM = 192;	}
		else if (i<24){	HOURS_PWM = 128;	}
		else if (i<28){	HOURS_PWM = 64;	}
		else{	HOURS_PWM = 0;	}
		
		MINS_PWM = HOURS_PWM;
		
		sleep_cpu();
	}*/
	for( int i=0; i<16; i++){
		if(i<2){	HOURS_PWM = 64;	}
		else if (i<4){	HOURS_PWM = 128;	}
		else if (i<6){	HOURS_PWM = 192;	}
		else if (i<8){	HOURS_PWM = 255;	}
		else if (i<10){	HOURS_PWM = 192;	}
		else if (i<12){	HOURS_PWM = 128;	}
		else if (i<14){	HOURS_PWM = 64;	}
		else{	HOURS_PWM = 0;	}
		
		MINS_PWM = HOURS_PWM;
		
		sleep_cpu();
	}
}

/*
int controlClockSweep(struct State *state){
	static long oldnumticks = 0;
	static int super_accumulator = 0;
	static int super_accumulator2 = 0;
	static int super_accumulator3 = 0;
	static int super_accumulator4 = 0;
	static int sweep = 0;
	
	super_accumulator += state->rotation_accumulator;
		
	//if in sweep mode and input comes in the other direction, stop sweeping
	if ((sweep > 0) && 	(state->rotation_accumulator < 0)){
		sweep = 0;
		turnOffTimerB();
		super_accumulator = 0;
		super_accumulator2 = 0;
		super_accumulator3 = 0;
		super_accumulator4 = 0;
		//chirp();   removed chirp from plain Clock
	}
	else if ((sweep < 0) && (state->rotation_accumulator > 0)){
		sweep = 0;
		turnOffTimerB();
		super_accumulator = 0;
		super_accumulator2 = 0;
		super_accumulator3 = 0;
		super_accumulator4 = 0;
		//chirp();    removed chirp from plain Clock
	}
	
	int total;
	total = super_accumulator + super_accumulator2 + super_accumulator3 + super_accumulator4;
	//if enough turns are accumulated in 1 second, start sweeping
	if (sweep == 0){
		if (total > 14){
			sweep = 600;
			turnOnTimerB();
			//chirp();    removed chirp from plain Clock
		}
		else if (total < -14){
			sweep = -600;
			turnOnTimerB();
			//chirp();    removed chirp from plain Clock
		}
	}
	
	//if you switched modes while sweeping, stop sweeping
	if (state->isJustEnteredMode){
		sweep = 0;
		turnOffTimerB();
		super_accumulator = 0;
		super_accumulator2 = 0;
		super_accumulator3 = 0;
		super_accumulator4 = 0;
	}
	
	//if the clock has ticked 2 seconds, flush the superaccumulator
	if (ISR_ticks > oldnumticks ){
		super_accumulator4 = super_accumulator3;
		super_accumulator3 = super_accumulator2;
		super_accumulator2 = super_accumulator;
		super_accumulator = 0;
		oldnumticks = ISR_ticks;
	}
	
	return sweep;
}
*/

void processClockEncoderInputs(struct State *state){
	//MODE_TEST, MODE_CAL_HOURS, MODE_CAL_MINUTES,MODE_WARBLE, MODE_TIME, MODE_ALARM
	int holder;
	int sweep;
	
	
	switch(state->mode){
		case MODE_TEST:
			
		
		break;
		
		case MODE_TIME:
			//sweep = controlClockSweep(state);
			
			state->numTicks += 60 * state->rotation_accumulator ; // + sweep; no more sweeping in Clock mode
			
			if (state->rotation_accumulator != 0){
				state->isJustBooted = false;
			}
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

void processClockPolledInputs(struct State *state){
		static int oldPotReading;
		static enum Mode lastMode = MODE_TIME;
		
		//
		//even at a 1Hz loop, the combination of the knob and user response time is fast enough to not need this
		//If the pot moved, the UI should be active
		/*		
		if ((int)state->potReading - oldPotReading > 5 ||
			(int)state->potReading - oldPotReading < -5){
			state->isUIActive = true;
			//state->isJustBooted = false;
		}
		*/
		
		oldPotReading = state->potReading;
		
			/*	 for reference
			#define CLOCK_POS_test 10
			#define CLOCK_POS_minutes 65
			#define CLOCK_POS_hours 129
			#define CLOCK_POS_warble 216
			#define CLOCK_POS_time 255*/

	
		if (state->potReading < CLOCK_POS_test){ state->mode = MODE_TEST;}
		else if (state->potReading < CLOCK_POS_minutes){state->mode = MODE_CAL_MINUTES;}
		else if (state->potReading < CLOCK_POS_hours){state->mode = MODE_CAL_HOURS;}
		else if (state->potReading < CLOCK_POS_warble){state->mode = MODE_WARBLE;}
		else {state->mode = MODE_TIME;}
	
		if (state->mode != lastMode){
			state->isJustEnteredMode = true;
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

void plainClockLoop(struct State *state){
	while (1) 
    {
		//************
		//*emptying ISRs
		//*************
		
		state->rotation_accumulator += ISR_accumulator;
		ISR_accumulator = 0;		
		
		if (ISR_ticked){
			ISR_ticked = false;
			
			//Clock loop is at 1 Hz, so modulo 4 is commented out
			//if (ISR_ticks % 4 == 0){
				state->numTicks += 1;
				handleTimeRollover(state);
			//}
			
			pollInputs(state);
			processClockPolledInputs(state);
			
		}
		
		processClockEncoderInputs(state);

		//noLEDs in Clock mode, so commented out
		//updateLEDs(state);
		//updateSpeaker(state);	
		updateClockMeters(state);
		
		state->isJustEnteredMode = false;
		sleep_cpu();
	}
}


int main(void)
{
	struct State state;
	
	state.numTicks = FIVE_OCLOCK_SOMEWHERE;
	state.alarmTimeInTicks = NINE_AM;
	
	state.cal_hours = INITIAL_HOURS_CAL;
	state.cal_minutes = INITIAL_MINUTES_CAL;
	state.rotation_accumulator = 0;
	
	state.warbleLevel = 5;
	state.isAlarmSet = false;
	state.isUIActive = false;
	state.isAlarmSwitchOn = false;
	state.mode = MODE_TIME;
	state.isJustBooted = true;
	
	state.isAlarmSounding = false;  	
	
	
	
	init_all();
	
	powerONST();	
	
	if (isPlainClock()){
		set1HzTicks();
		plainClockLoop(&state);
	}
	else{
		set4HzTicks();
		alarmClockLoop(&state);
	}
	
		
}







