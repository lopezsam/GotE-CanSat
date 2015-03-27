#include <SoftwareSerial.h>//OpenLog library
#include <Adafruit_Sensor.h>//Sensor general library
#include <SFE_BMP180.h>//BMP180 library
#include <Adafruit_LSM303_U.h>//Accel/Magneto sensor library
#include <Adafruit_L3GD20.h>//Gyroscope library
#include <Wire.h>

const int resetOpenLog = 4;
const int TEXAS = 2 ;//altitude of texas
int x = 1;
SoftwareSerial OpenLog(3, 2);//OpenLog data object
char *fileName = "telem.csv";
char *fileName2 = "deployed.txt";
char *fileName3 = "time.txt";

void setup() {
 

  //Serial.print("Hello");
  //OPENLOG SETUP
  //******************************************************************************************
  //SD Card Reader Setup
  setupOpenLog();
  gotoCommandMode();
  createOpenLogFile(fileName);
  
  setupOpenLog();
  gotoCommandMode();
  createOpenLogFile(fileName2);
  
  setupOpenLog();
  gotoCommandMode();
  createOpenLogFile(fileName3);
  setupOpenLog();
  gotoCommandMode();
 // char* missionTime = readFile(filename3);
  
  //retrieve deployed
  //char* deployed = readFile(filename2);
  
}

void loop() {
	//Start of OpenLog Code.
  //-------------------------------------------------------------------------------------- 
  
  //Storing data in format to send through XBee
  appendOpenLogFile(fileName);
  OpenLog.print(5713);
  OpenLog.print(",");
  /*OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);
  OpenLog.print(",");
  OpenLog.print(x);*/
  OpenLog.println();
  
  gotoCommandMode();
  appendOpenLogFile(fileName2);
  OpenLog.print(2);
  OpenLog.println();
  gotoCommandMode();
  x++;
   


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

void createOpenLogFile(char *fileName) {

  //Old way
  OpenLog.print("new ");
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r

  //New way
  //OpenLog.print("new ");
  //OpenLog.println(filename); //regular println works with OpenLog v2.51 and above

  //Wait for OpenLog to return to waiting for a command
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '>') break;
  }
/*
  OpenLog.print("append ");
  OpenLog.print(fileName);
  OpenLog.write(13); //This is \r

  //Wait for OpenLog to indicate file is open and ready for writing
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
*/
  //OpenLog is now waiting for characters and will record them to the new file  
}

void appendOpenLogFile(char *filename) { //sets openlog file to append
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
char* readFile(char *filename) {

  
  OpenLog.print("read ");
  OpenLog.println(filename); //regular println works with OpenLog v2.51 and above

  //The OpenLog echos the commands we send it by default so we have 'read log823.txt\r' sitting 
  //in the RX buffer. Let's try to not print this.
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '\r') break;
  }  

  //This will listen for characters coming from OpenLog and print them to the terminal
  //This relies heavily on the SoftSerial buffer not overrunning. This will probably not work
  //above 38400bps.
  //This loop will stop listening after 1 second of no characters received
  for(int timeOut = 0 ; timeOut < 1000 ; timeOut++) {
    while(OpenLog.available()) {
      char tempString[100];
      
      int spot = 0;
      while(OpenLog.available()) {
        tempString[spot++] = OpenLog.read();
        if(spot > 98) break;
      }
      tempString[spot] = '\0';
      timeOut = 0;
      return tempString;
    }
      
    delay(1);
  }

  //This is not perfect. The above loop will print the '.'s from the log file. These are the two escape characters
  //recorded before the third escape character is seen.
  //It will also print the '>' character. This is the OpenLog telling us it is done reading the file.  

  //This function leaves OpenLog in command mode
}

