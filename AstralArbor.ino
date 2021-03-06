
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"
#include <NewPing.h>
#include "config.h"
#include "core.h"
#include "legs.h"



void setup() {
	pinMode(PIN_BUTTON, INPUT);

  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  
	//create a "real" random numbers
	randomSeed(analogRead(0)); 

	//start the LED strips
	leaf_strip.begin();
  stage_strip.begin();
  ping_chain.begin();
  flower_chain.begin();
   
  colorWipeStage(1,(LedsNumSide*SidesNum),192,0,10,30);
  
	//begins the SoftSerials
	serialCT.begin(115200); // Pixie REQUIRES this baud rate
//	serialCB.begin(115200); // Pixie REQUIRES this baud rate
	//serialLEGS.begin(115200); // Pixie REQUIRES this baud rate

  serialLEG_0.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_1.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_2.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_3.begin(115200); // Pixie REQUIRES this baud rate
  serialLEG_4.begin(115200); // Pixie REQUIRES this baud rate
  
	chainCT.setBrightness(200);  // Adjust as necessary to avoid blinding
//	chainCB.setBrightness(200);  // Adjust as necessary to avoid blinding

  for (int i = 0;i < NUM_LEGS; i++)
  {
    chainLEG[i].setBrightness(255);
  }
 
}

