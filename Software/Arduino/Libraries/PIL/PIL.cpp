#include "PIL.h"

PIL::PIL(){

	nextFrameID = 0x00;

	// Radio Serial
	Serial1.begin(115200);

	// GPS Serial
	Serial2.begin(9600);

	delay(50);

	while(Serial1.available()){
		Serial1.read();
	}

	microsecondCount = 0;
	microsecondUpdate = 500000;

	// see if the card is present
	if(!SD.begin(53)){}
	else{}

	// Create new accelerometer and configure.  SDO is grounded
	if(!accelerometer.begin()){  	}
  	else{
  		accelerometer.setRange(ADXL345_RANGE_16_G);
  	}

	// Initialize the pressure sensor, and check for failure
	if(!pressure.begin()){}

	radio.ClearRXArray();
	radio.ClearTXSArray();

	gps.SetBaudRate(Venus638::baud_115200);
	Serial2.begin(115200);

	delay(50);
	gps.SetUpdateRate(Venus638::update_20);

	// Camera Serial
	bool camIsHighspeed = false;;
	Serial3.begin(256000);

	delay(50);
	
	// Try resetting camera at higher baud (in case power was left on from last baud set)
	if(camera.Reset()){}

	// Camera is not at high baud, set it there.
	else{
		Serial3.begin(115200);
		delay(50);

		if(camera.SetBaud(camera.baud_256000)){
			Serial3.begin(256000);
			delay(50);
		}
		else{
		}

	}
	
	if(!camera.SetSize(camera.size_640_480)){
	}

	//delay(2000);
	//this->TakeAndStorePhoto();
	Serial.println("done init");
/*
	for(int i = 0; i < 10; i++){
		delay(1000);
	}
	this->OpenParachute();
	*/
}

void PIL::UpdateAll(){

	this->UpdateTempHumidity();
	this->UpdatePressure();
	this->UpdateAcceleration();
	this->UpdateUV();
	this->UpdateSolar();
	this->UpdateGPS();
}

void PIL::Loop(){

	// Get latest PIL Data
	if(microsecondCount > microsecondUpdate){

		Serial.println("Updating and sending...");
		this->UpdateAll();
		// Send tracking string over radio, time since last transfer is now 0 microseconds

		SendPilData();
		microsecondCount = 0;
		Serial.println("Done updating and sending");
	}

	radio.GetPackets();

	int numRXPackets = radio.GetRXSize();

	RadioPacketRX * rxPackets = radio.GetRXArray();
 	
 	// Go through all incoming radio packets
	for(int i = 0; i < numRXPackets; i++){

		Serial.println("Radio packet recieved");
		// Get each radio packet
		RadioPacketRX tempRX = rxPackets[i];

		String incomingMessage = "";
		// Get message from packet data
		for(int byt = 0; byt < tempRX.dataLength; byt++){
			incomingMessage += (char)tempRX.data[byt];
		}

		// Parse and execute the incoming command
		this->ParseMessage(incomingMessage);
	}

	radio.ClearRXArray();
	radio.ClearTXSArray();

	delayMicroseconds(100);
	microsecondCount += 100;
}

// Updates the tempurature and humidity inside class variable pilData
void PIL::UpdateTempHumidity(){

	// Read the temperature and humidity data
	int result = tempHumidity.read22();

	switch (result){

		// Uodate the pilData values if result is okay
		case DHTLIB_OK:  
			pilData.Temperature = tempHumidity.temperature;
			pilData.Humidity = tempHumidity.humidity; 
			break;

		case DHTLIB_ERROR_CHECKSUM: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			break;

		case DHTLIB_ERROR_TIMEOUT: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			break;

		default: 
			pilData.Temperature = -9999;
			pilData.Humidity = -9999;
			break;
	}	
}

