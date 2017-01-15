//Dead code
/*
 * 
 * // leafs' state machine colors
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
    idle_pulseLegs();
    break;
  case 1:
    ritual_pulseLegs();
    break;
  case 2:
    off_Legs();
    break;
  }
 */
