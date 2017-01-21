#include "config.h"
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

//    chainCB.setPixelColor(0, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(3000-time2)));
//    chainCB.setPixelColor(1, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(2000-time2)));
//    chainCB.setPixelColor(2, 0,128+127*cos(2*PI/6000*(3000-time2)),128+63*cos(2*PI/6000*(1000-time2)));      
//    chainCB.show();
    
    lastCheck_core = millis();
   }
}


//is being used for the strips/leafs
void colorWipeStage(int startLED, int endLED, int colorR,int colorG,int colorB, int whiteC)
{
  //light just one one we need
  for (int i=startLED; i < endLED; i++)
  {
    stage_strip.setPixelColor(i, colorR,colorG,colorB,whiteC);
  }
  stage_strip.show();
}

//is being used for the pingpong
void colorWipePingPong(int startLED, int endLED, int colorR,int colorG,int colorB)
{
  //light just one one we need
  for (int i=startLED; i < endLED; i++)
  {
    ping_chain.setPixelColor(i, colorR,colorG,colorB);
  }
  ping_chain.show();
}

void colorWipeLeaf(int startLED, int endLED, int colorR,int colorG,int colorB, int whiteC)
{
  //light just one one we need
  for (int i=startLED; i < endLED; i++)
  {
    leaf_strip.setPixelColor(i, colorR,colorG,colorB,whiteC);
  }
  leaf_strip.show();
}

void colorWipeFlowers(int startLED, int endLED, int colorR,int colorG,int colorB, int whiteC)
{
  //light just one one we need
  for (int i=startLED; i < endLED; i++)
  {
    flower_chain.setPixelColor(i, colorR,colorG,colorB,whiteC);
  }
  flower_chain.show();
}
