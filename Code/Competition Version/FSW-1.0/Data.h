class Data
{
  public:

    Data();//constructor

    //Data storage related
    void updateOpenLog();
    void updateOpenLogTime();
    void updateOpenLogDeployed();
    void updateOpenLogTelemetry();
    void retrieveOpenLogTime();
    void retrieveOpenLogDeployed();
    void retrieveOpenLogTelemetry();
    void sendTelemetry();//sends telemetry data

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

    //Data update
    void updateDeployed();
    void updateTEAMID();
    void updateMissionTime();
    void updateAltitudeB();
    void updateOutTemp();
    void updateInTemp();
    void updateVolt();
    void updateFSWState();
    void updateAccelZ();
    void updateAccelY();
    void updateAccelX();

    //FlightStateDetermination
    int determineFS();

    //Other functions


  private:
    int TEAMID;
    int missiontime;
    int altitudeB;
    int outTemp;
    int inTemp;
    int volt;
    int fswState;
    int accelZ;
    int accelY;
    int accelX;
    int deployed
};