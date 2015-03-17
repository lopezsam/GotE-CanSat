/*
 Connect the following OpenLog to Arduino:
 RXI of OpenLog to pin 2 on the Arduino
 TXO to 3
 GRN to 4
 VCC to 5V
 GND to GND
 
 XBee Xplorer
 GND GND
 3V 3V
 DOUT D0
 DIN D1
 
 10 DOF IMU
 GND GND
 3V 3V
 SDA A4
 SCL A5
 
 TMP36
 3V 3V
 A2 
 A0
 GND GND
 
 GPS 
 5V 5V
 RX D8
 TX D9
 GND GND1
 
 *********************NOTE**************************
 Pre-FLight Checklist:
 1.File in SD card exists named T.csv
 2.File in SD card exists named D.csv with a single int value "0" in it.
*/

#include <SoftwareSerial.h>//OpenLog library
#include <Adafruit_Sensor.h>//Sensor general library
#include <SFE_BMP180.h>//BMP180 library
#include <Adafruit_LSM303_U.h>//Accel/Magneto sensor library
#include <Adafruit_L3GD20.h>//Gyroscope library
#include <Wire.h>
//-------------------------------------------------------------------------------------------
int resetOpenLog = 4;

const int TEXAS = ;//altitude of texas
const int resetOpenLog = 4;//D4 pin for OpenLog
const int TEAMID = 5761; //teamID
int time; //to be retrieved from SD card before start
int deployed;//retrieve from SD card before start 

int missionTime, altitudeB, outTemp, inTemp, volt, fswState, accel; //All mission/telemetry parameters are declared here
//-------------------------------------------------------------------------------------------
SFE_BMP180 pressure;
double baseline; // baseline pressure
//-------------------------------------------------------------------------------------------
// Define the pins for SPI
#define GYRO_CS 4 // labeled CS
#define GYRO_DO 5 // labeled SA0
#define GYRO_DI 6  // labeled SDA
#define GYRO_CLK 7 // labeled SCL

Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);

// No need to specify pins for I2C
//Adafruit_L3GD20 gyro();
//-------------------------------------------------------------------------------------------
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
//-------------------------------------------------------------------------------------------
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
//-------------------------------------------------------------------------------------------
SoftwareSerial OpenLog(3, 2);//OpenLog data object


void setup() {

  //SD Card Reader Setup
  setupOpenLog();
  gotoCommandMode();
  
  //retrive time
  missionTime = readFile('T');
  
  //retrieve deployed
  deployed = readFile('D');

  //Setup for the Pressure Sensor.
  //-------------------------------------------------------------------------------------------
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1); // Pause forever.
  }

  // Get the baseline pressure:

  baseline = getPressure();

  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");

  //Setup for the Gyroscope.
  //-------------------------------------------------------------------------------------------  
  // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) //Specifies 250 Degrees per Second.
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  else
  {
    Serial.println("L3DS20 init success");
  }

  //Setup for the Accelerometer.
  //-------------------------------------------------------------------------------------------
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  else
  {
    Serial.println("LSM303(Accel) init success");
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  //Setup for the Magnetometer.
  //-------------------------------------------------------------------------------------------
  Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  else
  {
    Serial.println("LSM303(Mag) init success");
  }
  //---------------------------------------------------------------------------------------------
  
  
  
  
  //determine flight state code
  //read altitude
  //read acceleration 
  if (deployed == 1 ){
   //between depending on altitude and acceleration 4-8
   
  }
  else
  {
   //between depending on altitude and acceleration 1-4
 
  }
  
}



