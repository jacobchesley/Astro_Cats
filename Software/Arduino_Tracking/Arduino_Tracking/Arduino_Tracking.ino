#include <ArduinoJson.h>
#include <Radio.h>
#include <Venus638.h>

#define SHUTDOWN_PIN 22
#define BINARY_PIN 23

Venus638 * gps;
GPSData gpsData;

Radio * radio;

void setup() {
  // Output serial
  Serial.begin(115200);
  
  // Radio Serial
  Serial1.begin(9600);
  
  // GPS Serial
  Serial2.begin(9600);

  // ********************** Create new GPS **********************
  gps = new Venus638(&Serial2);
  gps->InitGPSData(&gpsData);

  // Update baud rate for GPS to 115,200
  gps->SetBaudRate(Venus638::baud_115200);
  Serial2.begin(115200);

  delay(500);
  gps->SetUpdateRate(Venus638::update_20);
  
  // ********************** Create new Radio **********************
  radio = new Radio(&Serial1, SHUTDOWN_PIN, BINARY_PIN);
  delay(500);
  if(!radio->Test()){
    Serial.println("Radio not connected!");
  }
  radio->SetGuardTime(1);
  if(!radio->Test()){
    Serial.println("Set Guard time failed!");
  }
 
  if(radio->UpdateSerialBaudRate(Radio::baud_115200)){
    Serial1.begin(115200);
    delay(500);
    if(!radio->Test()){
      Serial.println("Baud rate change failed!");
        if(!radio->EnableHighSpeedRadio()){
          Serial.println("High speed radio failed!");
      }
    }
  }

  if(!radio->EnableBinaryCommands()){
    Serial.println("Binary commands failed!");
  }
}


void loop() {
  
  // Get latest GPS Information
  gps->Update();

  // Fill in GPS Data and send it over serial, in JSON form
  gps->FillInGPSData(&gpsData);
  sendJSONData();
  if(gpsData.NumSatellites > 0){
    delay(500);
  }
  else{
    delay(2000);
  }
}

void sendJSONData(){

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
  int len = trackingDataObject.measureLength();
  char * jsonCharBuffer = new char[len + 2];
  trackingDataObject.printTo(jsonCharBuffer, len+1);

  radio->SendMessage(jsonCharBuffer, len + 1);
  char newline[1] = {'\n'};
  radio->SendMessage(newline, 1);
  //Serial.println(len);
  //Serial.println(jsonCharBuffer);

  delete[] jsonCharBuffer;
  
  
}

