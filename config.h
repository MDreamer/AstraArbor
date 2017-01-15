// constant vars & pins' configuration
#ifndef CONFIG_H
#define CONFIG_H

#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"
#include <NewPing.h>

//Leafs' pin: pin 4 drives all the leafs
//ping pong: 7
//Legs' pins: 8 9 10 11 12 
//button: 13
//Stage: 3 
//Core: 5 6
//UltraSonic sensors: 22, 24, 26, 28, 30 - the sensors are not order like that

#define SONAR_NUM     5 // Number of sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.


#define PIN_LEAF 4 // one pin is used to control over all the leafs
#define PIN_STAGE 3 // one pin is used to 5 sides of the stage, 
                    //6 LEDs in every side

#define NUMPIXELS 3 // Number of Pixies in the chain
#define NUM_LEGS 5 // Number of legs in the structure

#define PIXIE_PIN_CT_TX  5 // Pin number for Core Top
#define PIXIE_PIN_CB_TX  6 // Pin number for Core Bottem

#define PIN_PING_PONG  7 // Pin for the ping pong balls

#define PIXIE_PIN_LEG_0_TX  8 // Pin number for leg 1
#define PIXIE_PIN_LEG_1_TX  9 // Pin number for leg 2
#define PIXIE_PIN_LEG_2_TX  10 // Pin number for leg 3
#define PIXIE_PIN_LEG_3_TX  11 // Pin number for leg 4
#define PIXIE_PIN_LEG_4_TX  12 // Pin number for leg 5

#define PIXIE_PIN_CT_UNUSED_RX  55 // Pin number for Core Top
#define PIXIE_PIN_CB_UNUSED_RX  56 // Pin number for Core Bottem
#define PIXIE_PIN_LEGS_UNUSED_RX  57 // Pin number for the 5 legs

#define PIN_BUTTON 13    // the number of the push button pin

//Black color for wipeout
#define BLCK   0x000000


// number of LEDs per leaf
const int ledsPerStrip = 90;

// num of sides in the stage and the num of led in each side
const int LedsNumSide = 6;
const int SidesNum = 5;

const int PingPongNum = 6; // num of ping pong balls = 5 (sides) + 1 (core)

// distance to detect a person
const unsigned int detection_dis = 80;

unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
bool sen_trig[SONAR_NUM] = {false,false,false,false,false};         // Are the sensors triggered
uint8_t iSensorCycle = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(24, 24, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(22, 22, MAX_DISTANCE),
  NewPing(28, 28, MAX_DISTANCE),
  NewPing(30, 30, MAX_DISTANCE),
  NewPing(26, 26, MAX_DISTANCE),
};

// Variables will change:
// leaf state machine:
// 0 = 100% White
// 1 = 75% White
// 2 = pulse
// 3 = OFF
int leafStateMachine = 0;         // the current state of the leafs' state machine
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastButtonDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceButtonDelay = 50;    // the debounce time; increase if the output flickers

// for blue climex
int j;
int period_flash = 10000;

int period_cooldown = 30000;
long timestamp_cooldown;


//used to fade in/out the stage light
uint8_t stage_fade[SidesNum];

long lastStageLights;
long timeStageLights;
const long interval_StageLights=10;

long lastCheck_legs;
long timePulse_legs;
const long interval_del_legs=5;

long lastCheck_core;
long timePulse_core;
const long interval_del_core=10;

long timeChasePongs; 
long lastChasePongs;
const long interval_ChasePongs = 250;

long timeUltrasonic;
long lastUltrasonic;
const long interval_timeUltrasonic = 10; //becasue we have 5 sensors it means that 50ms wil pass between every reading form every individual sensor
                                         //which is enough - the minimum value is ~29ms
                                         
int sumSenors = 0;  //number of sensors that were triggered

int max_ritual = 3; // number of ritual every max_ritual_period time
int cur_ritual_num = 0; // number of ritual happen in the last 10 min
long lastRitualTimestamp; //the point in time that the last ritual occured - used in order to count 10 min
const long max_ritual_period = 1000*60*10; // only max_ritual can occur during that time (ms in second)*(seconds in a minute)*(minutes) = 10 min
bool startRitual = false; // triggered by the US sensors
bool inProcessRitual = false; // is there a ritual in process

long lastRitualCheck;
long timeRitualCheck;
const long interval_ritual=1000;  //chekc x ms the status of ritual happening

//used to cycle between the pong balls in ritual (chase)
int iPongChase = 0;

//current status fo the state machine & desc:
// 0 = calm/normal
// 1 = climex
// 2 = off
int leg_state_machine = 0;

// the number of modes that the state machine is cycled thru and support
int stateMachineModes = 4;

//states:
// 0 = idle - reading sensors' data
// 1 = ritual - during ritual
// 2 = cooldown - after a ritual
int state_machine = 0;
long lastStateMachine;
long timeStateMachine;
const long interval_StateMachine = 5;


long lastLegCheck_state;
long timeLegPulse_state;
const long interval_del_state=30000;

int periodePulse_legs = 6000;
int displacePulse_legs = 1000;

// creates SoftSerial for the Pixies
SoftwareSerial serialCT(PIXIE_PIN_CT_UNUSED_RX, PIXIE_PIN_CT_TX);
SoftwareSerial serialCB(PIXIE_PIN_CB_UNUSED_RX, PIXIE_PIN_CB_TX);
//SoftwareSerial serialLEGS(PIXIE_PIN_LEGS_UNUSED_RX, PIXIE_PIN_LEGS_TX);

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
// stage control
Adafruit_NeoPixel stage_strip = Adafruit_NeoPixel((LedsNumSide * SidesNum), PIN_STAGE, NEO_GRBW + NEO_KHZ800);
// ping pong control
Adafruit_NeoPixel ping_chain = Adafruit_NeoPixel(PingPongNum, PIN_PING_PONG, NEO_RGB + NEO_KHZ800);

#endif //__CONFIG_H
