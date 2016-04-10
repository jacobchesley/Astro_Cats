#include <SPI.h>
#include <SD.h>
#include<Wire.h>
#include <Servo.h>

#include <adxl345.h>
#include <ArduinoJson.h>
#include <Camera.h>
#include <dht.h>
#include <ML8511.h>
#include <PIL.h>
#include <RadioAPI.h>
#include <RadioINIT.h>
#include <SFE_BMP180.h>
#include <SP215.h>
#include <Venus638.h>

PIL *pil;

void setup() {

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);
  delay(50);

  pil = new PIL();
  Serial.println("Made it to end of setup");
}

void loop() {

  pil->Loop();
}