void loop() 
{
  // Checks if ritual should happen
  timeRitualCheck = millis();
  if ((abs(timeRitualCheck - lastRitualCheck)) > interval_ritual)
  {
    if (cur_ritual_num < max_ritual) //rital is possible, start counting form now
    {
      if (cur_ritual_num == 0) //save the first ritual timestamp to be used on 10 min counter
        lastRitualTimestamp = timeRitualCheck;
      cur_ritual_num++;

      // if start a ritual command has been given and there is no other ritual being processed start it
      //inProcessRitual is being reset-ed at the end of the ritual
      if (startRitual and inProcessRitual == false and state_machine == 0)
      {
        inProcessRitual = true;
        startRitual = false; //ready for the next ritual
        //do ritual
        state_machine = 1;
      }
    }

    // if enough time passed since the first ritual reset thte timer
    if ((abs(timeRitualCheck - lastRitualTimestamp)) > max_ritual_period) 
      cur_ritual_num = 0;
    // save tim for the next loop-check
    lastRitualCheck = millis();
  }

  //read US sensors
  timeUltrasonic = millis();
  // every loop check if the time is right to read from the next sensor
  if ((abs(timeUltrasonic - lastUltrasonic)) > interval_timeUltrasonic)
  {
    cm[iSensorCycle] = sonar[iSensorCycle].ping_cm();
    if (cm[iSensorCycle] < detection_dis and cm[iSensorCycle] != 0 and sen_trig[iSensorCycle]==false)
    {
      sen_trig[iSensorCycle] = true;
    }
    else if ((sen_trig[iSensorCycle]==true) and (cm[iSensorCycle] > detection_dis or cm[iSensorCycle] == 0))
    {
      sen_trig[iSensorCycle] = false;
    }
    
    iSensorCycle++;
    iSensorCycle = iSensorCycle % SONAR_NUM;

    //check if all sensors are triggered and fade in completed. if so - start the ritual
    for (int k = 0; k < SONAR_NUM; k++)
      if (sen_trig[k] and stage_fade[k] > 250)
        sumSenors++;
      else
        sumSenors =0;

    if (sumSenors == SONAR_NUM)
    {
      startRitual = true;
    }
    
    for (int k = 0; k < SONAR_NUM; k++)
    {
      Serial.print("sen_num:");
      Serial.print(k);
      Serial.print(" sen cm:");
      Serial.print(cm[k]);
      Serial.print(" sen_trig:");
      Serial.print(sen_trig[k]);
      Serial.print(" sen_fade:");
      Serial.print(stage_fade[k]);
      Serial.print(" InProcessRitual:");
      Serial.print(inProcessRitual);
      Serial.print(" startRitual:");
      Serial.print(startRitual);
      Serial.print(" StateMachine:");
      Serial.println(state_machine);
    }
    
    Serial.println();
    Serial.println();
    
    lastUltrasonic = millis();
  }

  

  //buttonReading();
	
  
  timeStateMachine = millis();
  if ((abs(timeStateMachine - lastStateMachine)) > interval_StateMachine)
  {
    switch (state_machine) {
      
    case 0: //idle
      Serial.println("idle");
      //leafs command
      colorWipeLeaf(0,ledsPerStrip,128+25*cos(2*PI/12000*(3000-timeStateMachine)),0,128+127*cos(2*PI/12000*(3000-timeStateMachine)),0);
      //legs command
      idle_pulseLegs();
      //core command
      
      //Fade in/out stage lights
      timeStageLights = millis();
      if ((abs(timeStageLights - lastStageLights)) > interval_StageLights)
      {
        //create values for fade in-out for the pingpong and the stage lights
        for (int i; i < SidesNum; i++)
        {
          //fade in
          if ((stage_fade[i] < 255) and sen_trig[i] == true)
            stage_fade[i]++;
          //fade out
          if ((stage_fade[i] > 0) and sen_trig[i] == false)
            stage_fade[i]--;  
    
          
          //stage light control
          colorWipeStage((i*LedsNumSide),((i+1)*LedsNumSide),stage_fade[i],0,0,10);
          //pingpong control
          //creates a fast strong blink on the first interaction and then gose to full 
          int pingpong_signal = 0;
          if (stage_fade[i] >= 0 and stage_fade[i] <= 40)
            pingpong_signal = map(stage_fade[i],0,40,0,100);
          else
            pingpong_signal = map(stage_fade[i],51,255,101,255);
          
          colorWipePingPong(i,i+1,pingpong_signal,0,pingpong_signal);
          
        }
        lastStageLights = millis();
      }

      //Fade in/out flowers
      timeFlowers = millis();
      if ((abs(timeFlowers - lastFlowers)) > interval_timeFlowers)
      {
        colorWipeFlowers(0,NUM_FLOWERS,40+25*cos(2*PI/24000*(3000-timeFlowers)),25,128+127*cos(2*PI/24000*(3000-timeFlowers)),0);
        lastFlowers = millis();
      }

      break;
     
    case 1: //ritual
      Serial.println("Ritual");
      //leafs command
      colorWipeLeaf(0,ledsPerStrip,128+25*cos(2*PI/6000*(3000-timeStateMachine)),0,128+127*cos(2*PI/6000*(3000-timeStateMachine)),0);
      //legs command
      ritual_pulseLegs();
      //core command
      //cycle ping pong
            
      timeChasePongs = millis();
      if ((abs(timeChasePongs - lastChasePongs)) > interval_ChasePongs)
      {
          //pingpong control
          if (iPongChase == 0)
          {
            colorWipePingPong(0,1,255,0,128);
            colorWipePingPong(5,6,0,0,0);
          }
          else
          {
            colorWipePingPong(iPongChase,iPongChase+1,255,0,128);
            colorWipePingPong(iPongChase-1,iPongChase,0,0,0);
          }   
        
        iPongChase++;
        iPongChase % SONAR_NUM;
        lastChasePongs = millis();
      }
      //solid stage lights
      colorWipeStage(0,(LedsNumSide * SidesNum),255,255,0,10);
      break;
    case 2: //cooldown
      Serial.println("Cooldown");
      //leafs command
      colorWipeLeaf(0,ledsPerStrip,40+25*cos(2*PI/24000*(3000-timeStateMachine)),25,128+127*cos(2*PI/24000*(3000-timeStateMachine)),0);
      //legs command
      cooldown_pulseLegs();
      //core command
      //stage light control
      colorWipeStage(0,(LedsNumSide * SidesNum),0,0,220,10);
      //pingpong control
      colorWipePingPong(0,PingPongNum,0,0,220);
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