void PIL::UpdatePressure(){
	
	// Start bmp180 tempurature measurement
	int delayTemp = pressure.startTemperature();

	// If the requested delay is < 0,  there is an error.
	if(delayTemp <= 0){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}

	// Delay requested time from BMP180
	delay(delayTemp);
	
	// Read the tempurature from BMP180 and check for error
	if(!pressure.getTemperature(pilData.PressureTemp)){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}

	// Start BMP180 pressure measurement
	// oversampling value provided, 0 (lowest resolution) - 3 (highest resolution)
	int delayPressure = pressure.startPressure(3);

	// If the requested delay is < 0,  there is an error.
	if(delayPressure <= 0){
		pilData.Pressure = -9999;
		return;
	}

	// Delay requested time from BMP180
	delay(delayPressure);

	// Read the pressure from BMP180 and check for error
	if(!pressure.getPressure(pilData.Pressure, pilData.PressureTemp)){
		pilData.Pressure = -9999;
		pilData.PressureTemp = -9999;
		return;
	}
}

void PIL::UpdateAcceleration(){

	sensors_event_t event; 
  	accelerometer.getEvent(&event);
	pilData.AccelerationX = event.acceleration.x;
	pilData.AccelerationY = event.acceleration.y;
	pilData.AccelerationZ = event.acceleration.z;
}

void PIL::UpdateUV(){
	pilData.UV = uv.GetUV();
}

void PIL::UpdateSolar(){
	pilData.Solar = solar.GetSolar();
}

void PIL::UpdateGPS(){
	gps.Update();
	gps.FillInGPSData(&pilData.gpsData);
}

void PIL::TakeAndStorePhoto(){
	
	SD.remove("Photo1.jpg");
	File photoFile = SD.open("Photo1.jpg", FILE_WRITE);

	if(!photoFile){
		Serial.println("File bad");
		return;
	}

	if(camera.Capture()){
		Serial.println("Camera Capture Good!");
	}
	else{
		Serial.println("Captute bad?");
	}
	unsigned long StartTime = millis();

	while(camera.ContinueRead()){
		camera.SendNextPartOfFile(&photoFile);
	}
	unsigned long CurrentTime = millis();
	unsigned long ElapsedTime = CurrentTime - StartTime;
	Serial.println("Done!");
	Serial.println(ElapsedTime/1000.0f);
	
	photoFile.close();
}

PILData PIL::GetPilData(){
	return pilData;
}

void PIL::SendPilData(){

	StaticJsonBuffer<400> jsonBuffer;
	JsonObject& pilDataObject = jsonBuffer.createObject();

	pilDataObject["Source"] = "PIL";
	pilDataObject["MessageType"] = "Data";

	// Set atmospheric data
	pilDataObject["Temp"] = pilData.Temperature;
	pilDataObject["Humidity"] = pilData.Humidity;
	pilDataObject["Pressure"] = pilData.Pressure;
	pilDataObject["UV"] = pilData.UV;
	pilDataObject["Solar"] = pilData.Solar;

	// Set acceleration data
	pilDataObject["AccelerationX"] = pilData.AccelerationX;
	pilDataObject["AccelerationY"] = pilData.AccelerationY;
	pilDataObject["AccelerationZ"] = pilData.AccelerationZ;

	// Set GPS Data
	pilDataObject["Time"] = pilData.gpsData.Time;
	pilDataObject["Lat"].set(gps.DMSToDecimal(pilData.gpsData.Latitude), 5);
	pilDataObject["NS"] = (String)pilData.gpsData.NorthSouth;
	pilDataObject["Lon"].set(gps.DMSToDecimal(pilData.gpsData.Longitude), 5);
	pilDataObject["EW"] = (String)pilData.gpsData.EastWest;
	pilDataObject["Altitude"] = pilData.gpsData.Altitude;
	pilDataObject["Quality"] = pilData.gpsData.Quality;
	pilDataObject["PDOP"] = pilData.gpsData.PDOP;
	pilDataObject["HDOP"] = pilData.gpsData.HDOP;
	pilDataObject["VDOP"] = pilData.gpsData.VDOP;
	pilDataObject["NumSat"] = pilData.gpsData.NumSatellites;
	pilDataObject["SatList"] = pilData.gpsData.SatelliteList;

	// Initialize char and byte array
	int len = pilDataObject.measureLength();

	//char * jsonCharBuffer = new char[len + 2];
	char jsonCharBuffer[400];
	for(int i = 0; i < len + 2; i++){
		jsonCharBuffer[i] = ' ';
	}

	pilDataObject.printTo(jsonCharBuffer, len + 1);

	jsonCharBuffer[len] = '\r';
	jsonCharBuffer[len+1] = '\n';
	byte * jsonByteBuffer = (byte*)jsonCharBuffer;

	byte id = this->GetNextFrameID();
	RadioPacketTXR transmitPacket = radio.BuildTXRPacket(jsonByteBuffer, len + 2, radioAddresses.GROUND_ADDRESS_INT, id);
	radio.SendTXRPacket(transmitPacket);

	//pilDataObject.printTo(Serial);
}

