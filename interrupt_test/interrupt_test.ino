
#include <TinyPinChange.h>

#define HOURSPIN 0
#define MINUTESPIN 1
#define SWITCHPIN 1  // read analog port 1, which on the digispark is P2
#define QUAD_A 4  //interupt pin on pin4
#define QUAD_B 5  //interupt pin on pin5

#define SF_SECS .2553
#define SF_TENSECS .0255

#define SF_HOURS 43200000L
#define SF_MINS 3600000L

#define BAND1 85
#define BAND2 256
#define BAND3 426
#define BAND4 597
#define BAND5 768
#define BAND6 938

//analog port 0 = pin5
//analog port 1 = pin2
//analog port 2 = pin4
//analog port 3 = pin3


//<<<   inputs   >>>
int valSwitch=0;

//<<<   outputs   >>>
int hValDisplay=0;
int mValDisplay=0;

//<<<    internal   >>>
long milliStart;
long time = 0;
int maxHRange = 240;
int maxMRange = 240;

//<<<    interrupt   >>>
volatile long enc_count = 0;
volatile boolean lights = true;
uint8_t VirtualPortNb;
uint8_t VirtualPortNb_;




// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
 // pinMode(HOURSPIN, OUTPUT); //LED on Model B
 // pinMode(minutesPin, OUTPUT); //LED on Model A
 
    TinyPinChange_Init();
    VirtualPortNb=TinyPinChange_RegisterIsr(QUAD_A, encoder_isr);
    VirtualPortNb_=TinyPinChange_RegisterIsr(QUAD_B,encoder_isr);
    
    TinyPinChange_EnablePin(QUAD_A);
    TinyPinChange_EnablePin(QUAD_B);
    
    
    digitalWrite(MINUTESPIN, lights);
}

// the loop routine runs over and over again forever:
void loop() { 
  
 
}


void encoder_isr() {
    static int8_t lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t enc_val = 0;
    
    enc_val = enc_val << 1;
    enc_val = enc_val + digitalRead(QUAD_A); 
    enc_val = enc_val << 1;
    enc_val = enc_val + digitalRead(QUAD_B);

    
//    enc_val = enc_val | ((PIND & 0b1100) >> 2) leftover from makeatronics.blogspot.com
 
    enc_count = enc_count + lookup_table[enc_val & 0b1111];
    
    lights = !lights;
    digitalWrite(MINUTESPIN, lights);
}

