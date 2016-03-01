#include "RadioAPI.h"

RadioAPI::RadioAPI(HardwareSerial * serial, int shutdownPin){
	hardwareSerial = serial;

	guardTime = 1000;	
	binaryCommands = false;

	pinMode(shutdownPin, OUTPUT);
	digitalWrite(shutdownPin, HIGH);
	shutdownP = shutdownPin;

	escapeByte = 0x7D;
	startByte = 0x7E;
	xOn = 0x11;
	xOff = 0x13;

	maxRXArraySize = 50;
	maxTXSArraySize = 50;

	rxArray = new RadioPacketRX[maxRXArraySize];
	txsArray = new RadioPacketTXS[maxTXSArraySize];
	rxArraySize = 0;
	txsArraySize = 0;

	for(int i = 0; i < maxRXArraySize; i++){ rxArray[i].data = NULL; }
	this->Flush();
}


RadioAPI::RadioAPI(HardwareSerial * serial, int shutdownPin, int binaryPin){
	hardwareSerial = serial;

	guardTime = 100;	
	binaryCommands = false;

	pinMode(shutdownPin, OUTPUT);
	digitalWrite(shutdownPin, HIGH);
	shutdownP = shutdownPin;

	pinMode(binaryPin, OUTPUT);
	digitalWrite(binaryPin, HIGH);
	binaryP = binaryPin;

	escapeByte = 0x7D;
	startByte = 0x7E;
	xOn = 0x11;
	xOff = 0x13;

	maxRXArraySize = 50;
	maxTXSArraySize = 50;

	rxArray = new RadioPacketRX[maxRXArraySize];
	txsArray = new RadioPacketTXS[maxTXSArraySize];
	rxArraySize = 0;
	txsArraySize = 0;

	for(int i = 0; i < maxRXArraySize; i++){ rxArray[i].data = NULL; }
	this->Flush();
}

void RadioAPI::Flush(){

	while(hardwareSerial->available()){
		hardwareSerial->read();
	}
}

void RadioAPI::GetPackets(){

	RadioPacket tempPacket;

	if(hardwareSerial->available() >= 4){

		// Read start delimiter
		hardwareSerial->read();

		// Read Length;
		byte lengthMSB = hardwareSerial->read();
		byte lengthLSB = hardwareSerial->read();

		// Read ID
		tempPacket.cmdID = hardwareSerial->read();

		// Get size of data and create new data array
		int dataSize = (lengthMSB << 8) + lengthLSB;
		tempPacket.length = dataSize - 1;

		byte * tempData = new byte[tempPacket.length];
		//tempPacket.data = new byte[dataSize - 1];

		bool nextEscape = false;
		int trueIndex = 0;

		// Populate packet data
		for(int i = 0; i < tempPacket.length; i++){

			while(!hardwareSerial->available()){}
			tempData[trueIndex] = hardwareSerial->read();
	
			if(nextEscape){
				nextEscape = false;
				tempData[trueIndex] ^= 0x20;
			}
			//if(tempData[i] == escapeByte){
			//	nextEscape = true;
			//	continue;
			//}
			trueIndex += 1;
		}
		tempPacket.length = trueIndex;

		// Copy only elements we want to packet
		tempPacket.data = new byte[trueIndex];
		memcpy(tempPacket.data, tempData, trueIndex);
		delete[] tempData;

		// Get checksum
		while(!hardwareSerial->available()){}
		tempPacket.checksum = hardwareSerial->read();

		if(this->VerifyChecksum(&tempPacket)){

			// Build a RX (Recive) Packet from the raw data packet
			if(tempPacket.cmdID == 0x81){
				RadioPacketRX rxPacket = this->BuildRXPacket(&tempPacket);
				this->AddRXArray(rxPacket);
			}

			// Build a TXS (Transmit Status) Packet from the raw data packet
			else if(tempPacket.cmdID == 0x89){
				RadioPacketTXS txsPacket = this->BuildTXSPacket(&tempPacket);
				this->AddTXSArray(txsPacket);		
			}

			// Module status packet
			else if(tempPacket.cmdID == 0x8A){}

			// Unknown packet
			else{}
		}
		else{
			Serial.println("Incoming packet checksum failed");
		}
		delete[] tempPacket.data;
	}
}

bool RadioAPI::VerifyChecksum(RadioPacket * packet){
	byte checksum = 0x00;
	checksum += packet->cmdID;

	for(int i = 0; i < packet->length; i++){
		checksum += packet->data[i];
	}

	byte calculatedChecksum = 0xFF - checksum;

	if(packet->checksum == calculatedChecksum){
		return true;
	}
	return false;
}

byte RadioAPI::CalculateChecksum(byte * data, int dataLength){

	byte checksum = 0x00;

	for(int i = 0; i < dataLength; i++){
		checksum += data[i];
	}

	return 0xFF - checksum;
}

