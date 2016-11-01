
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"

// 3 4 5 6 7 
//LEGS
//8 9 10 11 12 
//13

#define NUMPIXELS 3 // Number of Pixies in the chain
#define NUM_LEGS 5 // Number of legs in the structure

#define PIXIE_PIN_CT_TX  5 // Pin number for Core Top
#define PIXIE_PIN_CB_TX  6 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS_TX  7 // Pin number for the all 5 legs

#define PIXIE_PIN_LEG_0_TX  8 // Pin number for leg 1
#define PIXIE_PIN_LEG_1_TX  9 // Pin number for leg 2
#define PIXIE_PIN_LEG_2_TX  10 // Pin number for leg 3
#define PIXIE_PIN_LEG_3_TX  11 // Pin number for leg 4
#define PIXIE_PIN_LEG_4_TX  12 // Pin number for leg 5

#define PIXIE_PIN_CT_UNUSED_RX  55 // Pin number for Core Top
#define PIXIE_PIN_CB_UNUSED_RX  56 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS_UNUSED_RX  57 // Pin number for the 5 legs

// one pin is used to control over all the leafs
#define PIN_LEAF 4

const int buttonPin = 13;    // the number of the pushbutton pin

// Variables will change:
int leafStateMachine = HIGH;         // the current state of the leafs' state machine
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//Black color for wipeout
#define BLCK   0x000000

// number of LEDs per leaf
const int ledsPerStrip = 90;

// creates SoftSerial for the Pixies
SoftwareSerial serialCT(PIXIE_PIN_CT_UNUSED_RX, PIXIE_PIN_CT_TX);
SoftwareSerial serialCB(PIXIE_PIN_CB_UNUSED_RX, PIXIE_PIN_CB_TX);
SoftwareSerial serialLEGS(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEGS_TX);

//TODO: add a contructor array to the class
//SoftwareSerial serialLEG[2] = {(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_0_TX),(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_0_TX);
SoftwareSerial serialLEG_0(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_0_TX);
SoftwareSerial serialLEG_1(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_1_TX);
SoftwareSerial serialLEG_2(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_2_TX);
SoftwareSerial serialLEG_3(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_3_TX);
SoftwareSerial serialLEG_4(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEG_4_TX);

// creates the Pixes' objects instances
Adafruit_Pixie chainCT = Adafruit_Pixie(NUMPIXELS, &serialCT);
Adafruit_Pixie chainCB = Adafruit_Pixie(NUMPIXELS, &serialCB);
Adafruit_Pixie chainLEG[NUM_LEGS] = {(Adafruit_Pixie(NUMPIXELS, &serialLEG_0)),(Adafruit_Pixie(NUMPIXELS, &serialLEG_1)), 
(Adafruit_Pixie(NUMPIXELS, &serialLEG_2)) ,(Adafruit_Pixie(NUMPIXELS, &serialLEG_3)), (Adafruit_Pixie(NUMPIXELS, &serialLEG_4))};

// leafs control
Adafruit_NeoPixel leaf_strip = Adafruit_NeoPixel(ledsPerStrip, PIN_LEAF, NEO_GRBW + NEO_KHZ800);

long lastCheck_legs;
long timePulse_legs;
const long interval_del_legs=5;

long lastCheck_leg[NUM_LEGS] = {0};
long timePulse_leg[NUM_LEGS] = {0};
const long interval_del_leg=5;

long lastCheck_core;
long timePulse_core;
const long interval_del_core=10;

//current status fo the state machine & desc:
// 1 = calm/normal
// 2 - climex
int state_machine = 1;

// the number of modes that the state machine is cycled thru
int stateMachineModes = 3;

long lastCheck_state;
long timePulse_state;
const long interval_del_state=30000;

int periodePulse_legs = 6000;
int displacePulse_legs = 1000;

//is being used for the strips/leafs
void colorWipe(int startLED, int endLED, int colorR,int colorG,int colorB, int whiteC)
{
  //kill all the leds
  for (int i=0; i < ledsPerStrip; i++)
  {
    leaf_strip.setPixelColor(i, BLCK);
    
  }
  //light just one one we need
  for (int i=0; i < ledsPerStrip; i++)
  {
    leaf_strip.setPixelColor(i, colorR,colorG,colorB,whiteC);
  }
  leaf_strip.show();
}
  

