#include <RadioAPI.h>

#define SHUTDOWN_PIN 22
#define BINARY_PIN 23

RadioAPI * radio;
void setup() {

  Serial.begin(115200);
  Serial1.begin(115200);
  delay(100);
  
  radio = new RadioAPI(&Serial1, SHUTDOWN_PIN, BINARY_PIN);
  radio->ClearRXArray();
}

void loop() {
    
 radio->GetPackets();
 int numRXPackets = radio->GetRXSize();
 RadioPacketRX * rxPackets = radio->GetRXArray();
 
 for(int i = 0; i < numRXPackets; i++){
  RadioPacketRX tempRX = rxPackets[i];
  for(int byt = 0; byt < tempRX.dataLength; byt++){
    Serial.print((char)tempRX.data[byt]);
  }
 }
 radio->ClearRXArray();
 
}
