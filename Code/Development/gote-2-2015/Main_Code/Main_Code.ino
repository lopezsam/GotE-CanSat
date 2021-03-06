/*
 Connect the following OpenLog to Arduino:
 RXI of OpenLog to pin 2 on the Arduino
 TXO to 3
 GRN to 4
 VCC to 5V
 GND to GND
*/

#include <SoftwareSerial.h>//OpenLog library
#include <SFE_BMP180.h>//BMP180 library
#include <Wire.h>

int resetOpenLog = 4;
SFE_BMP180 pressure;
double baseline; // baseline pressure


SoftwareSerial OpenLog(3, 2);//OpenLog data object


void setup() {


  //SD Card Reader Setup
  setupOpenLog();
  gotoCommandMode();
  char fileName =  'g' ;//filename = gote.csvd

  //check whether file is made
  OpenLog.write("ls\r");//
  if (OpenLog.read() == '>') { //empty card
    createOpenLogFile(fileName);
    OpenLog.print("Temperature Data");//Setting titles
    OpenLog.print(",");
    OpenLog.print("Number");
    OpenLog.println();
  }
  else//file already made
    appendOpenLogFile(fileName);

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

}
void loop() {

  //Pressure retrieval Code
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
  
  //Temperature Retrieval Code.
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
  
  OpenLog.print(1);//Put temperature Data here
  OpenLog.print(",");
  OpenLog.print(2);//Put something else here
  OpenLog.println();
  
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
