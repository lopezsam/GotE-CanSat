/*
 Connect the following OpenLog to Arduino:
 RXI of OpenLog to pin 2 on the Arduino
 TXO to 3
 GRN to 4
 VCC to 5V
 GND to GND



*/

#include <SoftwareSerial.h>//OpenLog library

int resetOpenLog = 4;


SoftwareSerial OpenLog(3, 2);//OpenLog data object 


void setup(){
  
  
  //SD Card Reader Setup
  setupOpenLog();
  gotoCommandMode();
  char fileName = [ 'g', 'o', 't', 'e', '.', 'c', 's', 'v'];//filename = gote.csvd
  
  //check whether file is made
  OpenLog.write("ls\r");//
  if(OpenLog.read() == '>'){//empty card
    createOpenLogFile(fileName);
    OpenLog.print("Temperature Data");//Setting titles
    OpenLog.print(",");
    OpenLog.print("Number");
    OpenLog.println();
  }
  else//file already made
    appendOpenLogFile(fileName);
 
}
void loop(){
  
  //Place temperature retrieval Code HERE
  
  
  
  
  
  
  
  
  OpenLog.print(1);//Put temperature Data here
  OpenLog.print(",");
  OpenLog.print(2);//Put something else here
  OpenLog.println();
  
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

void createOpenLogFile(char* filename){//creates file using openlog
  OpenLog.print("new ");
  OpenLog.println(filename);
 
  
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
 
}

void appendOpenLogFile(char *filename){//sets openlog file to append
  OpenLog.print("append ");
  OpenLog.print(filename);
  OpenLog.write(13);
  
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
  
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
