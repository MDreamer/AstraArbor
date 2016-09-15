
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"


#define NUMPIXELS 3 // Number of Pixies in the chain
#define PIXIE_PIN_CT_TX  5 // Pin number for Core Top
#define PIXIE_PIN_CB_TX  6 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS_TX  7 // Pin number for the 5 legs

#define PIXIE_PIN_CT_UNUSED_RX  55 // Pin number for Core Top
#define PIXIE_PIN_CB_UNUSED_RX  56 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS_UNUSED_RX  57 // Pin number for the 5 legs

#define PIN_LEAF 4

//Black color for wipeout
#define BLCK   0x000000

const int ledsPerStrip = 89;

SoftwareSerial serialCT(PIXIE_PIN_CT_UNUSED_RX, PIXIE_PIN_CT_TX);
SoftwareSerial serialCB(PIXIE_PIN_CB_UNUSED_RX, PIXIE_PIN_CB_TX);
SoftwareSerial serialLEGS(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEGS_TX);

Adafruit_Pixie chainCT = Adafruit_Pixie(NUMPIXELS, &serialCT);
Adafruit_Pixie chainCB = Adafruit_Pixie(NUMPIXELS, &serialCB);
Adafruit_Pixie chainLEGS = Adafruit_Pixie(NUMPIXELS, &serialLEGS);

Adafruit_NeoPixel leaf_strip = Adafruit_NeoPixel(ledsPerStrip, PIN_LEAF, NEO_GRBW + NEO_KHZ800);

long lastCheck_legs;
long timePulse_legs;
const long interval_del_legs=5;

long lastCheck_core;
long timePulse_core;
const long interval_del_core=10;

//state machine desc:
// 1 = calm/normal
// 2 - climex
int state_machine = 1;
long lastCheck_state;
long timePulse_state;
const long interval_del_state=15000;

int periodePulse_legs = 6000;
int displacePulse_legs = 1000;

//is being used for the strip
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

  //create a "real" random numbers
  randomSeed(analogRead(0));

  //start the LED strip
  leaf_strip.begin();

  serialCT.begin(115200); // Pixie REQUIRES this baud rate
  serialCB.begin(115200); // Pixie REQUIRES this baud rate
  serialLEGS.begin(115200); // Pixie REQUIRES this baud rate
  
  chainCT.setBrightness(200);  // Adjust as necessary to avoid blinding
  chainCB.setBrightness(200);  // Adjust as necessary to avoid blinding
  chainLEGS.setBrightness(255);  // Adjust as necessary to avoid blinding  
}

void aniLegs(uint8_t wait) 
{
  uint16_t i, j;
  
    for(i=0; i< NUMPIXELS; i++) 
    {
      chainLEGS.setPixelColor(i, 0,100,255);
      for (j=0;j<255;j++)
      {
        chainLEGS.setBrightness(j);
        delay(wait);
        chainLEGS.show();
      }
    }
    
    
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos,Adafruit_Pixie px) {
  if(WheelPos < 85) {
   return px.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return px.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return px.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void pulseLegs()
{
timePulse_legs = millis();
  if ((abs(timePulse_legs - lastCheck_legs)) > interval_del_legs)
  {
      
    long time1 = millis();
    chainLEGS.setPixelColor(0, 128+25*cos(2*PI/12000*(3000-time1)),0,128+127*cos(2*PI/6000*(3000-time1)));
    chainLEGS.setPixelColor(1, 128+25*cos(2*PI/12000*(2000-time1)),0,128+127*cos(2*PI/6000*(2000-time1)));
    chainLEGS.setPixelColor(2, 128+25*cos(2*PI/12000*(1000-time1)),0,128+127*cos(2*PI/6000*(1000-time1)));      
    chainLEGS.show();
    lastCheck_legs = millis();
   }
}
int j;
int period_flash = 10000;
void fast_pulseLegs()
{
timePulse_legs = millis();
  if ((abs(timePulse_legs - lastCheck_legs)) > 10)
  {
      
    long time1 = millis();
    chainLEGS.setPixelColor(0, 0,0,128+127*cos(2*PI/period_flash*(time1)));
    chainLEGS.setPixelColor(1, 0,0,128+127*cos(2*PI/period_flash*(170-time1)));
    chainLEGS.setPixelColor(2, 0,0,128+127*cos(2*PI/period_flash*(340-time1)));
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
    chainLEGS.show();
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
void loop() {
  
  //write the colors LED strip
  colorWipe(0,ledsPerStrip,255,25,20,0);

  timePulse_state = millis();
  if ((abs(timePulse_state - lastCheck_state)) > interval_del_state)
  {
    if (state_machine == 1)
      state_machine = 2;
    lastCheck_state = millis();
  }
  
  if (state_machine == 1)
    pulseLegs();
  else
    fast_pulseLegs();
  pulseCore();


  
   
}

