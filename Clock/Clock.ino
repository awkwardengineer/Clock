
int HOURSPIN = 0;
int MINUTESPIN = 4;
int KNOBPIN = 0;  // read analog port 0, which on the digispark, is P5

//analog port 0 = pin5
//analog port 1 = pin2
//analog port 2 = pin4
//analog port 3 = pin3



int val=0;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
 // pinMode(HOURSPIN, OUTPUT); //LED on Model B
 // pinMode(minutesPin, OUTPUT); //LED on Model A   
}

// the loop routine runs over and over again forever:
void loop() {
  
  val = analogRead(KNOBPIN); //divide by 4 because read is 0-1023, while write is 0-255
    
  analogWrite(HOURSPIN, 255 - val / 4);
  analogWrite(MINUTESPIN, val / 4);
 
 
  delay(100);               // wait for .1 sec
}
