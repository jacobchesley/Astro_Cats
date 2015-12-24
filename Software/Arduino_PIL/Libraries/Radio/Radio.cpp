#include "Radio.h"

Radio::Radio(HardwareSerial * serial){
	hardwareSerial = serial;
}

Radio::Radio(HardwareSerial * serial, int shutdownPin){
	hardwareSerial = serial;

	pinMode(shutdownPin, OUTPUT);
  	digitalWrite(shutdownPin, HIGH);
	shutdownP = shutdownPin;
}

void Radio::SendMessage(char * message, int len){

	for(int i = 0; i < len; i++){
		hardwareSerial->print(message[i]);
	}
}

int Radio::CheckIncomingMessages(){
	return hardwareSerial->available();
}

void Radio::CopyMessage(char * outMessage, int len){

	for(int i = 0; i < len; i++){
		outMessage[i] = hardwareSerial->read();
	}
}

void Radio::Shutdown(){
	digitalWrite(shutdownP, LOW);
}

void Radio::Wakeup(){
	digitalWrite(shutdownP, HIGH);
}

bool Radio::Test(){
	while(hardwareSerial->available()){
		hardwareSerial->read();
	}
	Serial.print("Testing...");
	hardwareSerial->print("+++");
	delay(2000);
	//char temp[3];

	while(hardwareSerial->available()){
		Serial.print(hardwareSerial->read());
	}
	Serial.print("Tested");
}
bool Radio::UpdateSerialBaudRate(int baudRate, bool saveToEPROM){
	// enter command mode
	hardwareSerial->print("+++");

	switch(baudRate){
		case baud_1200:
			hardwareSerial->println("ATBD0");
			break;

		case baud_2400:
			hardwareSerial->println("ATBD1");
			break;

		case baud_4800:
			hardwareSerial->println("ATBD2");
			break;

		case baud_9600:
			hardwareSerial->println("ATBD3");
			break;

		case baud_19200:
			hardwareSerial->println("ATBD4");
			break;

		case baud_38400:
			hardwareSerial->println("ATBD5");
			break;

		case baud_57600:
			hardwareSerial->println("ATBD6");
			break;

		case baud_115200:
			hardwareSerial->println("ATBD7");
			break;

		case baud_230400:
			hardwareSerial->println("ATBD8");
			break;
	}
	
	if(saveToEPROM){
		hardwareSerial->println("ATWR");
	}

	hardwareSerial->println("ATCN");
}

void Radio::EnableHighSpeedRadio(){
	// enter command mode
	hardwareSerial->print("+++");

	hardwareSerial->print("ATBR1\r");

	hardwareSerial->print("ATCN\r");
}

int Radio::GetLastSignalStrength(){

	while(hardwareSerial->available()){
		hardwareSerial->read();
	}

	hardwareSerial->print("+++");
	while(hardwareSerial->available() <= 0){ delay(10); }
	if(!this->CheckOK()){
		return - 1;
	}
	
	hardwareSerial->print("ATDB\r");
	while(hardwareSerial->available() <= 0){ delay(10); }
	char signalStrengthRaw[2] = {0};
	for(int i = 0; i < 2; i++){
		signalStrengthRaw[i] = hardwareSerial->read();
		Serial.print("  :  ");
		Serial.print(signalStrengthRaw[i], HEX);
		Serial.print("  :  ");
	}

	hardwareSerial->print("ATCN\r");
	while(hardwareSerial->available() <= 0){ delay(10); }
	if(!this->CheckOK()){
		return - 1;
	}

}

int Radio::ChangeBaseUnits(int baseUnit){

	while(hardwareSerial->available()){
		hardwareSerial->read();
	}

	hardwareSerial->print("+++");
	while(hardwareSerial->available() <= 0){ delay(10); }
	if(!this->CheckOK()){
		Serial.println("Failed 1");
		return - 1;
	}

	switch(baseUnit){
		case default_base_units:
			hardwareSerial->print("ATCF0\r");
			break;
		case hex_no_units:
			hardwareSerial->print("ATCF1\r");
			break;

		case default_base_no_units:
			hardwareSerial->print("ATCF2\r");
			break;
	}

	while(hardwareSerial->available() <= 0){ delay(10); }
	hardwareSerial->read();

	hardwareSerial->print("ATCN\r");
	while(hardwareSerial->available() <= 0){ delay(10); }
	if(!this->CheckOK()){

		Serial.println("Failed 2");
		return - 1;
	}

	Serial.println("CHANGED BASE!");

}
bool Radio::CheckOK(){

	char ok[3];
	ok[0] = 79;
	ok[1] = 75;
	ok[2] = 13;

	for(int i = 0; i < 3; i++){
		if(hardwareSerial->read() != ok[i]){
			return false;
		}
	}
	return true;
}