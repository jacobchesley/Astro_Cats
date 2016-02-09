#include <Radio.h>

#define SHUTDOWN_PIN 22

Radio * radio;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  radio = new Radio(&Serial1, SHUTDOWN_PIN);
  delay(1000);
  if(radio->Test()){
    Serial.println("Radio Connected!");
  }
  radio->SetGuardTime(1);
  if(radio->Test()){
    Serial.println("Guard time set correctly!");
  }
  delay(500);

  if(radio->UpdateSerialBaudRate(Radio::baud_115200,false)){
    Serial1.begin(115200);
    delay(500);
    if(radio->Test()){
      Serial.println("Baud rate changed!");
      if(radio->EnableHighSpeedRadio()){
        Serial.println("High speed radio enabled!");
      }
    }
  }
  Serial.println("----------------");
  Serial.println(radio->GetLastSignalStrength());
  Serial.println("----------------");
}

void loop() {
  char message[100];

  int currentBytes = 0;
  int sigStr = -8;
  String sigStrStr;
  if(radio->CheckIncomingMessages() > 0){
    // Get entire message as it is being sent.
    while(currentBytes != radio->CheckIncomingMessages()){
      currentBytes = radio->CheckIncomingMessages();
      delay(20);
    }
    radio->CopyMessage(message, currentBytes);
    for(int i = 0; i < currentBytes; i++){
      Serial.print(message[i]);
    }

    sigStr = radio->GetLastSignalStrength();
    sigStrStr = String(sigStr);
    Serial.println("Signal Strength: " + sigStrStr);
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");

  }
}
