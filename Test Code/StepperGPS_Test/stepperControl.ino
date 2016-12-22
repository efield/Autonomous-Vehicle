void driveSteppers(int stepDelay2_4, int stepDelay1_3) {
  
  unsigned long timerS1_3=micros();
  unsigned long timerS2_4=micros();
  unsigned long timerDuration = millis();

  // runs motor for set time (stepperRunDuration) before taking next sensor reading
  while(millis()-timerDuration < stepperRunDuration) {
  
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

void S1_3High() {
  digitalWrite(stepperPin1_3,HIGH);
}

void S2_4High() {
  digitalWrite(stepperPin2_4,HIGH);
}
