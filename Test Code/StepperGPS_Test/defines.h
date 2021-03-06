#ifndef defines_h
#define defines_h

#define stepperPin1_3 6 // digital pin 6
#define stepperPin2_4 5 // digital pin 5
#define fullStepPin 2   // digital pin 2

//#define stepDelay1_3 1500 //micro seconds
//#define stepDelay2_4 5000 //micro seconds


// time steppers are run for before new sensor readings taken
#define stepperRunDuration 1000

#define declinationAngle -0.22 // Ottawa=0.22  Calgary=-0.22

#define kp 5.0 // proportional gain
#define ki 0.00 // integral gain
#define kd 1.0 // derivative gain
#define ms_perSec 1000 // const
#define steps_perRot 400 // 0.9 deg per step

#define defaultDelayFast 1250 //micro seconds
#define defaultDelaySlow 5000 //micro seconds

#endif
