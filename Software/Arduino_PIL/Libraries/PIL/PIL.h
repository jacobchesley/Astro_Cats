#include <arduino.h>

#include "../DHTLib/dht.h"
#include "../BMP180/SFE_BMP180.h"
#include "../Accelerometer/Accelerometer.h"
#include "../UV/ML8511.h"
#include "../GPS/Venus638.h"
#include "../ArduinoJson/include/ArduinoJson.h"

struct PILData{

	double Temperature;
	double Humidity;
	String TempHumidityError;


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
	PIL(int tempHumidPin);

	void CreateJSONFromPILData(PILData pilData);
	void CreatePILDataFromJson();

	void UpdateTempHumidity();
	void UpdatePressure();
	void UpdateAcceleration();
	void UpdateUV();
	void UpdateGPS();

	void UpdateAll();

	PILData GetPilData();
	void SendJsonPilDataSerial(HardwareSerial * serial);

private:
	dht * tempHumidity;
	SFE_BMP180 * pressure;
	Accelerometer * accelerometer;
	ML8511 * uv;
	Venus638 * gps;

	PILData pilData;
};