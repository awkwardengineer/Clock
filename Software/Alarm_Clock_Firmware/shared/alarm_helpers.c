#ifndef _ALARM_HELPERS_C_
#define _ALARM_HELPERS_C_

#include "alarm_config.c"
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>

inline void setAlarmLightOn(void);
inline void setAlarmLightOff(void);
inline void setPMLightOn(void);
inline void setPMLightOff(void);
inline void setBuzzerOff(void);
inline void setBuzzerOn(void);
void chirp(void);
inline uint8_t pollPot(void);
inline uint8_t pollSwitchVoltage(void);
inline bool isPlainClock(void);
inline bool isSwitchUp(void);
inline struct PotAndCap pollPotWithCapSense(void);
uint8_t calcHoursFromTics(unsigned long tics, uint8_t scale);
uint8_t calcMinsFromTics(unsigned long tics, uint8_t scale);
void set4HzTicks(void);
void set1HzTicks(void);

void setMinutesMeter(uint8_t pwm);
void setHoursMeter(uint8_t pwm);

struct PotAndCap {
	uint8_t pot;
	uint8_t cap;
	};

inline void setAlarmLightOn(){
	PORTA |= (1<<ALARM_LIGHT);	
}

inline void setAlarmLightOff(){
	PORTA &= ~(1<<ALARM_LIGHT);
}

inline void setPMLightOn(){
	PORTA |= (1<<PM_LIGHT);
}

inline void setPMLightOff(){
	PORTA &= ~(1<<PM_LIGHT);
}

inline void setBuzzerOn(){
	PORTA |= (1<<ALARM_BUZZER);
}

inline void setBuzzerOff(){
	PORTA &= ~(1<<ALARM_BUZZER);
}

inline uint8_t pollPot(){

	PRR &= ~(1<<PRADC); //turns on the ADC comparator
	PORTA |= (1<<MODE_SELECT_POWER); //turn on to power pot
	ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
	while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
	PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
	PRR |= (1<<PRADC); //shuts down the ADC and comparator
	
	return ADCH;  //returns hight byte of AtoD conversion
}

inline uint8_t pollSwitchVoltage(){
	PRR &= ~(1<<PRADC); //turns on the ADC comparator
		 
		 ADMUX &= ~(0b00011111); //set the MUX bits to 00000, this selects ADC0 (MODE POWER pin as the AtoD source)
		 
			 //turn pot power back off
			 PORTA &= ~(1<<MODE_SELECT_POWER); //keep pot off to conserve power
		 
				 //make MODE POWER pin  an input
				 DDRA &= ~(1<<MODE_SELECT_POWER); //makes pot power pin an input
	
 					 ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
					 while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
		 
			//make MODE POWER an output again
			DDRA |= (1<<MODE_SELECT_POWER);		 				
		 				
		//return the admux to the POT input
		ADMUX |= (1<<MUX0); //return to MUX picking the POT input
		 				
	//shut down the ADC
	PRR |= (1<<PRADC); //shuts down the ADC and comparator
	
	return ADCH;
}

inline bool isSwitchUp(){
	
	if (pollSwitchVoltage() < SWITCH_THRESHOLD){
		return true;
	}
	else{
		return false;
	}
}

inline bool isPlainClock(){
	if (pollSwitchVoltage() < USB_POWER_THRESHOLD){
		return true;
	}
	else{
		return false;
	}
}

inline struct PotAndCap pollPotWithCapSense(){
		
	struct PotAndCap potAndCap;
	
	//turn on ADC and power
	PRR &= ~(1<<PRADC); //turns on the ADC comparator
	PORTA |= (1<<MODE_SELECT_POWER); //turn on to power pot
	
	//read pot	
	ADCSRA |= (1<<ADSC); // starts AtoD conversion by flipping ADC Start Conversion bit in AD Control and Status Register A
	while(ADCSRA & (1<<ADSC));  // loops while waiting for ADC to finish
	potAndCap.pot = ADCH; // save high byte in pot
	

		//switch AtoD Mux to MODE POWER pin
		ADMUX &= ~(0b00011111); //set the MUX bits to 00000, this selects ADC0 (MODE POWER pin as the AtoD source)
	
			int i=0;
			while (i<100){i++;}// delay to build up charge
			
			//turn pot power back off
			PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
					
				//make MODE POWER pin  an input
				DDRA &= ~(1<<MODE_SELECT_POWER); //makes pot power pin an input
	
						//read residual capacitive charge on input
						ADCSRA |= (1<<ADSC); 
						while(ADCSRA & (1<<ADSC)); 
						potAndCap.cap = ADCH;
	
				//make MODE POWER an output again
				DDRA |= (1<<MODE_SELECT_POWER);
	
	
		//return the admux to the POT input
		ADMUX |= (1<<MUX0); //return to MUX picking the POT input
	
	//shut down the ADC
	PRR |= (1<<PRADC); //shuts down the ADC and comparator
	
	
	return potAndCap;  //returns hight byte of AtoD conversion
}

void setMinutesMeter(uint8_t pwm){
	MINS_PWM = pwm; 
}

void setHoursMeter(uint8_t pwm){
	HOURS_PWM = pwm;
}


uint8_t calcHoursFromTics(unsigned long tics, uint8_t scale){
	uint8_t hours;
	
	hours = (((tics /(60* MINS_CONVERSION) % 12 ) * scale) / 12) ; //order of operations matters to avoid rounding errors
	if (hours==0){
		hours=scale;
	}
	
	return (uint8_t)(hours );  //get the low byte of hours;
}


uint8_t calcMinsFromTics(unsigned long tics, uint8_t scale){
	uint8_t minutes;
	minutes = (((tics / SECONDS_CONVERSION) % 3600) * scale) / 3600;
	
	return (uint8_t)(minutes); //get the low byte of minutes

}

void chirp(void){
	
	setBuzzerOn(); //the buzzer is turned off later on, but the idea is to get a short Bloop!
	int i=0;
	while(i<100){i++;}
}

void set4HzTicks(void){
	OCR1A = TICKS_4HZ;
}

void set1HzTicks(void){
	OCR1A = TICKS_1HZ;
}

void turnOnTimerB(void){
	OCR1B = OCR1A /2 ;
}

void turnOffTimerB(void){
	OCR1B = OCR1A + 1;
}

#endif