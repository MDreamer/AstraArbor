
void off_Legs()
{
  for (int i = 0;i < NUM_LEGS; i++)
    {
      chainLEG[i].setPixelColor(0, 0,0,0);
      chainLEG[i].setPixelColor(1, 0,0,0);
      chainLEG[i].setPixelColor(2, 0,0,0);
      chainLEG[i].show();
    }
}

void pulseLeg(Adafruit_Pixie &chainLEG_in)
{ 
  long time1 = millis();
  chainLEG_in.setPixelColor(0, 128+25*cos(2*PI/12000*(3000-time1)),0,128+127*cos(2*PI/6000*(3000-time1)));
  chainLEG_in.setPixelColor(1, 128+25*cos(2*PI/12000*(2000-time1)),0,128+127*cos(2*PI/6000*(2000-time1)));
  chainLEG_in.setPixelColor(2, 128+25*cos(2*PI/12000*(1000-time1)),0,128+127*cos(2*PI/6000*(1000-time1)));      
  chainLEG_in.show();
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

// Climex blue
void fast_pulseLegs()
{
timePulse_legs = millis();
  if ((abs(timePulse_legs - lastCheck_legs)) > interval_del_legs)
  {
      
    long time1 = millis();
    for (int i = 0;i < NUM_LEGS; i++)
    {
      chainLEG[i].setPixelColor(0, 0,0,128+127*cos(2*PI/period_flash*(time1)));
      chainLEG[i].setPixelColor(1, 0,0,128+127*cos(2*PI/period_flash*(170-time1)));
      chainLEG[i].setPixelColor(2, 0,0,128+127*cos(2*PI/period_flash*(340-time1)));
      chainLEG[i].show();
    }
    if (period_flash > 7500)
      period_flash = period_flash - interval_del_legs;
    else if (period_flash > 2500 and period_flash < 7500)
      period_flash = period_flash - interval_del_legs/2;
    else if (period_flash < 2500)
      period_flash = period_flash - interval_del_legs/4;
    if (period_flash <= 500)
    {
      period_flash = 10000;
      leg_state_machine = 1;
    }
    
    lastCheck_legs = millis();
   }
}
