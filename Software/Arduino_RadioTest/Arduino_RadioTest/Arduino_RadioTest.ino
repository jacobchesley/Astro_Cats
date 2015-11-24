#include<Radio.h>

Radio * radio;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  radio = new Radio(&Serial1, 53);
  delay(2000);
  radio->EnableHighSpeedRadio();
  delay(2000);
}

void loop() {

  char * message = "TEST";
  radio->SendMessage(message, 4);
  Serial.print(message[0]);
  delay(5000);
  
}

