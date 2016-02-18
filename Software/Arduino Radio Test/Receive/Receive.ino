#include <Radio.h>

#define SHUTDOWN_PIN 22
#define BINARY_PIN 23

Radio * radio;

String message;
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  
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
  Serial.println("Ground READY!");
}

void loop() {
  int messageSize = radio->CheckIncomingMessages();
  char * outMessages = new char[messageSize];
  radio->CopyMessage(outMessages, messageSize);
  Serial.print(outMessages);
  delete[] outMessages;
}

void SendLastSignalStrength(){

}


