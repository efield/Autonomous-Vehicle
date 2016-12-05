
// Pings ultrasonic sensors and determines average value of measurements
// Pings at rate set with US_PingRate, rate in milliseconds

#define NUM_READINGS 5

void ping_US(int &dist_US1, int &dist_US2, int &dist_US3, int US_PingRate)
{
  dist_US1 = (Ultrasonic_1.ping_median(NUM_READINGS))/US_ROUNDTRIP_CM;
  dist_US2 = (Ultrasonic_2.ping_median(NUM_READINGS))/US_ROUNDTRIP_CM;
  dist_US3 = (Ultrasonic_3.ping_median(NUM_READINGS))/US_ROUNDTRIP_CM;

  delay(US_PingRate);
}

// For debugging output from US sensors  
//  Serial.print(dist_US1);
//  Serial.print(", ");
//  Serial.println(dist_US2);
//  Serial.print(", ");
//  Serial.print(dist_US3);
//  Serial.print(", ");
//  Serial.println(dist_US4);
