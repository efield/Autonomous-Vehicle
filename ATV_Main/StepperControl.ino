
#define runTime 2000 // runs for 2 seconds


// used to return pins to high allows for both
// steppers to be run simultaneously
void S1_3High() {
  digitalWrite(stepperPin1_3,HIGH);
}

void S2_4High() {
  digitalWrite(stepperPin2_4,HIGH);
}

// steps motors simultaneously at defined speeds for duration of runTime
void driveSteppers(int stepDelay1_3, int stepDelay2_4)
{
  // tracks elapsed time between steps - micro seconds
  unsigned long timerS1_3=micros();
  unsigned long timerS2_4=micros();
  int runTimer=millis();
  
  while(runTimer<runTime) {
    
    //checks if time has elapsed to take next step steppers
    // if it has, step and reset timer. Else, do nothing.
    
    if((micros()-timerS1_3)>=stepDelay1_3) {
      digitalWrite(stepperPin1_3, LOW);
      timerS1_3=micros(); // reset timer
      S1_3High();
    }
    if((micros()-timerS2_4)>=stepDelay2_4) {
      digitalWrite(stepperPin2_4, LOW);
      timerS2_4=micros(); // reset timer
      S2_4High();
    }
  }
}

    
