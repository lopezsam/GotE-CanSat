
#include <SoftwareSerial.h>//OpenLog library
#include <Adafruit_Sensor.h>//Sensor general library
#include <SFE_BMP180.h>//BMP180 library
#include <Adafruit_LSM303_U.h>//Accel/Magneto sensor library
#include <Adafruit_L3GD20.h>//Gyroscope library
#include <Wire.h>
#include <Servo.h> 

class Data
{
  public:

    Data();//constructor

    //Data storage related
    void updateOpenLog();
    void updateOpenLogDeployed();
    void updateOpenLogTelemetry();
    void retrieveOpenLogDeployed();
    void retrieveOpenLogTelemetry();
    void sendTelemetry();//sends telemetry data
    //OpenLog stuff
    void setupOpenLog(void);
    void creatOpenLogFile(char *fileName);
    void appendOpenLogFile(char *filename);
    void gotoCommandMode(void);

    //Data retrieval
    int getDeployed();
    int getTEAMID();//returns stored TEAMID
    int getMissionTime();//returns stored mission time
    int getaltitudeB();//returns stored altitude determined by barometric sensor
    int getOutTemp(); // returns stored outside temperature
    int getInTemp(); //returns stored inside temperature
    int getVolt(); //returns stored battery voltage
    int getFSWState(); //returns stored FSW state
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
    void updateVolt();
    void updateFSWState();
    void updateAccelZ();
    void updateAccelY();
    void updateAccelX();
    void updateGPS();
    void updateTime();

    //FlightStateDetermination
    int determineFS();

    //Action functions
    void deploy();//deploys science vehicle
    void soundBuzzer();
    void SetupCheck();//retrieves things from files

    //Sensor Calibration
    void calibrateAllSensors(); //will do all commands below
    void calibrateGPS();
    void calibrateAccel();
    void calibrateInTemp();
    void calibrateOutTemp();
    void sensorTesting();
    
  private:
    int TEAMID;
    int missiontime;
    int altitudeB;
    int TexasOffset;
    int outTemp;
    int inTemp;
    int volt;
    int fswState;
    int accelZ;
    int accelY;
    int accelX;
    int Rdeployed; //cansat deployed from Rocket or not
    int SDeployed;
};