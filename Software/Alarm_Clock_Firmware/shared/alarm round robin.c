
#define ISCLOCK 1;
#define ISALARM 0;

typedef struct State{
	unsigned long numTicks;
	unsigned long alarmTimeInTicks;
	
	
	uint8_t cal_hours;
	uint8_t cal_minutes;
	
	bool isAlarmSet;
	bool isAlarmSounding;
	bool isUIActive;
	bool isAlarmSwitchOn;
	
}


//headsup for the compiler
int powerOnSelfTest();
void pollInputs(struct State *state);			
void processInputs(struct State *state);			
void updateMeters(struct State *state);
void updateLEDs(struct State *state);
void updateSpeaker(struct State *state);
			

int powerOnSelfTest(){
	
	if (//test for a clock goes here)
	{
		return ISCLOCK;
	}
	else{
		return ISALARM;
	}
}

void pollInputs(struct State *state){
	/*
	//pseudo code follows
	static uiActiveCounter
	
	if not from timer trigger
		return
		
	poll_the_knob()
	
		turn knob power on
		turn ADC on
		read the knob
		turn knob power off <===== this is to test capacitance
		read the knob		<===== this reading is of residual charge
		turn ADC off
	
			
		reading 1 sets the state of mode knob
		reading 2 determines the state of alarm switch	
		
	
	*/
}

void processInputs(struct State *state){
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


void updateMeters(struct State *state){
	/*
	use the mode knob to pick the display state
		switch on state, or use mode pointers
		
		
		calMinutes()
		
		or calHours()
		
		or displayTime()
		
		or displayAlarmTime()
		
		
	*/
}
void updateLEDs(struct State *state){
	/*
		if am/pm
			setAMLightON();
		else
			setAMLightOff();
	
	
		if isAlarmSounding
			don't do anything  // relinquish controll to speaker/alarm
		else
			if alarmIsSet
				setAlarmLightOn();
			else
				setAlarmLightOff();
		
	
	
	*/
	
	
}
void updateSpeaker(struct State *state){
	/*static lastBuzzChange;
	  static lastBuzzMode;
	
	if currentTime - lastBuzzChange is > 1 sec
		lastBuzzChange = current time
		
		if lastBuzzMode
			alarmLightON
			speakerON
		else
			alarmLightOff
			speakerOff
			
		toggle lastBuzzMode
	*/
}





main(){
	
	struct State state;
	
	init();
	
	
	if (powerOnSelfTest() == ISCLOCK){
		while(1){
			
			
		}
	}
	else{
		while(1){
			pollInputs(&state);
			
			processInputs(&state);
			
			updateMeters(&state);
			updateLEDs(&state);
			updateSpeaker(&state);
			
			sleep
		}
	}
	
	
}