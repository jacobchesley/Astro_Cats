#include "RadioINIT.h"


RadioINIT::RadioINIT(){}

RadioINIT::RadioINIT(HardwareSerial * serial, int shutdownPin){
	hardwareSerial = serial;

	apiRadio = new RadioAPI(serial, shutdownPin);

	guardTime = 1000;	

	pinMode(shutdownPin, OUTPUT);
	digitalWrite(shutdownPin, HIGH);

	this->Flush();
}

bool RadioINIT::InitGround(){
	Serial.println("Initializing ground radio..."); Serial.println("");

	if(this->DisableAPI()){ Serial.println("API mode disabled"); }
	else{ return false; }
	if(this->Reset()){ Serial.println("Radio reset"); }
	else{ return false; }

	// Default baud rate for rado is 9600 BPS
	hardwareSerial->begin(9600); delay(100);

	if(this->SetRadioSpeed(1)){ Serial.println("High speed radio enabled"); } 
	else{ return false; }
	if(this->SetMyAddress(GROUND_ADDRESS_HEX_STR)){ Serial.println("My address set to: " + GROUND_ADDRESS_HEX_STR); } 
	else{ return false; }
	if(this->SetHoppingChannel(HOPPING_CHANNEL)){ Serial.println("Hopping Channel Set to: " + HOPPING_CHANNEL); } 
	else{ return false; }
	if(this->SetKey(AES_KEY)){ Serial.println("Private key set"); }
	else{ return false;}
	if(this->SetSerial115200()){ Serial.println("Radio interface baud set to: 115,200 BPS"); }
	
	hardwareSerial->begin(115200); delay(100);

	if(this->EnableAPI()){ Serial.println("API mode enabled"); }
	else{ return false; }

	Serial.println("");
	Serial.println("Ground radio succesfully initialized!");
	return true;
}

bool RadioINIT::InitTracking(){

	Serial.println("Initializing tracking radio..."); Serial.println("");

	if(this->DisableAPI()){ Serial.println("API mode disabled"); }
	else{ return false; }
	if(this->Reset()){ Serial.println("Radio reset"); }
	else{ return false; }

	// Default baud rate for rado is 9600 BPS
	hardwareSerial->begin(9600); delay(100);

	if(this->SetRadioSpeed(1)){ Serial.println("High speed radio enabled"); } 
	else{ return false; }
	if(this->SetMyAddress(TRACKING_ADDRESS_HEX_STR)){ Serial.println("My address set to: " + TRACKING_ADDRESS_HEX_STR); } 
	else{ return false; }
	if(this->SetHoppingChannel(HOPPING_CHANNEL)){ Serial.println("Hopping Channel Set to: " + HOPPING_CHANNEL); } 
	else{ return false; }
	if(this->SetKey(AES_KEY)){ Serial.println("Private key set"); }
	else{ return false;}
	if(this->SetSerial115200()){ Serial.println("Radio interface baud set to: 115,200 BPS"); }
	
	hardwareSerial->begin(115200); delay(100);

	if(this->EnableAPI()){ Serial.println("API mode enabled"); }
	else{ return false; }

	Serial.println("");
	Serial.println("Tracking radio succesfully initialized!");
	return true;
}

bool RadioINIT::InitPIL(){
	Serial.println("Initializing PIL radio..."); Serial.println("");

	if(this->DisableAPI()){ Serial.println("API mode disabled"); }
	else{ return false; }
	if(this->Reset()){ Serial.println("Radio reset"); }
	else{ return false; }

	// Default baud rate for rado is 9600 BPS
	hardwareSerial->begin(9600); delay(100);

	if(this->SetRadioSpeed(1)){ Serial.println("High speed radio enabled"); } 
	else{ return false; }
	if(this->SetMyAddress(PIL_ADDRESS_HEX_STR)){ Serial.println("My address set to: " + PIL_ADDRESS_HEX_STR); } 
	else{ return false; }
	if(this->SetHoppingChannel(HOPPING_CHANNEL)){ Serial.println("Hopping Channel Set to: " + HOPPING_CHANNEL); } 
	else{ return false; }
	if(this->SetKey(AES_KEY)){ Serial.println("Private key set"); }
	else{ return false;}
	if(this->SetSerial115200()){ Serial.println("Radio interface baud set to: 115,200 BPS"); }
	
	hardwareSerial->begin(115200); delay(100);

	if(this->EnableAPI()){ Serial.println("API mode enabled"); }
	else{ return false; }

	Serial.println("");
	Serial.println("PIL radio succesfully initialized!");
	return true;
}

