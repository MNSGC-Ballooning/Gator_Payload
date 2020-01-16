// Functions for creating and printing data strings

void getDataStrings() {
  // Populate all of the data strings
  // temp sensor data
  tempData = String(temperature);

  // imu data
  accelData = String(accX) + ", " + String(accY) + ", " + String(accZ);
  magData = String(magX) + ", " + String(magY) + ", " + String(magZ);
  gyroData = String(gyroX) + ", " + String(gyroY) + ", " + String(gyroZ);
  attitudeData = String(heading) + ", " + String(pitch) + ", " + String(roll);

  // barometer data
  baroPressureData = String(baroRealPressure) + ", " + String(baroRealPressure * PASCALS_TO_ATM, 4);
  baroTempData = String(baroRealTemperature);
  baroCalculatedData = String(baroAbsoluteAltitude * METERS_TO_FEET);

  // gps data
  gpsData = String(latitude, 4) + ", " + String(longitude, 4) + ", " + String(gpsAltitude) + ", " + String(sats);
  
  
  data = String(millis()) + ", " + tempData + ", " + accelData + ", " + magData + ", " + gyroData + ", " + attitudeData;
  data += ", " + baroPressureData + ", " + baroTempData + ", " + baroCalculatedData + ", " + gpsData;

  
}

void printData() {
  // Print relevant data to the serial monitor
  Serial.print("TMP102 Temperature(C): \t\t\t");
  Serial.println(tempData);

  Serial.print("MS5611 Pressure (Pa) , (ATM) :\t\t"); 
  Serial.println(baroPressureData);

  Serial.print("MS5611 Temperature(C): \t\t\t");
  Serial.println(baroTempData);

  Serial.print("MS5611 Altitude(ft): \t\t\t");
  Serial.println(baroCalculatedData);

  Serial.print("Accelerometer x , y , z : \t\t");
  Serial.println(accelData);

  Serial.print("Magnetometer x , y , z : \t\t");
  Serial.println(magData);

  Serial.print("Gyroscope x , y , z : \t\t\t");
  Serial.println(gyroData);

  Serial.print("Heading , Pitch , Roll : \t\t");
  Serial.println(attitudeData);

  Serial.print("UBLOX Long. , Lat. , Alt.(ft) , Sats : \t");
  Serial.println(gpsData);

  Serial.print("\n\n");
}
