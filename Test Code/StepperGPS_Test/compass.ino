void compassRead() {
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);

  currentData.heading = (atan2(event.magnetic.y, event.magnetic.x)+declinationAngle)*(180/PI);
  
  // Correct for when signs are reversed.
  if(currentData.heading < 0)
    currentData.heading += 360;
    
  // Check for wrap due to addition of declination.
  if(currentData.heading > 360)
    currentData.heading -= 360;

  if(debugSensors) {
  Serial.print("Heading: "); Serial.println(currentData.heading);
  }
}
