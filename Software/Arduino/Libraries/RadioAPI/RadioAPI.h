#ifndef RadioAPI_H
#define RadioAPI_H

#include <arduino.h>

// Generic Radio Packet
struct RadioPacket{
	int length;
	byte cmdID;
	byte * data;
	byte checksum;
};

// Recieve
struct RadioPacketRX : RadioPacket{
	int dataLength;
	byte cmdID;
	int sourceAddress;
	int signalStrength;
	byte options;
	byte * data;
	byte checksum;
};

// Transmit Request
struct RadioPacketTXR : RadioPacket{
	int dataLength;
	byte cmdID;
	byte frameID;
	int destinationAddress;
	byte option;
	byte * data;
	byte checksum;
};

// Transmit Status
struct RadioPacketTXS : RadioPacket{
	byte cmdID;
	byte frameID;
	byte status;
	byte checksum;
};

// AT Command Request
struct RadioPacketAT : RadioPacket{
	int dataLength;
	byte cmdID;
	byte frameID;
	byte atLetter1;
	byte atLetter2;
	byte * parameters;
	byte checksum;
};

// AT Command Response
struct RadioPacketATR : RadioPacket{
	int dataLength;
	byte cmdID;
	byte frameID;
	byte atLetter1;
	byte atLetter2;
	byte status;
	byte * data;
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
	void ClearATRArray();

	int GetRXSize();
	int GetTXSSize();
	int GetATRSize();

	RadioPacketRX * GetRXArray();
	RadioPacketTXS * GetTXSArray();
	RadioPacketATR * GetATRArray();

	RadioPacketTXR BuildTXRPacket(byte * data, int dataLength, int destination, byte frameID);
	RadioPacketAT BuildATCommandPacket(char atLetter1, char atLetter2, byte * params, int paramLength);

	void SendTXRPacket(RadioPacketTXR sendPacket);
	void SendATCommandPacket(RadioPacketAT sendPacket);

	bool Test();
	bool WaitAndCheckOK();

private:

	RadioPacketRX BuildRXPacket(RadioPacket * packet);
	RadioPacketTXS BuildTXSPacket(RadioPacket * packet);
	RadioPacketATR BuildATRPacket(RadioPacket * packet);

	void AddRXArray(RadioPacketRX rxPacket);
	void AddTXSArray(RadioPacketTXS txsPacket);
	void AddATRArray(RadioPacketATR atrPacket);

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

	RadioPacketATR * atrArray;
	int atrArraySize;
	int maxATRArraySize;
};

#endif