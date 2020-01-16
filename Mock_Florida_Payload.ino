// This code is intended to be run off of a Teensy 3.5/3.6 and can communicate using an I2C bus with sensors recommended by Emory-Riddle University

// Libraries
#include <Wire.h>
#include "SparkFunTMP102.h"
#include <SparkFunLSM9DS1.h>
#include <MS5611.h>
#include <UbloxGPS.h>


// Pin Declarations
#define TMP_ALERT_PIN A1
#define UBLOX_SERIAL Serial4      // Any serial port is open...Serial4 was just chosen to make intial tests easier


// Constants
#define METERS_TO_FEET 3.28084
#define PASCALS_TO_ATM 9.86923E-6
#define NOFIX 0x00
#define FIX 0x01


// Timers
#define LOG_TIMER 4000            // log data every 4 seconds
#define CALIBRATION_TIMER 60000   // calibrate the MS5611 sea level pressure measurement a minute into the flight

unsigned long log_stamp = 0;


// I2C Address Declarations
#define TMP_ADDRESS 0x48
// #define UBLOX_ADDRESS 0x42


// TMP102 objects/variables
TMP102 sensorTMP(TMP_ADDRESS); // Initialize sensor at I2C address 0x48 (can be changed to different address with external jumpers...check documentation)
float temperature;
boolean alertPinState, alertRegisterState;  // This bools could be used as alerts if certain operating temperatures get too hot/cold

// In case of wiring error, TMP102 will return -0.06 C as a temp value


// LSM9DS1 IMU Declarations
LSM9DS1 imu;
float magX;     // magnetometer values
float magY;
float magZ;
float accX;     // acceleromter values
float accY;
float accZ;
float gyroX;    // gyroscopic values
float gyroY;
float gyroZ;
float heading;  // euler angles
float pitch;
float roll;
float declination = 0;   // declination angle in radians


// MS5611 Pressure Sensor Objects/Declarations
MS5611 baro;
double seaLevelPressure;        // in Pascals
double baroReferencePressure;   // some fun pressure/temperature readings below 
uint32_t baroRawTemp;           // non-"raw" readings are in Pa for pressure, C for temp, meters for altitude
uint32_t baroRawPressure;
double baroRealTemperature;
long baroRealPressure;
float baroAbsoluteAltitude;
float baroRelativeAltitude;
boolean baroCalibrated = false; // inidicates if the barometer has been calibrated or not  


// UBLOX NEO-M8N Objects/Declarations
UbloxGPS GPS(&UBLOX_SERIAL);    // unfortunately this runs off of UART serial
uint8_t fixStatus = NOFIX;      // byte that in later code versions will indicate if the GPS has a fix or not
float gpsAltitude;
float latitude;
float longitude;
int sats;

// Data string
String gyroData;                // these strings are separated in order to make the serial monitor print out more appealing to the eye
String accelData;
String magData;
String attitudeData;
String tempData;
String baroPressureData;
String baroTempData;
String baroCalculatedData;
String gpsData;
String data;                    // this string is a concatonation of all previous data strings and will be printed to the SD card


void setup() {
  Serial.begin(9600);
  Serial.println("Setup initiated...");

  Wire.begin();   // initialize the I2C communication lines

  tmpSetup();     // TMP102 temperature sensor setup sequence
  imuSetup();     // LSM9DS1 IMU setup sequence
  baroSetup();    // MS5611 barometer setup sequence
  gpsSetup();     // UBLOX NEO-M8N GPS setup sequence 

  Serial.println("Setup complete!");

}

void loop() {
  GPS.update();
  // setDeclination(); ...future function that sets the declination angle based off of latitude and longitude inputs

  if(millis() > CALIBRATION_TIMER && !baroCalibrated) {
    baroCalibration(baro.readPressure(), GPS.getAlt_feet()); // calibrates the MS5611 so it can more accurately calculte altitude
    baroCalibrated = true;                                   // indicate that the barometer has been calibrated
    
  }

  if(millis() - log_stamp > LOG_TIMER) {
    log_stamp = millis();

    // Update data from all sensors
    tmpUpdate();
    imuUpdate();
    baroUpdate();
    gpsUpdate();

    getDataStrings();         // populated the data strings
    printData();              // print the data to the serial monitor and in the future to the SD card
    
  }
  
}
