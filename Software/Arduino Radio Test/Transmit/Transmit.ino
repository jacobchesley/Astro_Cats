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
  delay(2000);

  if(radio->UpdateSerialBaudRate(Radio::baud_115200,false)){
    Serial1.begin(115200);
    delay(1000);
    if(radio->Test()){
      Serial.println("Baud rate changed");
      if(radio->EnableHighSpeedRadio()){
        Serial.println("High speed radio enabled!");
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  String prefix = "Hello from PIL! - ";
  String suffix;
  String ending = "\n";
  int count = 0;
  while(true){
    suffix = String(count);
    radio->SendMessage(prefix + suffix + ending);
    count += 1;

    delay(5000);
  }

}
