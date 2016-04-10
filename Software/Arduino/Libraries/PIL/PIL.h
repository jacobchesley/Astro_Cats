#include <arduino.h>

#include "../DHTLib/dht.h"
#include "../BMP180/SFE_BMP180.h"
#include "../ADXL345/ADXL345.h"
#include "../UV/ML8511.h"
#include "../Pyranometer/SP215.h"
#include "../GPS/Venus638.h"
#include "../ArduinoJson/include/ArduinoJson.h"
#include "../RadioAPI/RadioAPI.h"
#include "../RadioINIT/RadioINIT.h"
#include "../Camera/Camera.h"
#include "../Servo/src/Servo.h"

struct PILData{

	double Temperature;
	double Humidity;

	double UV;
	double Solar;

	double Pressure;
	double PressureTemp;

	double AccelerationX;
	double AccelerationY;
	double AccelerationZ;

	GPSData gpsData;
};

class PIL{
public:
	PIL();

	void Loop();

	void CreateJSONFromPILData(PILData pilData);
	void CreatePILDataFromJson();

	void UpdateTempHumidity();
	void UpdatePressure();
	void UpdateAcceleration();
	void UpdateUV();
	void UpdateSolar();
	void UpdateGPS();
	void TakeAndStorePhoto();

	void UpdateAll();

	PILData GetPilData();
	void SendPilData();

private:

	void ParseMessage(String message);
	void ExecuteCommand(String commandName, String commandValue);
	void SendMessageConfirmation(String commandName, String response);
	byte GetNextFrameID();

	void OpenParachute();
	void CloseParachute();

	dht tempHumidity = dht(26);
	SFE_BMP180 pressure = SFE_BMP180();
	Adafruit_ADXL345_Unified accelerometer = Adafruit_ADXL345_Unified(12345);
	ML8511  uv = ML8511(0, 1);
	SP215 solar = SP215(2);
	Venus638 gps = Venus638(&Serial2);

	PILData pilData;

	RadioAPI radio = RadioAPI(&Serial1, 22);
	RadioINIT radioAddresses = RadioINIT();
	int nextFrameID;

	long microsecondCount;
	long microsecondUpdate;

	File sdFile;

	Camera camera = Camera(&Serial3);

	Servo motor;
};