#include "Data.h"

Data::Data()
{
	int TEAMID = 5716;
	int missiontime = 0;
	int altitudeB = 0;
	int TexasOffset = //FIND OUT;
	int outTemp = 0;
	int inTemp = 0;
	int volt = 0;
	int fswState = 0;
	int accelZ = 0;
	int accelY = 0;
	int accelX = 0;
	int Rdeployed = 0; //cansat deployed from Rocket or not
	int SDeployed = 0;
}


//Data storage related
void Data::updateOpenLog()
{

}

void Data::updateOpenLogDeployed()
{

}

void Data::updateOpenLogTelemetry()
{

}

void Data::retrieveOpenLogDeployed()
{

}

void Data::retrieveOpenLogTelemetry()
{

}

void Data::sendTelemetry()//sends telemetry data
{

}

//OpenLog stuff
void setupOpenLog(void)
{

}

void creatOpenLogFile(char *fileName)
{

}

void appendOpenLogFile(char *filename)
{

}

void gotoCommandMode(void)
{

}


//Data retrieval
int getDeployed();
int getTEAMID();//returns stored TEAMID
int getMissionTime();//returns stored mission time
int getaltitudeB();//returns stored altitude determined by barometric sensor
int getOutTemp(); // returns stored outside temperature
int getInTemp(); //returns stored inside temperature
int getVolt(); //returns stored battery voltage

int Data::getFSWState() //returns stored FSW state
{
	return fswState;
}

int getAccelZ(); //returns stored Acceleration Z
int getAccelY(); //returns stored acceleration Y
int getAccelX(); //returns stored acceleration X
int getGPS();
int getLastTime();

//Data update (i.e. read sensors)
void updateAll();
void updateDeployed();
void updateMissionTime();
void updateAltitudeB();
void updateOutTemp();
void updateInTemp();
void updateVolt()
{
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
}
void updateFSWState();
void updateAccelZ();
void updateAccelY();
void updateAccelX();
void updateGPS();
void updateTime();

//FlightStateDetermination
void Data::determineFS()//returns a number 0 - 7 (0 being undetermined)
{
	//PreFlight case
	if (altitudeB <= 5 && Rdeployed == false && (accelZ => -5 && accelZ <= 5))
		fswState = 1;

	//LaunchWait
	else if (altitudeB <= 10 && Rdeployed == false && (accelZ => -5 && accelZ <= 5))
		fswState = 2;

	//Ascent
	else if (altitudeB > 10 && Rdeployed == false && (accelZ => 10))
		fswState = 3;

	//RocketDeployment
	else if (altitudeB >= 650 && altitudeB <= 690 && Rdeployed == false && (accelZ => -2 && accelZ <= 2))
		fswState = 4;

	//Stabilization
	else if (altitudeB <= 690 && altitudeB >= 500 && Rdeployed == true && ( accelZ <= -5))
		fswState = 5;

	//Descent
	else if (altitudeB <= 500 && Rdeployed == true && (accelZ <= -5))
		fswState = 6;

	//Landed
	else if (altitudeB <= 5 && Rdeployed == true && (accelZ <= 1))
		fswState = 7;

	//Unknown
	else
		fswState = 0;
}

//Action functions
void Data::deploy()//deploys science vehicle
{

}

void soundBuzzer();

//Sensor Calibration
void calibrateAllSensors(); //will do all commands below
void calibrateGPS();
void calibrateAccel();
void calibrateInTemp();
void calibrateOutTemp();