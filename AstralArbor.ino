
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"


#define NUMPIXELS 3 // Number of Pixies in the chain
#define PIXIE_PIN_CT  53 // Pin number for Core Top
#define PIXIE_PIN_CB  41 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS  45 // Pin number for the 5 legs

#define PIN_LEAF 23

//Black color for wipeout
#define BLCK   0x000000

const int ledsPerStrip = 30;

SoftwareSerial serialCT(-1, PIXIE_PIN_CT);
SoftwareSerial serialCB(-1, PIXIE_PIN_CT);
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
  chainLEGS.setBrightness(200);  // Adjust as necessary to avoid blinding
  
}

void loop() {
  
  //write the colors
  colorWipe(0,ledsPerStrip,255,0,0,0);
  delay(1000);

}
