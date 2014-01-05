
#define HOURSPIN 0
#define MINUTESPIN 4
#define KNOBPIN 0  // read analog port 0, which on the digispark, is P5

#define SF_SECS .2553
#define SF_TENSECS .0255

//analog port 0 = pin5
//analog port 1 = pin2
//analog port 2 = pin4
//analog port 3 = pin3



int valKnob=0;
int valSwitch=0;
int hValDisplay=0;
int mValDisplay=0;

long milliStart;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
 // pinMode(HOURSPIN, OUTPUT); //LED on Model B
 // pinMode(minutesPin, OUTPUT); //LED on Model A   
}

// the loop routine runs over and over again forever:
void loop() {
  
  valKnob = analogRead(KNOBPIN); //divide by 4 because read is 0-1023, while write is 0-255
  
  if (true){ // this will eventually be a series of if statements for each Switch State
    // this case is the "test mode" which should scroll through the display very quickly.
    
      milliStart = millis();
      hValDisplay = int(float(milliStart % 10000) * SF_TENSECS);
      mValDisplay = int(float(milliStart % 1000) * SF_SECS);
       
  }
    
    
    
  analogWrite(HOURSPIN, hValDisplay);
  analogWrite(MINUTESPIN, mValDisplay);
 
 
}
