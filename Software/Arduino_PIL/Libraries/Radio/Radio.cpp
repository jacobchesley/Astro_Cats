#include "Radio.h"

Radio::Radio(HardwareSerial * serial){
	hardwareSerial = serial;
	guardTime = 1000;
}

Radio::Radio(HardwareSerial * serial, int shutdownPin){
	hardwareSerial = serial;

	pinMode(shutdownPin, OUTPUT);
  	digitalWrite(shutdownPin, HIGH);
	shutdownP = shutdownPin;
	guardTime = 1000;
}

void Radio::SendMessage(char * message, int len){

	for(int i = 0; i < len; i++){
		hardwareSerial->print(message[i]);
	}
}

void Radio::SendMessage(String message){

	for(int i = 0; i < message.length(); i++){
		hardwareSerial->print(message.charAt(i));
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

	// Clear all data in buffer
	while(hardwareSerial->available()){
		hardwareSerial->read();
	}
	
	// Send AT command and wait for OK...
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	bool result = this->WaitAndCheckOK();

	// If it went into command mode, exit command mode.
	if(result){
		hardwareSerial->println("ATCN");
		return this->WaitAndCheckOK();
	}
}

bool Radio::SetGuardTime(int millisecondsHundred){

	// Clear all data in buffer
	while(hardwareSerial->available()){
		hardwareSerial->read();
	}
	
	// Enter command mode
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);
	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}

	hardwareSerial->println("ATAT0x01");

	// Wait and check if guard time command is successful
	if(!this->WaitAndCheckOK()){
		Serial.println("NOT OK - ATAT");
		return false;
	}

	hardwareSerial->println("ATBT0x01");
	// Wait and check if guard time command is successful
	if(!this->WaitAndCheckOK()){
		Serial.println("NOT OK - ATBT");
		return false;
	}

	// Exit command mode
	hardwareSerial->println("ATCN");
	// Wait and check if exit command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}

	guardTime = millisecondsHundred * 100;
	return true;


}
bool Radio::UpdateSerialBaudRate(int baudRate, bool saveToEPROM){

	// Clear all data in buffer
	while(hardwareSerial->available()){
		hardwareSerial->read();
	}

	// enter command mode
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}
	
	// Enter the baud rate change command
	long serialBaud = 9600;
	switch(baudRate){
		case baud_1200:
			hardwareSerial->println("ATBD0");
			serialBaud = 1200;
			break;

		case baud_2400:
			hardwareSerial->println("ATBD1");
			serialBaud = 2400;
			break;

		case baud_4800:
			hardwareSerial->println("ATBD2");
			serialBaud = 4800;
			break;

		case baud_9600:
			hardwareSerial->println("ATBD3");
			serialBaud = 9600;
			break;

		case baud_19200:
			hardwareSerial->println("ATBD4");
			serialBaud = 19200;
			break;

		case baud_38400:
			hardwareSerial->println("ATBD5");
			serialBaud = 38400;
			break;

		case baud_57600:
			hardwareSerial->println("ATBD6");
			serialBaud = 57600;
			break;

		case baud_115200:
			hardwareSerial->println("ATBD7");
			serialBaud = 115200;
			break;

		case baud_230400:
			hardwareSerial->println("ATBD8");
			serialBaud = 230400;
			break;
	}
	// Wait and check if baud change is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}

	// Save to EPROM if enabled
	if(saveToEPROM){
		hardwareSerial->println("ATWR");

		// Wait and check if save to EPROM is successful
		if(!this->WaitAndCheckOK()){
			return false;
		}	
	}

	// Exit command mode
	hardwareSerial->println("ATCN");
	// Wait and check if exit command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}
	return true;
}

bool Radio::EnableHighSpeedRadio(){

	// Clear all data in buffer
	while(hardwareSerial->available()){
		hardwareSerial->read();
	}

	// enter command mode
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}

	hardwareSerial->print("ATBR1\r");
	// Wait and check if rf data rate change is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}

	hardwareSerial->print("ATCN\r");
	// Wait and check if exit command mode is successful
	if(!this->WaitAndCheckOK()){
		return false;
	}
	return true;
}

int Radio::GetLastSignalStrength(){

	while(hardwareSerial->available()){
		hardwareSerial->read();
	}

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);
	
	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){
		Serial.println("Command mode failed");
		return -1;
	}

	hardwareSerial->print("ATDB\r");
	// Wait for signal strength data
	while(!hardwareSerial->available()){}

	delay(10);
	// Copy next 5 values from serial, 4 are each hex returned, followed by a carrige return
	char signalStrengthRaw[5] = {0};
	int length = 0;
	for(int i = 0; i < 5; i++){
		signalStrengthRaw[i] = hardwareSerial->read();
		Serial.print(signalStrengthRaw[i]);
		if(signalStrengthRaw[i] == 13){
			break;
		}
		length += 1;
	}
	

	hardwareSerial->print("ATCN\r");
	// Wait and check if exit command mode is successful
	if(!this->WaitAndCheckOK()){
		Serial.println("Exit command mode failed");
		return -1;
	}

	// Copy only the signal strength part of the returned data
	char signalStrengthHex[4] = {0};
	for(int i = 0; i < length; i++){
		signalStrengthHex[i] = signalStrengthRaw[i];
	}

	// Convert hex char array to signal strength int
	int signalStrength = (int)strtol(signalStrengthHex, NULL, 16);
	return signalStrength;
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

	char temp;
	for(int i = 0; i < 3; i++){
		if(temp = hardwareSerial->read() != ok[i]){
			Serial.print(temp);
			return false;
		}
	}
	return true;
}

bool Radio::WaitAndCheckOK(){
	while(hardwareSerial->available() <= 0){ delay(20); }
	delay(100);
	return this->CheckOK();
}