void setup() {
	pinMode(buttonPin, INPUT);
  
	//create a "real" random numbers
	randomSeed(analogRead(0));

	//start the LED strip
	leaf_strip.begin();

	//begins the SoftSerials
	serialCT.begin(115200); // Pixie REQUIRES this baud rate
	serialCB.begin(115200); // Pixie REQUIRES this baud rate
	serialLEGS.begin(115200); // Pixie REQUIRES this baud rate

  serialLEG_0.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_1.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_2.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_3.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_4.begin(115200); // Pixie REQUIRES this baud rate
  
	chainCT.setBrightness(200);  // Adjust as necessary to avoid blinding
	chainCB.setBrightness(200);  // Adjust as necessary to avoid blinding

  for (int i = 0;i < NUM_LEGS; i++)
  {
    chainLEG[i].setBrightness(255);
  }
 
}


void pulseLeg(Adafruit_Pixie &chainLEG)
{

}

void pulseLegs()
{
  timePulse_legs = millis();
  if ((abs(timePulse_legs - lastCheck_legs)) > interval_del_legs)
  {
      
    long time1 = millis();

    for (int i = 0;i < NUM_LEGS; i++)
    {
      chainLEG[i].setPixelColor(0, 128+25*cos(2*PI/12000*(3000-time1)),0,128+127*cos(2*PI/6000*(3000-time1)));
      chainLEG[i].setPixelColor(1, 128+25*cos(2*PI/12000*(2000-time1)),0,128+127*cos(2*PI/6000*(2000-time1)));
      chainLEG[i].setPixelColor(2, 128+25*cos(2*PI/12000*(1000-time1)),0,128+127*cos(2*PI/6000*(1000-time1)));      
      chainLEG[i].show();
    }
    
    lastCheck_legs = millis();
   }
}
int j;
int period_flash = 10000;


// Climex blue
void fast_pulseLegs()
{
timePulse_legs = millis();
  if ((abs(timePulse_legs - lastCheck_legs)) > 10)
  {
      
    long time1 = millis();
    for (int i = 0;i < NUM_LEGS; i++)
    {
      chainLEG[i].setPixelColor(0, 0,0,128+127*cos(2*PI/period_flash*(time1)));
      chainLEG[i].setPixelColor(1, 0,0,128+127*cos(2*PI/period_flash*(170-time1)));
      chainLEG[i].setPixelColor(2, 0,0,128+127*cos(2*PI/period_flash*(340-time1)));
      chainLEG[i].show();
    }
    
    if (period_flash > 2500 and period_flash < 7500)
      period_flash = period_flash - 5;
    else if (period_flash < 2500)
      period_flash = period_flash - 2;
    else
      period_flash = period_flash - 10;
    if (period_flash <= 500)
    {
      period_flash = 10000;
      state_machine = 1;
    }
    
    lastCheck_legs = millis();
   }
}

void pulseCore()
{
timePulse_core = millis();
  if ((abs(timePulse_core - lastCheck_core)) > interval_del_core)
  {
      
    long time2 = millis();
    chainCT.setPixelColor(0, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(3000-time2)));
    chainCT.setPixelColor(1, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(2000-time2)));
    chainCT.setPixelColor(2, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(1000-time2)));      
    chainCT.show();

    chainCB.setPixelColor(0, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(3000-time2)));
    chainCB.setPixelColor(1, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(2000-time2)));
    chainCB.setPixelColor(2, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(1000-time2)));      
    chainCB.show();
    
    lastCheck_core = millis();
   }
}

// reads the button, takes care of debouncing and cycle thru the state machine
void buttonReading()
{
	// read the state of the switch into a local variable:
	int reading = digitalRead(buttonPin);

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonState) 
	{
		// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay) 
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != buttonState) 
		{
	  		buttonState = reading;

	  		// only toggle the LED if the new button state is HIGH
	  		if (buttonState == HIGH) 
	  		{
	    		leafStateMachine = leafStateMachine + 1;
	    		leafStateMachine = leafStateMachine % stateMachineModes;
	  		}
		}
  }


  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void loop() {

  buttonReading();
	// leafs' state machine colors
  	switch (leafStateMachine) {
	case 0:
		colorWipe(0,ledsPerStrip,15,5,0,255);
		break;
	case 1:
		colorWipe(0,ledsPerStrip,15,5,0,196);
		break;
	case 2:
		long time1 = millis();
		colorWipe(0,ledsPerStrip,128+25*cos(2*PI/12000*(3000-time1)),0,128+127*cos(2*PI/12000*(3000-time1)),0);
		break;
  }
  
  // legs' state machine
  timePulse_state = millis();
  // if enough time has passed, changes status on the state machine
  if ((abs(timePulse_state - lastCheck_state)) > interval_del_state)
  {
    if (state_machine == 1)
      //TODO: prevents from the state machine to change status
      state_machine = 1;
    lastCheck_state = millis();
  }
  
  if (state_machine == 1)
    pulseLegs();
  else
    fast_pulseLegs();

  pulseCore();


  
   
}

