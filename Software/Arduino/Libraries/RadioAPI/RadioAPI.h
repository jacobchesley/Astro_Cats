#ifndef RadioAPI_H
#define RadioAPI_H

#include <arduino.h>

struct RadioPacket{
	int length;
	byte cmdID;
	byte * data;
	byte checksum;
};

struct RadioPacketRX : RadioPacket{
	int dataLength;
	byte cmdID;
	int sourceAddress;
	int signalStrength;
	byte options;
	byte * data;
	byte checksum;
};

struct RadioPacketTXR : RadioPacket{
	int dataLength;
	byte cmdID;
	byte frameID;
	int destinationAddress;
	byte option;
	byte * data;
	byte checksum;
};

struct RadioPacketTXS : RadioPacket{
	byte cmdID;
	byte frameID;
	byte status;
	byte checksum;
};

struct RadioPacketAT : RadioPacket{
	int length;
	byte cmdID;
	byte frameID;
	byte atLetter1;
	byte atLetter2;
	byte * parameters;
	byte checksum;
};

class RadioAPI{
public:
	RadioAPI(HardwareSerial * serial, int shutdownPin);
	RadioAPI(HardwareSerial * serial, int shutdownPin, int binaryPin);

	void GetPackets();
	void Flush();

	void ClearRXArray();
	void ClearTXSArray();
	int GetRXSize();
	int GetTXSSize();
	RadioPacketRX * GetRXArray();
	RadioPacketTXS * GetTXSArray();

	RadioPacketTXR BuildTXRPacket(byte * data, int dataLength, int destination, byte frameID);
	RadioPacketAT BuildATCommandPacket(char atLetter1, char atLetter2, byte * params, int paramLength);

	bool SendTXRPacket(RadioPacketTXR sendPacket);
	bool SendATPacket(RadioPacketAT sendPacket);

	bool Test();
	bool WaitAndCheckOK();

private:

	RadioPacketRX BuildRXPacket(RadioPacket * packet);
	RadioPacketTXS BuildTXSPacket(RadioPacket * packet);
	
	void AddRXArray(RadioPacketRX rxPacket);
	void AddTXSArray(RadioPacketTXS txsPacket);

	bool CheckOK();
	//bool WaitAndCheckOK();

	bool VerifyChecksum(RadioPacket * packet);
	byte CalculateChecksum(byte * data, int dataLength);

	int guardTime;
	bool binaryCommands;

	HardwareSerial * hardwareSerial;
	int shutdownP;
	int binaryP;

	byte escapeByte;
	byte startByte;
	byte xOn;
	byte xOff;

	RadioPacketRX * rxArray;
	int rxArraySize;
	int maxRXArraySize;
	RadioPacketTXS * txsArray;
	int txsArraySize;
	int maxTXSArraySize;
};

#endif