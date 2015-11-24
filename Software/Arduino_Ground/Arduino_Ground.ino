#include<Radio.h>

Radio * radio;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  radio = new Radio(&Serial1, 53);
  
  delay(2000);
  radio->EnableHighSpeedRadio();
  delay(1000);
  radio->ChangeBaseUnits(Radio::default_base_units);
}

void loop() {
  int numBytes = radio->CheckIncomingMessages();
  char tempBuff[100];
  radio->CopyMessage(tempBuff, numBytes);
  for(int i = 0; i < numBytes; i++){
    Serial.print(tempBuff[i]);

    if(i == numBytes - 1){
      Serial.print("   ");
      radio->GetLastSignalStrength();
      Serial.println(" ");
    }
  }
  
  delay(100);
}
