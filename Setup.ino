// Functions for sensor setups

void tmpSetup() {
  pinMode(TMP_ALERT_PIN,INPUT);  // Declare alertPin as an input
  sensorTMP.begin();  // Join I2C bus

  // Initialize sensorTMP settings
  // These settings are saved in the sensor, even if it loses power
  
  // set the number of consecutive faults before triggering alarm.
  // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
  sensorTMP.setFault(0);  // Trigger alarm immediately
  
  // set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
  sensorTMP.setAlertPolarity(1); // Active HIGH
  
  // set the sensor in Comparator Mode (0) or Interrupt Mode (1).
  sensorTMP.setAlertMode(0); // Comparator Mode.
  
  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensorTMP.setConversionRate(2);
  
  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensorTMP.setExtendedMode(0);

  //set T_HIGH, the upper limit to trigger the alert on
  sensorTMP.setHighTempC(29.4); // set T_HIGH in C
  
  //set T_LOW, the lower limit to shut turn off the alert
  sensorTMP.setLowTempC(26.67); // set T_LOW in C


  Serial.println("TMP102 setup successful...");
}


void imuSetup() {
  if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will \n"
                   "work for an out of the box LSM9DS1 \n" 
                   "Breakout, but may need to be modified \n" 
                   "if the board jumpers are.");

    while (1);
  }

  Serial.println("LSM9DS1 IMU setup successful...");
}


void baroSetup() {
  while(!baro.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  baroReferencePressure = baro.readPressure();                    // Get a reference pressure for relative altitude

  Serial.println("MS5611 barometer setup successful...");
}


void gpsSetup() {
  UBLOX_SERIAL.begin(UBLOX_BAUD);
  GPS.init();

  delay(50);
  if(GPS.setAirborne()){
    Serial.println("Airborne mode set.");
  }

  Serial.println("UBLOX NEO-M8N GPS setup successful...");
}


void baroCalibration(float pressure, float alt) {
  // inputs: pressure (PA), alt (feet)
  // Read a pressure value in order get an accurate sea level pressure value
  seaLevelPressure = baro.getSeaLevel(pressure, alt/METERS_TO_FEET);

  Serial.println("MS5611 calibrated...");
  
}
