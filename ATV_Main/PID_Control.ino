
#define kp 1 //Proportional gain
#define kd 1 //Derivative gain
#define ki 1 //Integral gain
#define ms_perSec 1000
#define steps_perRot 400
#define defaultDelayFast 1500 //micro seconds
#define defaultDelaySlow 5000 //micro seconds

// calculates the motor output

void pidControl(float AdafruitGPSdata->latitude, float AdafruitGPSdata->longitude, float setpoint_lat, float setpoint_long, float current_heading) {
  
  float setpoint_heading = atan2(sin(setpoint_long-AdafruitGPSdata->latitude)*cos(setpoint_lat),
                ((cos(AdafruitGPSdata->latitude)*sin(setpoint_lat))-sin(AdafruitGPSdata->latitude)*cos(setpoint_long)*cos(setpoint_lat-AdafruitGPSdata->latitude));
  
  void pidCalculation(setpoint_heading, current_heading, timer2) {
     
  float output=0,integral=0,derivative=0,previous_error=0;
  
  unsigned long timer2 = millis(); // done once per setpoint.
  
  while(AdafruitGPSdata->latitude != setpoint_lat && AdafruitGPSdata->longitude != setpoint_long) {
    byte turn_dir = 9;
    float error = setpoint_heading - current_heading; 
        // error range is -180 - 180
        if(error<180) error+=360;
        if(error>360) error-+360;
        
        // allows for plus/minus 2 degree compass error
        // if -2<x<2 remains 9 => motors have same speed
        if (error<-2) turn_dir = 0; // left
        if (error>2) turn_dir = 1; // right
        
        // takes absolute value used for pid calculation
        // range of error is now 0 to 180
        error = abs(error);
    
    float dt = millis()-timer2; // current time - when pid was started
    
    // to avoid divide by zero errors for derivative
    // dt shold never be zero other than the first calculation
    if(dt==0) dt=0;
        
    integral = integral + error*dt;
    derivative = (error-previous_error)/dt; // could run into a problem where you divide by zero if millis()=timer2 at this point
    output = kp*error + ki*integral + kd*derivative;
    previous_error=error;
    
    constrain(output,0,540);
    map(output,0,(180*kp+180*kd+180*ki),defaultDelaySlow,defaultDelayFast);
    
    setSteppers(output,turn_dir);
       
   }
  }
}            
}

// takes pid output and maps it to motor speed values
void setSteppers(int output, byte turn_dir) {
           
  //Left
  if(turn_dir==0) {
    // motor 2_4 faster 1_3
    driveSteppers(defaultDelay,output);
  } 
            
  //Right
  else if(turn_dir==1) {
     // motor 1_3 faster 2_4
     driveSteppers(output,defaultDelay);
  }
           
  //straight
  
  else {
     // motor 1_3 & 2_4 same speed
     driveSteppers(output, output);
  }
} 
