#include <SD.h>
#include<Wire.h>

#include <Accelerometer.h>
#include <ArduinoJson.h>
#include <dht.h>
#include <ML8511.h>
#include <PIL.h>
#include <SFE_BMP180.h>
#include <Venus638.h>

// Sensor Pins
int pin_TempHumid = 22;
int CS_PIN = 53;

PIL *pil;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  // Setup SD Card
  pinMode(CS_PIN, OUTPUT);
  
  // see if the card is present
  if (!SD.begin(CS_PIN)){
    Serial.println("SD Card failed to initialize");
  }
  else{
    Serial.println("SD Card Initialized!");
  }
 
  pil = new PIL(pin_TempHumid);
}

void loop() {

  pil->UpdateAll();
  pil->SendJsonPilDataSerial(&Serial);
  Serial.println("");
  delay(300);
}