RadioPacketAT RadioAPI::BuildATCommandPacket(char atLetter1, char atLetter2, byte * params, int paramLength){
	RadioPacketAT outPacket;

	outPacket.length = paramLength + 4;

	// 0x08 is at command
	outPacket.cmdID = 0x08;
	outPacket.frameID = 0x01;

	// Set command name
	outPacket.atLetter1 = atLetter1;
	outPacket.atLetter2 = atLetter2;

	// Set parameters
	outPacket.parameters = new byte[paramLength];
	for(int i = 0; i < paramLength; i++){
		outPacket.parameters[i] = params[i];
	}

	// Create checksum
	byte * checksumData = new byte[paramLength + 4];
	checksumData[0] = outPacket.cmdID;
	checksumData[1] = outPacket.frameID;
	checksumData[2] = outPacket.atLetter1;
	checksumData[3] = outPacket.atLetter2;

	for(int i = 4; i < outPacket.length; i++){
		checksumData[i] = params[i - 4];
	}
	outPacket.checksum = this->CalculateChecksum(checksumData, outPacket.length);
	return outPacket;
}

RadioPacketRX RadioAPI::BuildRXPacket(RadioPacket * packet){
	RadioPacketRX outPacket;
	outPacket.dataLength = packet->length - 4;
	outPacket.sourceAddress = (packet->data[0]<< 8) + packet->data[1];
	outPacket.signalStrength = packet->data[2] * -1; 
	outPacket.options = packet->data[3];
	outPacket.data = new byte[outPacket.dataLength];

	for(int i = 4; i < packet->length; i++){
		outPacket.data[i - 4] = packet->data[i]; 
	}

	return outPacket;
}

RadioPacketTXR RadioAPI::BuildTXRPacket(byte * data, int dataLength, int destination, byte frameID){
	RadioPacketTXR outPacket;
	outPacket.dataLength = dataLength + 5;
	outPacket.cmdID = 0x01;
	outPacket.frameID = frameID;
	outPacket.destinationAddress = destination;
	outPacket.option = 0x00;
	outPacket.data = new byte[dataLength];

	for(int i = 0; i < dataLength; i++){
		outPacket.data[i] = data[i]; 
	}

	byte * checksumData = new byte[outPacket.dataLength];

	checksumData[0] = (byte) outPacket.cmdID;
	checksumData[1] = (byte) outPacket.frameID;
	checksumData[2] = (byte)(destination >> 8) & 0XFF;
	checksumData[3] = (byte) destination & 0xFF;
	checksumData[4] = (byte) outPacket.option;

	for(int i = 5; i < outPacket.dataLength; i++){
		checksumData[i] = data[i-5]; 
	}

	outPacket.checksum = this->CalculateChecksum(checksumData, outPacket.dataLength);
	delete[] checksumData;

	return outPacket;
}

bool RadioAPI::SendTXRPacket(RadioPacketTXR sendPacket){

	// Send start delimtier
	hardwareSerial->write(0x7E);

	// Turn length into 2 bytes and send
	byte lengthMSB = (byte)((sendPacket.dataLength >> 8) & 0xFF);
	byte lengthLSB = (byte)  sendPacket.dataLength & 0xFF;
	hardwareSerial->write(lengthMSB);
	hardwareSerial->write(lengthLSB);

	// Send API ID for transmit request
	hardwareSerial->write(sendPacket.cmdID);

	// Send Frame ID
	hardwareSerial->write(sendPacket.frameID);

	// Send Destination Address
	byte destinationMSB = (byte)((sendPacket.destinationAddress >> 8) & 0xFF);
	byte destinationLSB = (byte)  sendPacket.destinationAddress & 0xFF;
	hardwareSerial->write(destinationMSB);
	hardwareSerial->write(destinationLSB);

	// Send option
	hardwareSerial->write(sendPacket.option);

	// Send data
	for(int i = 0; i < sendPacket.dataLength - 5; i++){
		hardwareSerial->write(sendPacket.data[i]);
	}

	// Send checksum
	hardwareSerial->write(sendPacket.checksum);
}

RadioPacketTXS RadioAPI::BuildTXSPacket(RadioPacket * packet){
	RadioPacketTXS outPacket;
	outPacket.cmdID = packet->cmdID;
	outPacket.frameID = packet->data[0];
	outPacket.status = packet->data[1];
	outPacket.checksum = packet->checksum;

	return outPacket;
}

void RadioAPI::AddTXSArray(RadioPacketTXS txsPacket){
	txsArray[txsArraySize] = txsPacket;
	txsArraySize += 1;
}

void RadioAPI::AddRXArray(RadioPacketRX rxPacket){
	rxArray[rxArraySize] = rxPacket;
	rxArraySize += 1;
}

void RadioAPI::ClearTXSArray(){
	txsArraySize = 0;
}

void RadioAPI::ClearRXArray(){
	for(int i = 0; i < maxRXArraySize; i++){
		if(rxArray[i].data != NULL){
			delete[] rxArray[i].data;
			rxArray[i].data = NULL;
		}
	}
	rxArraySize = 0;
}

int RadioAPI::GetTXSSize(){
	return txsArraySize;
}

int RadioAPI::GetRXSize(){
	return rxArraySize;
}

RadioPacketTXS * RadioAPI::GetTXSArray(){
	return txsArray;
}

RadioPacketRX * RadioAPI::GetRXArray(){
	return rxArray;
}

bool RadioAPI::CheckOK(){

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

bool RadioAPI::WaitAndCheckOK(){

	while(hardwareSerial->available() <= 0){ delay(20); }
	delay(50);
	return this->CheckOK();
}