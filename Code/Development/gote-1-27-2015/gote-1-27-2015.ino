/*
 Connect the following OpenLog to Arduino:
 RXI of OpenLog to pin D2 on the Arduino
 TXO to D3
 GRN to D4
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
*/

#include <SoftwareSerial.h>//OpenLog library


const int TEXAS = ;//altitude of texas
const int resetOpenLog = 4;//D4 pin for OpenLog
const int TEAMID = 5761; //teamID
int time; //to be retrieved from SD card before start
int deployed;//retrieve from SD card before start 

int missionTime, altitudeB, outTemp, inTemp, volt, fswState, accel; //All mission/telemetry parameters are declared here

SoftwareSerial OpenLog(3, 2);//OpenLog data object 


void setup(){
  
  
  //SD Card Reader Setup
  setupOpenLog();
  gotoCommandMode();
  
  //retrive time
  missionTime = readFile('T');
  
  //retrieve deployed
  deployed = readFile('D');
  
  //determine flight state code
  //read altitude
  //read acceleration 
  if (deployed == 1 ){
   //between depending on altitude and acceleration 4-8
  }
  else
   //between depending on altitude and acceleration 1-4
}
  
    
  
  
  char fileName =  'g' ;//filename = gote.csvd
  
  //check whether file is made
  // file is ALREADY MADE. Make Sure only G and D are there.
  createOpenLogFile(fileName);
  appendOpenLogFile(fileName);
  
  //Titles for the CSV file
  OpenLog.print(TEAMID);
  OpenLog.print(",");
  OpenLog.print("MISSION TIME");
  OpenLog.print(",");
  OpenLog.print("ALT_SENSOR");
  OpenLog.print(",");
  OpenLog.print("OUTSIDE_TEMP");
  OpenLog.print(",");
  OpenLog.print("INSIDE_TEMP");
  OpenLog.print(",");
  OpenLog.print("VOLTAGE");
  OpenLog.print(",");
  OpenLog.print("FSW_STATE");
  OpenLog.print(",");
  OpenLog.print("BONUS");
  OpenLog.println();
  
  
  
}
void loop(){
  
  //SENSOR RETRIEVAL CODE HERE
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
  
  //XBee sending data code here
  
  Serial.println(TEAMID);
  Serial.println(",");
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
  
}

void setupOpenLog(void){
  pinMode(resetOpenLog, OUTPUT);
  OpenLog.begin(9600);

  //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
 }
}
void createOpenLogFile(char filename){//creates file using openlog
  OpenLog.print("new ");
  OpenLog.println(filename);
 
  
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
 
}

void appendOpenLogFile(char filename){//sets openlog file to append
  OpenLog.print("append ");
  OpenLog.print(filename);
  OpenLog.write(13);
  
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
  
}
int readFile(char fileName){//reads openlog file
  OpenLog.print("read ");
  OpenLog.println(fileName);
  char reading;
  
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '\r') break;
  }
  for(int timeOut = 0 ; timeOut < 1000; timeOut++) {
    while(OpenLog.available()) {
      reading = OpenLog.read();
    }
  }
  return reading;
}
  
  
void gotoCommandMode(void) {//allows OpenLog to create folder, go to append mode
  //Send three control z to enter OpenLog command mode
  OpenLog.write(26);
  OpenLog.write(26);
  OpenLog.write(26);

  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '>') break;
  }
}
