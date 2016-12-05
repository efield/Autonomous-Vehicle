
// Gets gps data and put into data array

void gpsData() // function prototype for struct
{
     if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }

    // if millis() or timer wraps around, we'll just reset it
    if (timer > millis())  timer = millis();

    // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) { 
      timer = millis(); // reset the timer
//      currentData.hour = GPS.hour;
//      currentData.minute = GPS.minute;
//      currentData.seconds = GPS.seconds;
//      currentData.milliseconds = GPS.milliseconds;
      currentData.fix = GPS.fixquality;
    
      if (GPS.fix) {
        currentData.latitude = GPS.latitudeDegrees;
        currentData.longitude = GPS.longitudeDegrees;
      }
    }
 }
