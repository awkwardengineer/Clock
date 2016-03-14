
#include "alarm_config.c"

inline void setAlarmLightOn(void);
inline void setAlarmLightOff(void);
inline void setPMLightOn(void);
inline void setPMLightOff(void);
inline void setBuzzerOff(void);
inline void setBuzzerOn(void);

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