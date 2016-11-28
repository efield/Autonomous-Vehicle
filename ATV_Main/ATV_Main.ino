/***********************************************************************************
  WRITTEN BY: ERIC FIELD
  DATE: 21-NOV-2016
  PROJECT: AUTONOMOUS ALL TERRAIN VEHICLE
***********************************************************************************/


/***********************************************************************************
                                 LIBRARIES
***********************************************************************************/

#include <NewPing.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "MyTypes.h"

/***********************************************************************************
                              GPS INITIALIZATIONS
***********************************************************************************/
SoftwareSerial mySerial(8,7); // Ensure switch is set to SoftSerial
Adafruit_GPS GPS(&mySerial);

boolean usingInterrupt = false; // this keeps track of whether we're using the interrupt, interrupt starts off by default
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

gps_t AdafruitGPSdata; // creates
/***********************************************************************************
                        ULTRASONIC SENSOR INITIALIZATIONS
***********************************************************************************/

#define TRIGGER_PIN  12
#define ECHO_PIN_1   11
#define ECHO_PIN_2   10
#define ECHO_PIN_3   9
#define MAX_DISTANCE 200 //cm -> 2m

int US_PingRate = 1000; // Ultrasonic sensor ping rate in ms
int dist_US1, dist_US2, dist_US3; // distance measured by US sensors
 
NewPing Ultrasonic_1(TRIGGER_PIN, ECHO_PIN_1, MAX_DISTANCE);
NewPing Ultrasonic_2(TRIGGER_PIN, ECHO_PIN_2, MAX_DISTANCE);
NewPing Ultrasonic_3(TRIGGER_PIN, ECHO_PIN_3, MAX_DISTANCE);

/***********************************************************************************
                             STEPPER INITIALIZATIONS
***********************************************************************************/

#define stepperPin1_3  6
#define stepperPin2_4  4

#define stepperDirPin1_3  5
#define stepperDirPin2_4  3

#define fullStepPin 2

boolean direction1_3 = 0; // 0=FWD 1=REV
boolean direction2_4 = 0; // 0=FWD 1=REV

/***********************************************************************************
                             COMPASS INITIALIZATIONS
***********************************************************************************/

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

/***********************************************************************************
                                   SETUP
***********************************************************************************/

void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  Serial.begin(115200);
  Serial.flush();
  
  // Stepper pinModes
  pinMode(stepperPin1_3, OUTPUT);
  pinMode(stepperPin2_4, OUTPUT);
  pinMode(fullStepPin, OUTPUT);
  
  // enables full stepping on steppers
  // all steppers connect to this common output
  digitalWrite(fullStepPin, LOW);
  
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  useInterrupt(true);
  
  compassRead();

  delay(1000);
}

/***********************************************************************************
                                   GPS FUNCTIONS
***********************************************************************************/

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

/***********************************************************************************
                                     MAIN LOOP
***********************************************************************************/

void loop()
{ 
  gpsData(&AdafruitGPSdata); // uses struct
  ping_US(dist_US1, dist_US2, dist_US3, US_PingRate);
  //driveS1(speedS1);
}
