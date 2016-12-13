
//pid control for steppers between current location and setpoint location
void pidControl(unsigned long spTimeElapsed) {
 
  byte turn_dir = 9; // 9=straight 0=left 1=right 
  int stepSpeed;

  // calculate setpoint heading in radians, convert to degrees
  setpointData.heading = (atan2(sin(setpointData.longitude-currentData.latitude)*cos(setpointData.latitude),
                         ((cos(currentData.latitude)*sin(setpointData.latitude))-sin(currentData.latitude)*cos(setpointData.longitude)*
                         cos(setpointData.latitude-currentData.latitude))))*(180/PI);

  if(setpointData.heading < 0) {
    setpointData.heading +=360;
  }
  
  calcHeadingError(&turn_dir);
  
  if(debugPID) {
     Serial.print("Setpoint Heading: ");
     Serial.println(setpointData.heading);
     Serial.print("Current Heading: ");
     Serial.println(currentData.heading);
     Serial.print("Error: ");
     Serial.println(pidData.error);
     Serial.print("PID output: ");
     Serial.println(pidData.output);
     Serial.print("Turn Direction: ");
     Serial.println(turn_dir);
     Serial.println();
  }    
    
  if(turn_dir!=9) {
    pidCalc(spTimeElapsed);
    setSteppersTurn(pidData.output,turn_dir); // drive with turn function
  }
    if(debugPID) {
      Serial.println("Turning");
      Serial.print("Step delay: ");
      Serial.println(pidData.output);
    }
  
  else {
     driveSteppers(defaultDelayFast, defaultDelayFast); //motor 1_3 & 2_4 same speed
  }
  
  if(debugPID) {
    Serial.println("Driving Straight");
    Serial.println();
  }
}

void calcHeadingError(byte *turn_dir) {
    
  pidData.error = setpointData.heading - currentData.heading;
        
//        if(pidData.error<-360) { // 180
//          pidData.error+=360; // 360
//        }
//        
//        if(pidData.error>360) { // 360
//          pidData.error-=360; //360
//        }

        // if -2< pidData.error < 2 turn dir remains = 9 => drive striaght 
        if(pidData.error>180) {
          *turn_dir = 0;
          pidData.error -= 360; // sets to smaller angle between setpoint and current heading 
        }
        else if(pidData.error>2 && pidData.error<=180) {
          *turn_dir = 1;
        }
        else if(pidData.error<-2 && pidData.error>-180) {
          *turn_dir = 0;
        }
        else if(pidData.error<-180) {
          *turn_dir = 1;
          pidData.error += 360; // sets to smaller angle between sepoint and current heading
        }
        
//        // allows for plus/minus 2 degree compass error
//        // if -2<x<2 remains 9 => motors have same speed
//        if (pidData.error<-2) {
//          *turn_dir = 0; // left
//        }
//        else if(pidData.error>2) {
//          *turn_dir = 1; // right
//        }
        
        // takes absolute value used for pid calculation
        // range of error is now 0 to 360
        pidData.error = abs(pidData.error);
}

// calculates the motor output
void pidCalc(unsigned long spTimeElapsed) {

    pidData.output = 0; // reset to calculate each time
    
    unsigned long dt = (millis()-spTimeElapsed)/1000; // current time - when pid was started
    
    pidData.integral = pidData.integral + pidData.error*dt;
    
    if(dt==0) {
      pidData.derivative = 0;
    }
    else {
      pidData.derivative = (pidData.error-pidData.previousError)/dt;
    }
    
    pidData.output = (kp*pidData.error + ki*pidData.integral + kd*pidData.derivative);

    if(debugPID) {
      Serial.print("Unconstrained output: ");
      Serial.println(pidData.output);
    }
    
    pidData.previousError = pidData.error;
    
    // calculation for what max output should be based on gain settings
    // may need tweaking
    // calculated based on estimated angle changes (proportional can be off 180 max, 
    //max heading change of 30 for vehicle makes sense for derivative, inegral will have no impact
    int rangeConstraint = (180*kp + 10*kd); 
    
    if(debugPID) {
      Serial.print("PID Range Constraint: ");
      Serial.println(rangeConstraint);
    }
    
    pidData.output = constrain(pidData.output,0,rangeConstraint); // these values may need to be tuned once tested and pid data available
    
    if(debugPID) {
      Serial.print("Constrained output: ");
      Serial.println(pidData.output);
    }
    
    pidData.output = map(pidData.output, 0, rangeConstraint, defaultDelayFast, defaultDelaySlow); // maps the output to a delay in microseconds

    if(debugPID) {
      Serial.print("Mapped output: ");
      Serial.println(pidData.output);
      Serial.println();
    }
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
