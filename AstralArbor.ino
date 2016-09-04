
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"


#define NUMPIXELS 3 // Number of Pixies in the chain
#define PIXIE_PIN_CT  5 // Pin number for Core Top
#define PIXIE_PIN_CB  6 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS  7 // Pin number for the 5 legs

#define PIN_LEAF 4

//Black color for wipeout
#define BLCK   0x000000

const int ledsPerStrip = 89;

SoftwareSerial serialCT(-1, PIXIE_PIN_CT);
SoftwareSerial serialCB(-1, PIXIE_PIN_CB);
SoftwareSerial serialLEGS(-1, PIXIE_PIN_LEGS);

Adafruit_Pixie chainCT = Adafruit_Pixie(NUMPIXELS, &serialCT);
Adafruit_Pixie chainCB = Adafruit_Pixie(NUMPIXELS, &serialCB);
Adafruit_Pixie chainLEGS = Adafruit_Pixie(NUMPIXELS, &serialLEGS);

Adafruit_NeoPixel leaf_strip = Adafruit_NeoPixel(ledsPerStrip, PIN_LEAF, NEO_GRBW + NEO_KHZ800);

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

int rndCoreR;
int rndCoreG;
int rndCoreB;

int rndLegsR;
int rndLegsG;
int rndLegsB;

void setup() {

  rndCoreR = random(0,25);
  rndCoreG = random(0,25);
  rndCoreB = random(0,25);

  rndLegsR = random(0,25);
  rndLegsG = random(0,25);
  rndLegsB = random(0,25);
  
  //create a "real" random numbers
  randomSeed(analogRead(0));

  //start the LED strip
  leaf_strip.begin();

  serialCT.begin(115200); // Pixie REQUIRES this baud rate
  serialCB.begin(115200); // Pixie REQUIRES this baud rate
  serialLEGS.begin(115200); // Pixie REQUIRES this baud rate
  
  chainCT.setBrightness(200);  // Adjust as necessary to avoid blinding
  chainCB.setBrightness(200);  // Adjust as necessary to avoid blinding
  chainLEGS.setBrightness(200);  // Adjust as necessary to avoid blinding

  
  
}

bool fadeCoreUpR = true;
bool fadeCoreUpG = true;
bool fadeCoreUpB = true;

bool fadeLegsUpR = true;
bool fadeLegsUpG = true;
bool fadeLegsUpB = true;
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait, Adafruit_Pixie _chain1) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      _chain1.setPixelColor(i, Wheel(((i * 256 / _chain1.numPixels()) + j) & 255,_chain1));
    }
    _chain1.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_Pixie _chain) {
  if(WheelPos < 85) {
   return _chain.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return _chain.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return _chain.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void loop() {
  
  //write the colors
  colorWipe(0,ledsPerStrip,255,25,20,0);
  
  rainbowCycle(10, chainCT);
  //rainbowCycle(15, chainCB);
  //rainbowCycle(30, chainLEGS);
  delay(10);
}

