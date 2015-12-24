#include "PIL.h"

PIL::PIL(int tempHumidPin){

	// Create new DHT sensor
	tempHumidity = new dht(tempHumidPin);

	// Create new UV Sensor. analog pin 0 is output, 1 is reference
	uv = new ML8511(0, 1);

	// Create new accelerometer and configure.  SDO is grounded
	accelerometer = new Accelerometer(true);
	accelerometer->Configure();

	// Create new BMP180 sensor
	pressure = new SFE_BMP180();
	// Initialize the pressure sensor, and check for failure
	if(!pressure->begin()){

	}

	gps = new Venus638(&Serial2);
	gps->SetBaudRate(Venus638::baud_115200);
	Serial2.begin(115200);

	gps->SetUpdateRate(Venus638::update_20);
}

void PIL::UpdateAll(){

	this->UpdateTempHumidity();
	this->UpdatePressure();
	this->UpdateAcceleration();
	this->UpdateUV();
	this->UpdateGPS();
}

// Updates the tempurature and humidity inside class variable pilData
void PIL::UpdateTempHumidity(){

	// Read the temperature and humidity data
	int result = tempHumidity->read22();

	switch (result){

		// Uodate the pilData values if result is okay
		case DHTLIB_OK:  
			pilData.Temperature = tempHumidity->temperature;
			pilData.Humidity = tempHumidity->humidity; 
			pilData.TempHumidityError = "";
			break;

		case DHTLIB_ERROR_CHECKSUM: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			pilData.TempHumidityError = "Checksum error";
			break;

		case DHTLIB_ERROR_TIMEOUT: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			pilData.TempHumidityError = "Time out error";
			break;

		default: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			pilData.TempHumidityError = "Unknown error";
			break;
	}	
}

void PIL::UpdatePressure(){
	
	// Start bmp180 tempurature measurement
	int delayTemp = pressure->startTemperature();

	// If the requested delay is < 0,  there is an error.
	if(delayTemp <= 0){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}

	// Delay requested time from BMP180
	delay(delayTemp);
	
	// Read the tempurature from BMP180 and check for error
	if(!pressure->getTemperature(pilData.PressureTemp)){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}

	// Start BMP180 pressure measurement
	// oversampling value provided, 0 (lowest resolution) - 3 (highest resolution)
	int delayPressure = pressure->startPressure(3);

	// If the requested delay is < 0,  there is an error.
	if(delayPressure <= 0){
		pilData.Pressure = -9999;
		return;
	}

	// Delay requested time from BMP180
	delay(delayPressure);

	// Read the pressure from BMP180 and check for error
	if(!pressure->getPressure(pilData.Pressure, pilData.PressureTemp)){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}
}

void PIL::UpdateAcceleration(){
	pilData.AccelerationX = accelerometer->GetAccelerationX();
	pilData.AccelerationY = accelerometer->GetAccelerationY();
	pilData.AccelerationZ = accelerometer->GetAccelerationZ();
}

void PIL::UpdateUV(){
	pilData.UV = uv->GetUV();
}

void PIL::UpdateGPS(){
	gps->Update();
	gps->FillInGPSData(&pilData.gpsData);
}


PILData PIL::GetPilData(){
	return pilData;
}

void PIL::SendJsonPilDataSerial(HardwareSerial * serial){

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& pilDataObject = jsonBuffer.createObject();

	// Set atmospheric data
	pilDataObject["Temp"] = pilData.Temperature;
	pilDataObject["Humidity"] = pilData.Humidity;
	pilDataObject["Pressure"] = pilData.Pressure;
	pilDataObject["UV"] = pilData.UV;

	// Set acceleration data
	pilDataObject["AccelerationX"] = pilData.AccelerationX;
	pilDataObject["AccelerationY"] = pilData.AccelerationY;
	pilDataObject["AccelerationZ"] = pilData.AccelerationZ;

	// Set GPS Data
	pilDataObject["Time"] = pilData.gpsData.Time;
	pilDataObject["Lat"] = pilData.gpsData.Latitude;
	pilDataObject["NS"] = pilData.gpsData.NorthSouth;
	pilDataObject["Lon"] = pilData.gpsData.Longitude;
	pilDataObject["EW"] = pilData.gpsData.EastWest;
	pilDataObject["Altitude"] = pilData.gpsData.Altitude;
	pilDataObject["Quality"] = pilData.gpsData.Quality;
	pilDataObject["NumSat"] = pilData.gpsData.NumSatellites;
	pilDataObject["HDOP"] = pilData.gpsData.HDOP;

	pilDataObject.printTo(Serial);
}