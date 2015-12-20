#include <ArduinoJson.h>

#include <PIL.h>

#include<Wire.h>

#include <dht.h>
#include <SFE_BMP180.h>
#include <Accelerometer.h>
#include <ML8511.h>
#include <Venus638.h>

PIL *pil;
PILData *data;

dht *DHT;

// Sensor Pins
int pin_TempHumid = 22;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial2.begin(9600);

  pil = new PIL(pin_TempHumid);
  
}

void loop() {

  pil->UpdateAll();
  pil->SendJsonPilDataSerial(&Serial);
  Serial.println("");
 
/*  
  Serial.print("Temp: ");
  Serial.print(pil->GetPilData().Temperature);
  Serial.println("");
  
  Serial.print("Humidity: ");
  Serial.print(pil->GetPilData().Humidity);
  Serial.println("");
  
  Serial.print("Pressure: ");
  Serial.print(pil->GetPilData().Pressure);
  Serial.println("");

  Serial.print("UV: ");
  Serial.print(pil->GetPilData().UV);
  Serial.println("");

  Serial.print("Acceleration X: ");
  Serial.print(pil->GetPilData().AccelerationX);
  Serial.println("");
  
  Serial.print("Acceleration Y: ");
  Serial.print(pil->GetPilData().AccelerationY);
  Serial.println("");
  
  Serial.print("Acceleration Z: ");
  Serial.print(pil->GetPilData().AccelerationZ);
  Serial.println("");

  Serial.print("GPS Lat: ");
  Serial.print(pil->GetPilData().gpsData.Latitude);
  Serial.println("");
  
  Serial.print("GPS N/S: ");
  Serial.print(pil->GetPilData().gpsData.NorthSouth);
  Serial.println("");
  
  Serial.print("GPS Lon: ");
  Serial.print(pil->GetPilData().gpsData.Longitude);
  Serial.println("");

  Serial.print("GPS E/W: ");
  Serial.print(pil->GetPilData().gpsData.EastWest);
  Serial.println("");

  Serial.print("GPS Time: ");
  Serial.print(pil->GetPilData().gpsData.Time);
  Serial.println("");

  Serial.print("GPS Quality: ");
  Serial.print(pil->GetPilData().gpsData.Quality);
  Serial.println("");
  
  Serial.print("GPS Num Satellites: ");
  Serial.print(pil->GetPilData().gpsData.NumSatellites);
  Serial.println("");

  Serial.print("GPS HDOP: ");
  Serial.print(pil->GetPilData().gpsData.HDOP);
  Serial.println("");

  Serial.print("GPS Altitude: ");
  Serial.print(pil->GetPilData().gpsData.Altitude);
  Serial.println("");

  Serial.print("GPS Station ID: ");
  Serial.print(pil->GetPilData().gpsData.StationID);
  Serial.println("");
  
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  //delay(500);
  */
}

