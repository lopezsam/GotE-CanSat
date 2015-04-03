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

void updateAltitudeB(int baselineP){ //altitude determined by barometric sensor
	char status;
	double T, P, a;

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
	        a = pressure.altitude(P, baselineP);
	        return (a);
	      }
	    }
	  }
	}
}

void updateOutTemp();

void updateInTemp(){ //inside temperature
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
	  else Serial.println("error retrieving temperature measurement\n");
	}
	else Serial.println("error starting temperature measurement\n");
}

void updateVolt()
{
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
}

void updateFSWState();

void updateAccelZ(){//Acceleration Z
	sensors_event_t event; 
  	accel.getEvent(&event);
	return (event.acceleration.z);
} 

void updateAccelY(){ //acceleration Y
	sensors_event_t event; 
  	accel.getEvent(&event);
	return (event.acceleration.y);
}

void updateAccelX(){ //acceleration X
	sensors_event_t event; 
  	accel.getEvent(&event);
	return (event.acceleration.x);
}

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
void calibrateAllSensors(){//will do all commands below
	int baselineP
	baselineP = getPressure();
	return (baselineP);
} 
void calibrateGPS();
void calibrateAccel();
void calibrateInTemp();
void calibrateOutTemp();

void sensorTesting(){ //status of the sensors
	int i=0;
  //BMP180 Sensor (Altitude)
	if (pressure.begin())
	{
    	Serial.println("BMP180(Altitude) init success");
    	i++;
    }
	else
	{
  		Serial.println("BMP180(Altitude) init fail \n\n");
  	}

  //L3DS20 (Gyroscope)
  	if (gyro.begin(gyro.L3DS20_RANGE_250DPS)) //Specifies 250 Degrees per Second.
  	{
    	Serial.println("L3DS20(Gyroscope) init success");
    	i++;
  	}
  	else
  	{
    	Serial.println("L3GD20(Gyroscope) init fail \n\n");

  	}
  //LSM303 (Accelerometer)
  	if(accel.begin())
  	{
    	Serial.println("LSM303(Accel) init success");
    	i++;
  	}
  	else
  	{
    	Serial.println("LSM303(Accel) init fail \n\n");
  	}

  	Serial.println(i);
  	Serial.println("/3 sensors are functional.\n");
  	return(0);
}