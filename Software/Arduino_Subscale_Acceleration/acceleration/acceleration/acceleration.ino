#include <SPI.h>
#include <SD.h>
#include <adxl345.h>

int ledPin = 13;
int SDCSPin = 10;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
File myFile;
String fileName;
unsigned long startTime;
unsigned long stopTime;
bool started;
unsigned long delta;

void setup() {


  // Setup serial and LED pin
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  if (!SD.begin(SDCSPin)) {
    Serial.println("SD initialization failed!");
    digitalWrite(ledPin, LOW);
    return;
  }
  else{
    Serial.println("SD initialization successful!");
    digitalWrite(ledPin, HIGH);
  }

  for(int i = 0; i < 9999; i++){
    String suffix = String(i);
    fileName = "Acc" + suffix + ".txt";
    if(!SD.exists(fileName)){
      break;
    }
  }

  if(!accel.begin()){
    Serial.println("ADXL345 not found.");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  startTime = 0;
  stopTime = 0;
  delta = 0;
  started = false;
}

void loop() {

  if(!started){
    started = true;
    Serial.println("Started logging!");
  }
  startTime = micros();
  
  sensors_event_t event; 
  accel.getEvent(&event);

  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    myFile.print(event.acceleration.x);myFile.print(",");
    myFile.print(event.acceleration.y);myFile.print(",");
    myFile.print(event.acceleration.z);myFile.print(",");
    myFile.print(delta); myFile.println("");
    myFile.close();
    digitalWrite(ledPin, HIGH);
  } 
  else {
    Serial.println("error opening file");
    digitalWrite(ledPin, LOW);
  }
  
  stopTime = micros();
  delta = stopTime - startTime;
  
}
