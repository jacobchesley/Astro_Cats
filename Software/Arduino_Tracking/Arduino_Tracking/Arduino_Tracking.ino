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
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& trackingDataObject = jsonBuffer.createObject();
  
  // Set GPS Data
  trackingDataObject["Source"] = "Rocket";
  trackingDataObject["Time"] = gpsData.Time;
  trackingDataObject["Lat"] = gpsData.Latitude;
  trackingDataObject["NS"] = (String)gpsData.NorthSouth;
  trackingDataObject["Lon"] = gpsData.Longitude;
  trackingDataObject["EW"] = (String)gpsData.EastWest;
  trackingDataObject["Altitude"] = gpsData.Altitude;
  trackingDataObject["Quality"] = gpsData.Quality;
  trackingDataObject["NumSat"] = gpsData.NumSatellites;
  //trackingDataObject["PDOP"] = gpsData.PDOP;
  trackingDataObject["HDOP"] = gpsData.HDOP;
  //trackingDataObject["VDOP"] = gpsData.VDOP;
  
  // Send GPS Data
  trackingDataObject.printTo(*serial);
}

