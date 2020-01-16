// Functions for pulling data from sensors

void tmpUpdate() {

  // Turn on sensor to start measurements
  sensorTMP.wakeup();

  temperature = sensorTMP.readTempC();    // read the temperature in celsius

  alertPinState = digitalRead(TMP_ALERT_PIN); // read the Alert from pin
  alertRegisterState = sensorTMP.alert();   // read the Alert from register

  // Turn off sensor to conserve power
  sensorTMP.sleep();
  
}

void imuUpdate() {

  // Pull data from the sensors

  if(imu.gyroAvailable()) {
    imu.readGyro(); 
  }
  
  if(imu.accelAvailable()) {
    imu.readAccel(); 
  }
  
  if(imu.magAvailable()) {
    imu.readMag(); 
  }


  // Store processed data in variables
  
  // Magnetometer values
  magX = imu.calcMag(imu.mx);
  magY = imu.calcMag(imu.my);
  magZ = imu.calcMag(imu.mz);

  // Accelerometer values
  accX = imu.calcAccel(imu.ax);
  accY = imu.calcAccel(imu.ay);
  accZ = imu.calcAccel(imu.az);

  // Magnetometer values
  gyroX = imu.calcMag(imu.gx);
  gyroY = imu.calcMag(imu.gy);
  gyroZ = imu.calcMag(imu.gz);


  // Calculate heading, pitch, and roll

  roll = atan2(imu.ay, imu.az);
  pitch = atan2(-imu.ax, sqrt(imu.ay * imu.ay + imu.az * imu.az));

  if (imu.my == 0)
    heading = (-imu.mx < 0) ? PI: 0;
  else
    heading = atan2(-imu.mx, -imu.my);

  heading -= declination ;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
}


void baroUpdate(){
  // Read raw values
  baroRawTemp = baro.readRawTemperature();
  baroRawPressure = baro.readRawPressure();

  // Read true temperature & Pressure
  baroRealTemperature = baro.readTemperature();
  baroRealPressure = baro.readPressure();

  // Calculate altitude
  baroAbsoluteAltitude = baro.getAltitude(baroRealPressure, seaLevelPressure);
  baroRelativeAltitude = baro.getAltitude(baroRealPressure, baroReferencePressure); 
}


void gpsUpdate(){
  // Update GPS telemetry variables
  longitude = GPS.getLon();
  latitude = GPS.getLat();
  gpsAltitude = GPS.getAlt_feet();
  sats= GPS.getSats();
 
}
