#include "RadioINIT.h"

RadioINIT::RadioINIT(HardwareSerial * serial, int shutdownPin){
	hardwareSerial = serial;

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

	if(this->EnableHighSpeedRadio()){ Serial.println("High speed radio enabled"); } 
	else{ return false; }
	if(this->SetMyAddress(GROUND_ADDRESS_HEX_STR)){ Serial.println("My address set to: " + GROUND_ADDRESS_HEX_STR); } 
	else{ return false; }
	if(this->SetHoppingChannel("7")){ Serial.println("Hopping Channel Set to: 7"); } 
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

	if(this->EnableHighSpeedRadio()){ Serial.println("High speed radio enabled"); } 
	else{ return false; }
	if(this->SetMyAddress(TRACKING_ADDRESS_HEX_STR)){ Serial.println("My address set to: " + TRACKING_ADDRESS_HEX_STR); } 
	else{ return false; }
	if(this->SetHoppingChannel("7")){ Serial.println("Hopping Channel Set to: 7"); } 
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

}

bool RadioINIT::SetSerial115200(){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	hardwareSerial->println("ATBD7");
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::EnableHighSpeedRadio(){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	hardwareSerial->println("ATBR1");
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::EnableAPI(){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	hardwareSerial->println("ATAP1");
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	return true;
}

bool RadioINIT::DisableAPI(){
	
	this->Flush();

	// Start delimiter
	hardwareSerial->write(0x7E);

	// Length
	hardwareSerial->write(0x00);
	hardwareSerial->write(0x05);

	// AT Command ID, Frame ID, 
	hardwareSerial->write(0x08);
	hardwareSerial->write(0x01);

	// First AT Command Char, Second AT Command Char
	hardwareSerial->write('A');
	hardwareSerial->write('P');
	hardwareSerial->write(0x00);

	byte * checksumData = new byte[5];
	checksumData[0] = 0x08;
	checksumData[1] = 0x01;
	checksumData[2] = 0x41;
	checksumData[3] = 0x50;
	checksumData[4] = 0x00;

	byte checksum = this->CalculateChecksum(checksumData, 5);

	// Send checksum
	hardwareSerial->write(checksum);

	while(!hardwareSerial->available()){}

	// Print out response for disabling API - debugging purposes
	byte temp = 0x00;
	while(hardwareSerial->available()){
		temp = hardwareSerial->read();
		//Serial.print(temp, HEX);
		//Serial.print(" ");
		delay(10);
	}

	delete[] checksumData;
}

bool RadioINIT::SetMyAddress(String address){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	hardwareSerial->println("ATMY" + address);
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::SetHoppingChannel(String hopping){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	// Change hopping channel
	hardwareSerial->println("ATHP" + hopping);
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::SetKey(String key){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	// Change key
	hardwareSerial->println("ATKY" + key);
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

	// Exit command mode
	hardwareSerial->println("ATCN");
	if(!this->WaitAndCheckOK()){ return false; }
	
	return true;
}

bool RadioINIT::Reset(){
	this->Flush();

	delay(guardTime);
	hardwareSerial->print("+++");
	delay(guardTime);

	// Wait and check if command mode is successful
	if(!this->WaitAndCheckOK()){ return false; }

	hardwareSerial->println("ATRE");
	if(!this->WaitAndCheckOK()){ return false;}

	// Write settings to non volitale memory
	hardwareSerial->println("ATWR");
	if(!this->WaitAndCheckOK()){ return false; }

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

byte RadioINIT::CalculateChecksum(byte * data, int dataLength){
	byte checksum = 0x00;

	for(int i = 0; i < dataLength; i++){
		checksum += data[i];
	}

	return 0xFF - checksum;
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
				case 0:
					key += "A";
					break;
				case 1:
					key += "B";
					break;
				case 2:
					key += "C";
					break;
				case 3:
					key += "D";
					break;
				case 4:
					key += "E";
					break;
				case 5:
					key += "F";
					break;
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
		if(temp != ok[i]){
			return false;
		}
	}
	return true;
}

bool RadioINIT::WaitAndCheckOK(){

	while(hardwareSerial->available() <= 0){ delay(20); }
	delay(50);
	return this->CheckOK();
}