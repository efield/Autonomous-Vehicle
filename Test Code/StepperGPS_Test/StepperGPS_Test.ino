/***********************************************************************************
  WRITTEN BY: ERIC FIELD
  DATE: 8-DEC-2016
  PROJECT: AUTONOMOUS ALL TERRAIN VEHICLE
***********************************************************************************/

/***********************************************************************************
                                 LIBRARIES
***********************************************************************************/
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "defines.h"
#include "dataTypes.h"

/***********************************************************************************
                              INITIALIZATIONS
***********************************************************************************/
// Program mode
boolean basicTest = false;

// Data storage
cur_data_t currentData; // creates struct for holding current data
sp_data_t setpointData; // creates struct for holding calculated setpoint data
pid_data_t pidData;

// Waypoints
float waypoints[10][2]; // stores gps waypoints
byte currentWP = 0; // index for which waypoint system is on

// debugging
boolean debug = false; // debugs main loop
boolean debugSensors = false; // debugs sensor readings
boolean debugPID = true; // debugs PID calculations

// Compass
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// GPS
SoftwareSerial mySerial(8,7);
Adafruit_GPS GPS(&mySerial);
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

/***********************************************************************************
                                   SETUP
***********************************************************************************/

void setup()  {
  // 115200 so GPS can be read fast enough without dropping chars
  Serial.begin(115200);
  Serial.flush();
  Serial.println("Initializing");

  mag.begin(); // initialize compass
  
  // Stepper pinModes
  pinMode(stepperPin1_3, OUTPUT);
  pinMode(stepperPin2_4, OUTPUT);
  pinMode(fullStepPin, OUTPUT);
  
  // enables full stepping on steppers
  // all steppers connect to this common output
  digitalWrite(fullStepPin, LOW);

  // 9600 is the default baud rate for Adafruit GPS
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate 1hz = 1s
  
  useInterrupt(true);

  Serial.println("Connecting to satellites...");
  delay(1000);
  Serial.println("Satellites Aquired");
  Serial.println();
  delay(1000);
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop() {

if(basicTest) {
  gpsData();
  compassRead();
  driveSteppers(defaultDelayFast, defaultDelaySlow);
}

else {

  Serial.print("Heading to waypoint ");
  Serial.println(currentWP);
  Serial.println();

  waypoints[currentWP][0] = 45.3191;
  waypoints[currentWP][1] = -75.9330;

  gpsData(); // gets current GPS location and time (UTS)
  compassRead(); // gets current heading
  
  setpointData.latitude = waypoints[currentWP][0];
  setpointData.longitude = waypoints[currentWP][1];
  
  // initializations done once per setpoint
  unsigned long spTimeElapsed = millis();
  pidData.integral=0; 
  pidData.previousError=0;
  pidData.output=0;

  while(currentData.latitude!=setpointData.latitude && currentData.longitude!=setpointData.longitude) {
      gpsData(); // gets current GPS location and time (UTS)
      compassRead(); // gets current heading
      
      if(debug) {
        Serial.print("Current Latitude: ");
        Serial.println(currentData.latitude,4); // 4 decimal place precision
        Serial.print("Setpoint Latitude: ");
        Serial.println(setpointData.latitude,4);
        Serial.println();
        
        Serial.print("Current Longitude: ");
        Serial.println(currentData.longitude,4);
        Serial.print("Setpoint Longitude: ");
        Serial.println(setpointData.longitude,4);
        Serial.println();
        
        Serial.print("Current Heading: ");
        Serial.println(currentData.heading);
        Serial.print("Setpoint Heading: ");
        Serial.println(setpointData.heading);
        Serial.println();
      }
      
      pidControl(spTimeElapsed); //enables control system to drive motors until setpoint is reached
      //Serial.println("Motors Driving");
  }
  
  Serial.print("Reached waypoint");
  
  // if reached last WP, reset to first waypoint
  // THIS MEANS CLOSED LOOP PATH FOLLOWING
  if(currentWP==9) {
    currentWP=0;
  }
  
  else currentWP++; // set next waypoint
  }
}




