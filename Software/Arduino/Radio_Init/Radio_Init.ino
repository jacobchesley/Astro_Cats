#include <RadioAPI.h>
#include <RadioINIT.h>

//#define SHUTDOWN_PIN 22 // Ground and Tracking
#define SHUTDOWN_PIN 31 // PIL

RadioINIT * initRadio;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(3000);
  
  while(Serial.available()){ Serial.read(); }
  while(Serial1.available()){ Serial1.read(); }
  initRadio = new RadioINIT(&Serial1, SHUTDOWN_PIN);

  int keySize = 64;
  //Serial.print(keySize); Serial.print(" digit hex key: "); Serial.print(initRadio->GenerateRandomKey(keySize)); Serial.println("");
  
  initRadio->InitPIL();
}


void loop() {}
