#include "Tracking.h"

Tracking::Tracking(){

	// Output serial
	Serial.begin(115200);

	// Radio Serial
	Serial1.begin(115200);

	// GPS Serial
	Serial2.begin(9600);
	  
	delay(100);
	  
	// **************** Create new GPS *****************
	gps = new Venus638(&Serial2);
	gps->InitGPSData(&gpsData);

	// Update baud rate for GPS to 115,200
	gps->SetBaudRate(Venus638::baud_115200);
	Serial2.begin(115200);

	delay(100);
	gps->SetUpdateRate(Venus638::update_20);

	// **************** Create new radio *****************
	radio = new RadioAPI(&Serial1, 22);
	radio->ClearRXArray();
	radio->ClearTXSArray();

	radioAddresses = new RadioINIT();

	incomingMessage = "";
	lastCommand = "";
	lastCommandValue = "";
	microsecondCount = 0;
	microsecondUpdate = 1000000;
}

void Tracking::Loop(){
	// Get latest GPS Data and fill in gps data struct
	if(microsecondCount > microsecondUpdate){
		gps->Update();
		gps->FillInGPSData(&gpsData);

		// Send tracking string over radio, time since last transfer is now 0 microseconds
		SendTrackingData();
		microsecondCount = 0;
	}

	radio->GetPackets();
	int numRXPackets = radio->GetRXSize();
	RadioPacketRX * rxPackets = radio->GetRXArray();
 	
 	// Go through all incoming radio packets
	for(int i = 0; i < numRXPackets; i++){

		// Get each radio packet
		RadioPacketRX tempRX = rxPackets[i];

		// Get message from packet data
		for(int byt = 0; byt < tempRX.dataLength; byt++){
			incomingMessage += (char)tempRX.data[byt];
		}

		// Parse and execute the incoming command
		this->ParseMessage();
		this->ExecuteLastCommand();
	}

	radio->ClearRXArray();
	radio->ClearTXSArray();

	delayMicroseconds(100);
	microsecondCount += 100;
}

void Tracking::SendTrackingData(){
	// Create JSON Buffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject& trackingDataObject = jsonBuffer.createObject();
  
	// Set GPS Data
	trackingDataObject["Source"] = "Rocket";
	trackingDataObject["MessageType"] = "Data";
	trackingDataObject["Time"] = gpsData.Time;
	trackingDataObject["Lat"].set(gps->DMSToDecimal(gpsData.Latitude), 5);
	trackingDataObject["NS"] = (String)gpsData.NorthSouth;
	trackingDataObject["Lon"].set(gps->DMSToDecimal(gpsData.Longitude), 5);
	trackingDataObject["EW"] = (String)gpsData.EastWest;
	trackingDataObject["Altitude"] = gpsData.Altitude;
	trackingDataObject["Quality"] = gpsData.Quality;
	trackingDataObject["PDOP"] = gpsData.PDOP;
	trackingDataObject["HDOP"] = gpsData.HDOP;
	trackingDataObject["VDOP"] = gpsData.VDOP;
	trackingDataObject["NumSat"] = gpsData.NumSatellites;
	trackingDataObject["SatList"] = gpsData.SatelliteList;
  
	// Initialize char array
	int len = trackingDataObject.measureLength();
	char * jsonCharBuffer = new char[len + 2];
	for(int i = 0; i < len + 2; i++){
		jsonCharBuffer[i] = ' ';
	}

	// Print JSON Buffer to char array
	trackingDataObject.printTo(jsonCharBuffer, len+1);

	// New lines for ease of reading data on ground control
	jsonCharBuffer[len] = '\r';
	jsonCharBuffer[len+1] = '\n';

	// Set byte buffer to point to char array
	byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  
	RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len + 2, radioAddresses->GROUND_ADDRESS_INT, 0x01);
	radio->SendTXRPacket(transmitPacket);
  
	delete[] transmitPacket.data;
	transmitPacket.data = NULL;

	delete[]jsonCharBuffer;
	jsonCharBuffer = NULL;
}

void Tracking::ParseMessage(){

	// Create JSON from incoming message
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& command = jsonBuffer.parseObject(incomingMessage);

	// Get Destination and Message type from JSON
	const char * dest = command["Destination"];
	const char * type = command["MessageType"];
  
	String destination(dest);
	String messageType(type);

	// Verify the destination is for the rocket and the message type is a command
	if(destination == "Rocket" && messageType == "Command"){
		const char * cmd = command["Command"];
		const char * val = command["Value"];

		// Set the last command and last command value that will be executed in ExecuteLastCommand
		String command(cmd);
		String value(val);

		lastCommand = command;
		lastCommandValue = value;
	}
	// Reset incoming message now that we have extracted the command and value
	incomingMessage = "";
}

void Tracking::ExecuteLastCommand(){

	Serial.println("Executing last command");

	
	// Radio Power Level Command
	if(lastCommand == "RadioPower"){

		Serial.println("Power level command");
		// Get integer from lastCommandValue string
		int powerLevel = lastCommandValue.toInt();

		// Verify sure radio power is between 0 and 4
		if(powerLevel < 0 || powerLevel > 4){ return; }

		Serial.println("Value good");
		// Build the AT Radio Command Packet for power level
		byte * radioPowerLevel = new byte[1];
		radioPowerLevel[0] = powerLevel & 0xFF;
		RadioPacketAT powerLevelAPIPacket = radio->BuildATCommandPacket('P', 'L', radioPowerLevel, 1);
		radio->SendATCommandPacket(powerLevelAPIPacket);

	}

	// GPS Update and Send Interval in Microseconds
	if(lastCommand == "UpdateInterval"){

		// Get char array from lastCommandValue string
		char * charVal = new char[lastCommandValue.length() + 1];
		lastCommandValue.toCharArray(charVal, lastCommandValue.length() + 1);

		// Set microsecondUpdate from char array
		microsecondUpdate = atol(charVal);

		// Send confirmation message to ground
		SendMessageConfirmation();
	}
}

void Tracking::SendMessageConfirmation(){
	// Create JSON Buffer
	StaticJsonBuffer<150> jsonBuffer;
	JsonObject& confirmationDataObject = jsonBuffer.createObject();

	// Set Confirmation Data
	confirmationDataObject["Source"] = "Rocket";
	confirmationDataObject["MessageType"] = "CommandResponse";
	confirmationDataObject["Command"] = lastCommand;
	confirmationDataObject["Value"] = lastCommandValue;

	// Initialize char array
	int len = confirmationDataObject.measureLength();
	char * jsonCharBuffer = new char[len + 2];
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
  
  	// Build and send radio transmit packet to ground control
	RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len + 2, radioAddresses->GROUND_ADDRESS_INT, 0x01);
	radio->SendTXRPacket(transmitPacket);
  
	delete[] transmitPacket.data;
	transmitPacket.data = NULL;

	delete[]jsonCharBuffer;
	jsonCharBuffer = NULL;
}
