#ifndef _ALARM_HELPERS_C_
#define _ALARM_HELPERS_C_

#include "alarm_config.c"
#include <stdlib.h>

inline void setAlarmLightOn(void);
inline void setAlarmLightOff(void);
inline void setPMLightOn(void);
inline void setPMLightOff(void);
inline void setBuzzerOff(void);
inline void setBuzzerOn(void);
inline uint8_t pollPot(void);
uint8_t calcHoursFromTics(unsigned long tics, uint8_t scale);
uint8_t calcMinsFromTics(unsigned long tics, uint8_t scale);

void setMinutesMeter(uint8_t pwm);
void setHoursMeter(uint8_t pwm);

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
	//========> //PORTA &= ~(1<<MODE_SELECT_POWER); //turn pot back off to conserve power
	PRR |= (1<<PRADC); //shuts down the ADC and comparator
	
	return ADCH;  //returns hight byte of AtoD conversion
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

#endif