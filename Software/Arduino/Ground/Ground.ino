#include <ArduinoJson.h>
#include <RadioAPI.h>
#include <RadioINIT.h>

#define SHUTDOWN_PIN 22

RadioAPI * radio;
RadioINIT * radioAddresses;

int lastTrackingSignal;
int lastPILSignal;
String incomingMessage;
byte frameID;

void setup() {

  Serial.begin(115200);
  Serial1.begin(115200);
  delay(100);

  while(Serial.available()){
    Serial.read();
  }
  while(Serial1.available()){
    Serial1.read();
  }
  radio = new RadioAPI(&Serial1, SHUTDOWN_PIN);
  radioAddresses = new RadioINIT();
  
  radio->ClearRXArray();
  incomingMessage = "";
  frameID = 0x01;
  Serial.println("Ground Ready");
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
  
  if(tempRX.sourceAddress == radioAddresses->TRACKING_ADDRESS_INT){
    SendSignalStrength("Tracking", tempRX.signalStrength);
  }
  if(tempRX.sourceAddress == radioAddresses->PIL_ADDRESS_INT){
    SendSignalStrength("PIL", tempRX.signalStrength);
  }
 }
 radio->ClearRXArray();

 int previousMessageLength = incomingMessage.length();
 CheckIncomingMessages();
 int newMessageLength = incomingMessage.length();
 if(newMessageLength == previousMessageLength && newMessageLength != 0){
  SendMessage();
  incomingMessage = "";
 }
}

void SendSignalStrength(String source, int signalStrength){

   // Create JSON Buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& trackingDataObject = jsonBuffer.createObject();
  
  // Set GPS Data
  trackingDataObject["Source"] = "Ground";
  trackingDataObject["MessageType"] = "SignalStrength";
  trackingDataObject["SignalSource"] = source;
  trackingDataObject["SignalStrength"] = signalStrength;
  //trackingDataObject.printTo(Serial);
  //Serial.print('\r');
  //Serial.print('\n');
  
  // Initialize char array
  int len = trackingDataObject.measureLength();
  char * jsonCharBuffer = new char[len + 2];
  for(int i = 0; i < len+2; i++){
    jsonCharBuffer[i] = ' ';
  }

  trackingDataObject.printTo(jsonCharBuffer, len+1);
  jsonCharBuffer[len] = '\r';
  jsonCharBuffer[len+1] = '\n';

  for(int i = 0; i < len+2; i++){
    Serial.print(jsonCharBuffer[i]);
  }
  delete[]jsonCharBuffer;
  jsonCharBuffer = NULL;
  
}

void CheckIncomingMessages(){

  while(Serial.available()){
    incomingMessage += (char) Serial.read();
  }
}

int GetMessageDestination(){

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& command = jsonBuffer.parseObject(incomingMessage);

  const char * dest = command["Destination"];
  String destination(dest);
  if(destination == "PIL"){
    return radioAddresses->PIL_ADDRESS_INT;
  }
  if(destination == "Rocket"){
    return radioAddresses->TRACKING_ADDRESS_INT;
  }
  return -1;
}

void SendMessage(){

  Serial.println(incomingMessage);
  int destination = GetMessageDestination();

  if(destination < 0){ return; }

  int len = incomingMessage.length();
  char jsonCharBuffer[400];
  for(int i = 0; i < len+2; i++){
    jsonCharBuffer[i] = ' ';
  }

  for(int i = 0; i < len; i++){
    jsonCharBuffer[i] = incomingMessage[i];
  }
  jsonCharBuffer[len] = '\r';
  jsonCharBuffer[len+1] = '\n';
  byte * jsonByteBuffer = (byte*)jsonCharBuffer;
  
  RadioPacketTXR transmitPacket = radio->BuildTXRPacket(jsonByteBuffer, len+2, destination, frameID);
  radio->SendTXRPacket(transmitPacket);

  
  frameID = (frameID + 0x01) % 0xFF;
  if(frameID == 0x00){
    frameID +=1;
  }
}


