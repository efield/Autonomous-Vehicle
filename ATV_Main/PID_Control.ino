
#define kp 1.0
#define ki 0.1
#define kd 1.0
#define ms_perSec 1000
#define steps_perRot 400 // 0.9 deg per step
#define defaultDelayFast 1500 //micro seconds
#define defaultDelaySlow 5000 //micro seconds

//pid control for steppers between current location and setpoint location
void pidControl(int integral, int previous_error) {
 
  byte turn_dir = 9; // 9=straight 0=left 1=right  
  unsigned long timer2 = millis(); // done once per setpoint
  int stepSpeed;
  
  setpointData.heading = atan2(sin(setpointData.longitude-currentData.latitude)*cos(setpointData.latitude),
                           ((cos(currentData.latitude)*sin(setpointData.latitude))-sin(currentData.latitude)*
                           cos(setpointData.longitude)*cos(setpointData.latitude-currentData.latitude)));
  float error = calcHeadingError(&turn_dir);
  
  if(debugPID) {
     Serial.print("Setpoint Heading: ");
     Serial.println(setpointData.heading);
     Serial.print("Current Heading: ");
     Serial.println(currentData.heading);
     Serial.print("Error: ");
     Serial.println(error);
     Serial.print("Turn Direction: ");
     Serial.print(turn_dir);
  }    
    
  if(turn_dir!=9) {
    stepSpeed = pidCalc(timer2,integral,previous_error,error);
    if(debugPID) {
      Serial.print("Step delay: ");
      Serial.println(stepSpeed);
    setSteppersTurn(stepSpeed,turn_dir); // drive with turn function
    }
  }
  
  else {
     driveSteppers(defaultDelayFast, defaultDelayFast); //motor 1_3 & 2_4 same speed
     if(debugPID) {
       Serial.print("Step delay default fast: ");
       Serial.println(defaultDelayFast);
     }
  }
}

float calcHeadingError(byte *turn_dir) {
    
  float headingError = setpointData.heading - currentData.heading;
        // error range is -180 - 180
        if(headingError<180) headingError+=360;
        if(headingError>360) headingError-+360;
        
        // allows for plus/minus 2 degree compass error
        // if -2<x<2 remains 9 => motors have same speed
        if (headingError<-2) *turn_dir = 0; // left
        else if(headingError>2) *turn_dir = 1; // right
        
        // takes absolute value used for pid calculation
        // range of error is now 0 to 180
        headingError = abs(headingError);
        
        return headingError;
}

// calculates the motor output
int pidCalc(unsigned long timer2, int integral, float previous_error, float error) {
    
    int dt = millis()-timer2; // current time - when pid was started
    int derivative=0,output=0; // reset each calculation
    
    integral = integral + error*dt;
    if(dt==0) derivative = 0;
    
    derivative = (error-previous_error)/dt;
    
    output = (kp*error + ki*integral + kd*derivative);
    previous_error=error;
    
    // calculation for what max output should be based on gain settings
    // may need tweaking
    int rangeConstraint = (180*kp + 180*ki + 180*kd);
    
    if(debugPID) {
      Serial.print("PID Range Constraint: ");
      Serial.println(rangeConstraint);
    }
    
    constrain(output,0,rangeConstraint); // these values may need to be tuned once tested and pid data available
    map(output,0,rangeConstraint,defaultDelaySlow,defaultDelayFast); // maps the output to a delay in microseconds
    return output;
}

  
// takes pid output and cotrols speed of stepepr motors
void setSteppersTurn(int stepSpeed, byte turn_dir) {
           
  //Left
  if(turn_dir==0) {
    // motor 2_4 faster 1_3
    driveSteppers(defaultDelayFast,stepSpeed);
  } 
            
  //Right
  else {
     // motor 1_3 faster 2_4
     driveSteppers(stepSpeed,defaultDelayFast);
  }
}
