
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"
#include <NewPing.h>
#include "config.h"
#include "core.h"
#include "legs.h"

void colorWipeLeaf(int startLED, int endLED, int colorR,int colorG,int colorB, int whiteC)
{
  //light just one one we need
  for (int i=startLED; i < endLED; i++)
  {
    leaf_strip.setPixelColor(i, colorR,colorG,colorB,whiteC);
  }
  leaf_strip.show();
}

void setup() {
	pinMode(PIN_BUTTON, INPUT);

  //Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  
	//create a "real" random numbers
	randomSeed(analogRead(0)); 

	//start the LED strips
	leaf_strip.begin();
  stage_strip.begin();
   
  colorWipeStage(1,(LedsNumSide*SidesNum),192,0,10,30);
  
	//begins the SoftSerials
	serialCT.begin(115200); // Pixie REQUIRES this baud rate
	serialCB.begin(115200); // Pixie REQUIRES this baud rate
	//serialLEGS.begin(115200); // Pixie REQUIRES this baud rate

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

void loop() 
{
  timeRitualCheck = millis();
  if ((abs(timeRitualCheck - lastRitualCheck)) > interval_ritual)
  {
    if (cur_ritual_num < max_ritual) //rital is possible, start counting form now
    {
      if (cur_ritual_num == 0) //save the first ritual timestamp to be used on 10 min counter
        lastRitualTimestamp = timeRitualCheck;
      cur_ritual_num++;

      //TODO: start ritual
      // if start a ritual command has been given and there is no other ritual being processed start it
      //TODO: reset inProcessRitual at the end of the ritual
      if (startRitual and inProcessRitual == false)
      {
        inProcessRitual = true;
        //do ritual
        startRitual = false; //ready for the next ritual
      }
    }

    // if enough time passed since the first ritual reset thte timer
    if ((abs(timeRitualCheck - lastRitualTimestamp)) > max_ritual_period) 
      cur_ritual_num = 0;
    // save tim for the next loop-check
    lastRitualCheck = millis();
  }

  timeUltrasonic = millis();
  // every loop check if the time is right to read from the next sensor
  if ((abs(timeUltrasonic - lastUltrasonic)) > interval_timeUltrasonic)
  {
    cm[iSensorCycle] = sonar[iSensorCycle].ping_cm();
    if (cm[iSensorCycle] < detection_dis and sen_trig[iSensorCycle]==false)
    {
      //colorWipeStage((iSensorCycle*LedsNumSide),((iSensorCycle+1)*LedsNumSide),255,0,0,10);
      sen_trig[iSensorCycle] = true;
    }
    else if ((sen_trig[iSensorCycle]==true) and cm[iSensorCycle] > detection_dis)
    {
      //colorWipeStage((iSensorCycle*LedsNumSide),((iSensorCycle+1)*LedsNumSide),192,0,10,30);
      sen_trig[iSensorCycle] = false;
    }
    timeUltrasonic = millis();
    iSensorCycle++;
    iSensorCycle = iSensorCycle % SONAR_NUM;

    //check if all sensors are triggered and fade in completed. if so - start the ritual
    for (int k = 0; k < SONAR_NUM; k++)
      if (sen_trig[k] and stage_fade[k] == 255)
        sumSenors++;

    if (sumSenors == SONAR_NUM)
      startRitual = true;

  }
  
  timeStageLights = millis();
  if ((abs(timeStageLights - lastStageLights)) > interval_StageLights)
  {
    for (int i; i < SidesNum; i++)
    {
      //fade in
      if ((stage_fade[i] < 255) and sen_trig[i] == true)
        stage_fade[i]++;
      //fade out
      if ((stage_fade[i] > 0) and sen_trig[i] == false)
        stage_fade[i]--;

      colorWipeStage((i*LedsNumSide),((i+1)*LedsNumSide),stage_fade[i],0,0,10);
    }
    lastStageLights = millis();
  }

  buttonReading();
	// leafs' state machine colors
  switch (leafStateMachine) {
	case 0:
    // full(100%) white brightness
		colorWipeLeaf(0,ledsPerStrip,15,5,0,255);
    leg_state_machine = 0;
		break;
	case 1:
    // 75% white brightness
		colorWipeLeaf(0,ledsPerStrip,15,5,0,196);
    leg_state_machine = 0;
		break;
	case 2:
		long timeStrip;
    timeStrip = millis();
		colorWipeLeaf(0,ledsPerStrip,128+25*cos(2*PI/12000*(3000-timeStrip)),0,128+127*cos(2*PI/12000*(3000-timeStrip)),0);
    leg_state_machine = 0;
    break;
  case 3:
    // Lights are down/off
    colorWipeLeaf(0,ledsPerStrip,0,0,0,0);
    leg_state_machine = 2;
		break;
  }
  
  // legs' state machine
  timeLegPulse_state = millis();
  // if enough time has passed, changes status on the state machine
  if ((abs(timeLegPulse_state - lastLegCheck_state)) > interval_del_state)
  {
    if (leg_state_machine == 0)
      //TODO: prevents from the state machine to change status
      leg_state_machine = 0;
    lastLegCheck_state = millis();
  }
  
  switch (leg_state_machine) {
  case 0:
    pulseLegs();
    break;
  case 1:
    fast_pulseLegs();
    break;
  case 2:
    off_Legs();
    break;
  }

  timeStateMachine = millis();
  if ((abs(timeStateMachine - lastStateMachine)) > interval_StateMachine)
  {
    switch (state_machine) {
    case 0: //idle
      //leafs command
      //legs command
      //core command
      break;
    case 1: //ritual
      //leafs command
      //legs command
      //core command
      break;
    case 2: //cooldown
      //leafs command
      //legs command
      //core command
      break;
    }    
    lastStateMachine = millis();
  }
}


// reads the button, takes care of debouncing and cycle thru the state machine
void buttonReading()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(PIN_BUTTON);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) 
  {
    // reset the debouncing timer
    lastButtonDebounceTime = millis();
  }

  if ((millis() - lastButtonDebounceTime) > debounceButtonDelay) 
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




