#include "Data.h"

int currentFS;// current flight state
Data GOTE(); //object that continas the most up-to-date data

void setup()
{
  //initial sensor
  GOTE.calibrateAllSensors();
  GOTE.updateAll();
  GOTE.determineFS();
}

void loop()
{ 
  currentFS = GOTE.getFSWState();
  switch (currentFS){//goes to correct loop based on right 
    case 1://PreFlight
    {
        
    }
    case 2://LaunchWait
    {

    }
    case 3://Ascent
    {

    }
    case 4://RocketDeployment
    {

    }
    case 5://Stabilization
    {

    }
    case 6://Descent
    {

    }
    case 7://Landed
    {

    }
    default://in case go out of bounds
    {
      GOTE.determineFS();//determine flight state
    }


  }
}


void OpenLogModeCommand(){
  setupOpenLog();
  gotoCommandMode();

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

  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '>') break;
  }

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

//TODO: NEED TO WORK ON
/*
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

}
*/
