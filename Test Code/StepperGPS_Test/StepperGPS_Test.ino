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

// Data storage
cur_data_t currentData; // creates struct for holding current data
sp_data_t setpointData; // creates struct for holding calculated setpoint data

// Waypoints
float waypoints[10][2]; // stores gps waypoints
byte currentWP = 0; // index for which waypoint system is on

// debugging
boolean debugSensors = true;
boolean debugPID = false;

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
  Serial.println("Initializing!");

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
  
  gpsData();
  compassRead();
  //Serial.println("Compass Read");
  runSteppers();
  //Serial.println("Running Steppers");
}




