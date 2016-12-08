void gpsData() {

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another 
  }
  
  // date and time data
  currentData.hour = GPS.hour;
  currentData.minute = GPS.minute;
  currentData.seconds = GPS.seconds;
  currentData.milliseconds = GPS.milliseconds;
  currentData.fix = GPS.fix;
      
  if (GPS.fix) {
    // coordinate and satellite data
    currentData.latitude = GPS.latitudeDegrees;
    currentData.longitude = GPS.longitudeDegrees;
  }
  
  if(debugSensors) {
      Serial.print("\nTime: ");
      Serial.print(currentData.hour, DEC); Serial.print(':');
      Serial.print(currentData.minute, DEC); Serial.print(':');
      Serial.print(currentData.seconds, DEC); Serial.print('.');
      Serial.println(currentData.milliseconds);
      Serial.print("Fix: "); Serial.println(currentData.fix);
      Serial.print("Location: "); Serial.print(currentData.latitude, 4);
      Serial.print(", "); Serial.println(currentData.longitude, 4);
    }
}
