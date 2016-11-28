
// Connects to A4 and A5 of Uno for I2C communication

void compassRead(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = 0.22;
  heading = (heading + declinationAngle)*(180/M_PI);
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 360;
    
  // Check for wrap due to addition of declination.
  if(heading > 360)
    heading -= 360;
   
  //Serial.print("Heading (degrees): "); Serial.println(heading);
  
  delay(500);
}