void PIL::ParseMessage(String message){

	// Create JSON from incoming message
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& command = jsonBuffer.parseObject(message);

	// Get Destination and Message type from JSON
	const char * dest = command["Destination"];
	const char * type = command["MessageType"];
  
	String destination(dest);
	String messageType(type);

	// Verify the destination is for the rocket and the message type is a command
	if(destination == "PIL" && messageType == "Command"){
		const char * cmd = command["Command"];
		const char * val = command["Value"];

		// Set the last command and last command value that will be executed in ExecuteLastCommand
		String command(cmd);
		String value(val);

		this->ExecuteCommand(command, value);
	}
}

void PIL::ExecuteCommand(String commandName, String commandValue){

	if(commandName == "CaptureImage"){

	}

	if(commandName == "ListFiles"){

	}

	if(commandName == "LockParachute"){
		this->CloseParachute();
		this->SendMessageConfirmation(commandName, "SUCCESS");
	}

	if(commandName == "ReleaseParachute"){
		this->OpenParachute();
		this->SendMessageConfirmation(commandName, "SUCCESS");
	}

	// PIL Data Update and Send Interval in Microseconds
	if(commandName == "UpdateInterval"){

		// Get char array from lastCommandValue string
		char * charVal = new char[commandValue.length() + 1];
		commandValue.toCharArray(charVal, commandValue.length() + 1);

		// Set microsecondUpdate from char array
		microsecondUpdate = atol(charVal);

		// Send confirmation message to ground
		this->SendMessageConfirmation(commandName, commandValue);

		delete[] charVal;
	}
}

void PIL::SendMessageConfirmation(String commandName, String response){
	// Create JSON Buffer
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& confirmationDataObject = jsonBuffer.createObject();

	// Set Confirmation Data
	confirmationDataObject["Source"] = "PIL";
	confirmationDataObject["MessageType"] = "CommandResponse";
	confirmationDataObject["Command"] = commandName;
	confirmationDataObject["Value"] = response;

	// Initialize char array
	int len = confirmationDataObject.measureLength();
	char  jsonCharBuffer[200];
	for(int i = 0; i < len + 2; i++){
		jsonCharBuffer[i] = ' ';
	}

	// Print JSON Buffer to char array
	confirmationDataObject.printTo(jsonCharBuffer, len + 1);

	// New lines for ease of reading data on ground control
	jsonCharBuffer[len] = '\r';
	jsonCharBuffer[len+1] = '\n';

	// Set byte buffer to point to char array
	byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  	
  	byte id = this->GetNextFrameID();
  	// Build and send radio transmit packet to ground control
	RadioPacketTXR transmitPacket = radio.BuildTXRPacket(jsonByteBuffer, len + 2, radioAddresses.GROUND_ADDRESS_INT, id);
	radio.SendTXRPacket(transmitPacket);

  
}

void PIL::OpenParachute(){
	motor.attach(8);
	motor.write(180);
	delay(2000);
	motor.detach();
}

void PIL::CloseParachute(){
	motor.attach(8);
	motor.write(90);
	delay(2000);
	motor.detach();
}

byte PIL::GetNextFrameID(){
	nextFrameID += 1;
	nextFrameID = nextFrameID % 255;
	if(nextFrameID == 0){
		nextFrameID += 1;
	}
	return nextFrameID & 0xFF;

}