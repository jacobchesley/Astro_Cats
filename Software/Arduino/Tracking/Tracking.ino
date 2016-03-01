#include <ArduinoJson.h>
#include <RadioAPI.h>
#include <Venus638.h>

#define SHUTDOWN_PIN 22
#define BINARY_PIN 23

RadioAPI * radio;
Venus638 * gps;
GPSData gpsData;

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
}

void loop() {

  // Get latest GPS Data and fill in gps data struct
  gps->Update();
  gps->FillInGPSData(&gpsData);
  
  // Send json string over radio
  SendJSONData();

  // Delay 0.5 seconds if satellites are locked in, otherwise delay 2 seconds
  if(gpsData.NumSatellites > 0){ delay(500); }
  else{ delay(2000); }

  radio->ClearRXArray();
  radio->ClearTXSArray();
}


void SendJSONData(){
  
  // Create JSON Buffer
  StaticJsonBuffer<500> jsonBuffer;
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
  char * jsonCharBuffer = new char[len + 4];
  for(int i = 0; i < len+4; i++){
    jsonCharBuffer[i] = ' ';
  }
  trackingDataObject.printTo(jsonCharBuffer, len+1);
  jsonCharBuffer[len+2] = '\r';
  jsonCharBuffer[len+3] = '\n';
  byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  
  RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len+4, 4794, 0x01);
  radio->SendTXRPacket(transmitPacket);
  
  delete[] transmitPacket.data;
  transmitPacket.data = NULL;

  delete[]jsonCharBuffer;
  jsonCharBuffer = NULL;
}