void loop() {

  //Pressure retrieval Code
  //-------------------------------------------------------------------------------------------
  double a, P;

  // Get a new pressure reading:

  P = getPressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:

  a = pressure.altitude(P, baseline);

  Serial.print("relative altitude: ");
  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(a, 1);
  Serial.print(" meters, ");
  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(a * 3.28084, 0);
  Serial.println(" feet");
  
  //End of Pressure Retrieval Code.
  //-------------------------------------------------------------------------------------------
  
  //Temperature Retrieval Code.
  //-------------------------------------------------------------------------------------------
  double T;
  
  T = getTemperature();
  
  Serial.print("Temperature: ");
  if (T >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(T, 1);
  Serial.print(" degrees Celsius, ");
  if (T >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(T + 273.15, 0);
  Serial.println(" Kelvin");
  
  //End of Temperature Retrieval Code.
  //-------------------------------------------------------------------------------------------
  
  //Start of Gyroscope Code.
  //-------------------------------------------------------------------------------------------
  gyro.read();
  Serial.println("Gyroscope Readings in degrees per second:");
  Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
  
  //End of Gyroscope Code.
  //-------------------------------------------------------------------------------------------
  
  //Start of Accelerometer Code.
  //-------------------------------------------------------------------------------------------
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.println("Accelerometer Readings in m/s^2:");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  //End of Accelerometer Code.
  //-------------------------------------------------------------------------------------------
  
  //Start of Magnetometer Code.
  //-------------------------------------------------------------------------------------------
   mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.println("Magnetometer Readings in uT:");
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  delay(500);
  
  //End of Magnetometer Code.
  //-------------------------------------------------------------------------------------------
  
  //Start of OpenLog Code.
  //-------------------------------------------------------------------------------------------
  
 //**
  //**
  //** 
  //Replace this code here with actual numbers
  missionTime = missionTime + millis();
  altitudeB = ##;
  outTemp = ##;
  inTemp = ##;
  volt = ##;
  fswState = ##;
  accel = ##;
  
  
  //code to determine flight state
  if (altitudeB < 5 + Texas ){
    fswState = 0;
  }
  else if (altitudeB < 10 + Texas || accel
  
  //Start of XBee Code
  //XBee sending data code here
  //--------------------------------------------------------------------------------------
  
  Serial.println(TEAMID);
  Serial.println(",");
  Serial.println(missionTime);
  Serial.println(",");
  Serial.println(altitudeB);
  Serial.println(",");
  Serial.println(outTemp);
  Serial.println(",");
  Serial.println(inTemp);
  Serial.println(",");
  Serial.println(volt);
  Serial.println(",");
  Serial.println(fswState);
  Serial.println(",");
  Serial.println(accel);
  
   
  //Start of OpenLog Code.
  //-------------------------------------------------------------------------------------- 
  
  
  //Storing data in format to send through XBee
  OpenLog.print(TEAMID);
  OpenLog.print(",");
  OpenLog.print(missionTime);
  OpenLog.print(",");
  OpenLog.print(altitudeB);
  OpenLog.print(",");
  OpenLog.print(outTemp);
  OpenLog.print(",");
  OpenLog.print(inTemp);
  OpenLog.print(",");
  OpenLog.print(volt);
  OpenLog.print(",");
  OpenLog.print(fswState);
  OpenLog.print(",");
  OpenLog.print(accel);
  OpenLog.println();
  
  gotoCommandMode();
  appendOpenLogFile('D');
  OpenLog.print(2);
  OpenLog.println();
  gotoCommandMode();
  appendOpenLogFile('g');
  
  delay(500);  
}

void setupOpenLog(void) {
  pinMode(resetOpenLog, OUTPUT);
  OpenLog.begin(9600);

  //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') break;
  }
}

void createOpenLogFile(char filename) { //creates file using openlog
  OpenLog.print("new ");
  OpenLog.println(filename);


  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') break;
  }
  
  //End of OpenLog Code.
  //-------------------------------------------------------------------------------------------

}

void appendOpenLogFile(char filename) { //sets openlog file to append
  OpenLog.print("append ");
  OpenLog.print(filename);
  OpenLog.write(13);

  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '<') break;
  }

}

void gotoCommandMode(void) {//allows OpenLog to create folder, go to append mode
  //Send three control z to enter OpenLog command mode
  OpenLog.write(26);
  OpenLog.write(26);
  OpenLog.write(26);

  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '>') break;
  }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

double getPressure(){//Retrieves pressure data from the sensor.
  
  char status;
  double T, P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

double getTemperature(){//Retrieves temperature data from the sensor.

  char status;
  double T;
  
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
   
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
        {
          return (T);
        }
    else Serial.println("error retrieving pressure measurement\n");
  }
  else Serial.println("error starting pressure measurement\n");
}

