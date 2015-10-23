int i;
void setup() {
  Serial.begin(9600);
  i = 0;
}

void loop() {
  if(Serial.available() > 0){
    int messageSize = 0;
    char * message = readPCMessage(messageSize);
    Serial.println(sizeof(message)
  }

  delay(200);
}

char * readPCMessage(int * messageSize){

  char tempBuffer[1024];
  int currentIndex = 0;

  // Read all bytes of data available
  while(Serial.available > 0){
    tempBuffer[currentIndex] = Serial.read();
    currentIndex += 1;
  }
  
  *messageSize = currentIndex;
  return message;
}

bool compareString(char * str1, char * str2, int len){

  
}

