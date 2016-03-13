#include <ArduinoJson.h>
#include <RadioAPI.h>
#include <Venus638.h>

#define SHUTDOWN_PIN 22
#define BINARY_PIN 23

RadioAPI * radio;
Venus638 * gps;
GPSData gpsData;

String incomingMessage;
String lastCommand;
String lastCommandValue;

long microsecondCount;
long microsecondUpdate;

void setup() {

  
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
  radio = new RadioAPI(&Serial1, SHUTDOWN_PIN, BINARY_PIN);
  radio->ClearRXArray();
  radio->ClearTXSArray();

  incomingMessage = "";
  lastCommand = "";
  lastCommandValue = "";
  microsecondCount = 0;
  microsecondUpdate = 1000000;

}

void loop() {

  // Get latest GPS Data and fill in gps data struct
  if(microsecondCount > microsecondUpdate){
    gps->Update();
    gps->FillInGPSData(&gpsData);
  
    // Send json string over radio
    SendJSONData();
    microsecondCount = 0;
   }

  radio->GetPackets();
  int numRXPackets = radio->GetRXSize();
  RadioPacketRX * rxPackets = radio->GetRXArray();
 
  for(int i = 0; i < numRXPackets; i++){
    RadioPacketRX tempRX = rxPackets[i];
    Serial.println("Got Something");
    for(int byt = 0; byt < tempRX.dataLength; byt++){
      incomingMessage += (char)tempRX.data[byt];
    }
    ParseMessage();
    ExecuteLastCommand();
  }

  radio->ClearRXArray();
  radio->ClearTXSArray();

  delayMicroseconds(100);
  microsecondCount += 100;
}

void ParseMessage(){

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& command = jsonBuffer.parseObject(incomingMessage);

  const char * dest = command["Destination"];
  const char * type = command["MessageType"];
  
  String destination(dest);
  String messageType(type);
  if(destination == "Rocket" && messageType == "Command"){
    const char * cmd = command["Command"];
    const char * val = command["Value"];

    String command(cmd);
    String value(val);

    lastCommand = command;
    lastCommandValue = value;
  }
  incomingMessage = "";
}

void ExecuteLastCommand(){
  if(lastCommand == "RadioPower"){
    int powerLevel = lastCommandValue.toInt();
    if(powerLevel < 0 || powerLevel > 4){ return; }

      byte * radioPowerLevel = new byte[1];
      radioPowerLevel[0] = powerLevel & 0xFF;
      RadioPacketAT powerLevelAPIPacket = radio->BuildATCommandPacket('P', 'L', radioPowerLevel, 1);
      radio->SendATCommandPacket(powerLevelAPIPacket);
      SendMessageConfirmation();
  }

  if(lastCommand == "UpdateInterval"){
    char * charVal = new char[lastCommandValue.length() + 1];
    lastCommandValue.toCharArray(charVal, lastCommandValue.length() + 1);
    microsecondUpdate = atol(charVal);
    SendMessageConfirmation();
  }
}

void SendMessageConfirmation(){

  // Create JSON Buffer
  StaticJsonBuffer<150> jsonBuffer;
  JsonObject& confirmationDataObject = jsonBuffer.createObject();
  
  // Set Confirmation Data
  confirmationDataObject["Source"] = "Rocket";
  confirmationDataObject["MessageType"] = "CommandResponse";
  confirmationDataObject["Command"] = lastCommand;
  confirmationDataObject["Value"] = lastCommandValue;

  // Initialize char and byte array
  int len = confirmationDataObject.measureLength();
  char * jsonCharBuffer = new char[len + 2];
  for(int i = 0; i < len+2; i++){
    jsonCharBuffer[i] = ' ';
  }
  confirmationDataObject.printTo(jsonCharBuffer, len+1);
  jsonCharBuffer[len] = '\r';
  jsonCharBuffer[len+1] = '\n';
  byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  
  RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len+2, 4794, 0x01);
  radio->SendTXRPacket(transmitPacket);
  
  delete[] transmitPacket.data;
  transmitPacket.data = NULL;

  delete[]jsonCharBuffer;
  jsonCharBuffer = NULL;
}


void SendJSONData(){
  
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
  
  // Initialize char and byte array
  int len = trackingDataObject.measureLength();
  char * jsonCharBuffer = new char[len + 2];
  for(int i = 0; i < len+2; i++){
    jsonCharBuffer[i] = ' ';
  }
  trackingDataObject.printTo(jsonCharBuffer, len+1);
 
  jsonCharBuffer[len] = '\r';
  jsonCharBuffer[len+1] = '\n';
  byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  
  RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len+2, 4794, 0x01);
  radio->SendTXRPacket(transmitPacket);
  
  delete[] transmitPacket.data;
  transmitPacket.data = NULL;

  delete[]jsonCharBuffer;
  jsonCharBuffer = NULL;
}
