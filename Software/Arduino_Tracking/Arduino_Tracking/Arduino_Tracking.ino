#include <ArduinoJson.h>
#include <Radio.h>
#include <Venus638.h>

Venus638 * gps;
GPSData gpsData;

void setup() {
  // Output serial
  Serial.begin(115200);
  
  // GPS Serial
  Serial1.begin(9600);

  // Create new GPS
  gps = new Venus638(&Serial1);
  gps->InitGPSData(&gpsData);

  // Update baud rate for GPS to 115,200
  gps->SetBaudRate(Venus638::baud_115200);
  Serial1.begin(115200);

  // GPS Update rate at 20 Hz
  gps->SetUpdateRate(Venus638::update_20);
}


void loop() {
  // Get latest GPS Information
  gps->Update();

  // Fill in GPS Data and send it over serial, in JSON form
  gps->FillInGPSData(&gpsData);
  sendJSONDataSerial(&Serial);
  Serial.println("");
  delay(500);
}

void sendJSONDataSerial(HardwareSerial * serial){

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
  
  // Send GPS Data
  trackingDataObject.printTo(*serial);
  
}

