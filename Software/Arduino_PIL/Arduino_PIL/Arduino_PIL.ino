#include <Camera.h>

Camera * cam;

enum IncomingCommands{
  TAKE_PHOTO,
  GET_GPS,
  GET_MEASUREMENTS,
  SAVE_BATTERY
};

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  cam = new Camera(&Serial1);
  
}

void loop() {
  cam->Reset();
  delay(2000);
  cam->SetBaud(Camera::baud_115200);
  delay(500);

  cam->SetSize(Camera::size_1024_768);
  delay(500);

  cam->SetCompression(0x03);
  delay(500);

   // cam->Reset();
    delay(2000);                            //Wait 2-3 second to send take picture command
    cam->Capture();
    delay(1000);
  
    while(cam->ContinueRead()){
      cam->SendNextPartOfFile(&Serial);
    }

    delay(1000);
    Serial.print("END");
    while(1);
}

int CheckIncomingCommands{
  if(
}
}