bool RadioINIT::SetSerial115200(){

	this->SendATCommand("BD", "7", true);
}

bool RadioINIT::SetRadioSpeed(int speed){

	// Send new address and save
	if(speed == 1){ return this->SendATCommand("BR", "1", true); }
	else{ return this->SendATCommand("BR", "0", true); }
}

bool RadioINIT::EnableAPI(){

	// Send API command (non escaped characters - param 1) and save
	return this->SendATCommand("AP", "1", true);
}

bool RadioINIT::DisableAPI(){
	
	this->Flush();

	byte * disableAPParams = new byte[1];
	disableAPParams[0] = 0x00;
	RadioPacketAT disableAPIPacket = apiRadio->BuildATCommandPacket('A', 'P', disableAPParams, 1);
	apiRadio->SendATCommandPacket(disableAPIPacket);

	delay(100);
	apiRadio->Flush();
	return true;
}

bool RadioINIT::SetMyAddress(String address){

	// Send new address and save
	return this->SendATCommand("MY", address, true);
}

bool RadioINIT::SetHoppingChannel(String hopping){

	// Send new hopping channel and save
	return this->SendATCommand("HP", hopping, true);
}

bool RadioINIT::SetKey(String key){

	// Send new key and save
	return this->SendATCommand("KY", key, true);
}

bool RadioINIT::Reset(){

	// Send reset command and save
	return this->SendATCommand("RE", true);
}

bool RadioINIT::SendATCommand(String commandName, bool write){

	// Clear serial buffer
	this->Flush();

	// Enter command mode
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	// Execute command
	hardwareSerial->println("AT" + commandName);
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory, if enabled
	if(write){
		hardwareSerial->println("ATWR");
		if(!this->WaitAndCheckOK()){ return false; }
	}

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::SendATCommand(String commandName, String parameters, bool write){

	// Clear serial buffer
	this->Flush();

	// Enter command mode
	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	// Execute command
	hardwareSerial->println("AT" + commandName + parameters);
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory, if enabled
	if(write){
		hardwareSerial->println("ATWR");
		if(!this->WaitAndCheckOK()){ return false; }
	}

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

void RadioINIT::Flush(){

	while(hardwareSerial->available()){
		hardwareSerial->read();
	}
}

String RadioINIT::GenerateRandomKey(int keySize){
	String key = "";
	int tempRandom;
	randomSeed(analogRead(0));

	for(int i = 0; i < keySize; i++){

		// Choose random number, 0 or 1
		tempRandom = random(2);

		// if it is 0, choose a random hex letter
		if(tempRandom == 0){
			tempRandom = random(6);
			switch(tempRandom){
				case 0: key += "A"; break;
				case 1: key += "B"; break;
				case 2: key += "C"; break;
				case 3: key += "D"; break;
				case 4: key += "E"; break;
				case 5: key += "F"; break;
			}
		}

		// If it is 1, choose random number between 0 and 9
		else{
			tempRandom = random(10);
			key += tempRandom;
		}
	}
	return key;
}

bool RadioINIT::CheckOK(){

	char ok[3];
	ok[0] = 79;
	ok[1] = 75;
	ok[2] = 13;

	char temp;
	for(int i = 0; i < 3; i++){
		temp = hardwareSerial->read();
		if(temp != ok[i]){ return false; }
	}
	return true;
}

bool RadioINIT::WaitAndCheckOK(){

	while(hardwareSerial->available() <= 0){ delay(20); }
	delay(50);
	return this->CheckOK();
}