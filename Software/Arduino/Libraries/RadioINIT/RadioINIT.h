#ifndef RadioINIT_H
#define RadioINIT_H

#include <arduino.h>
#include "../RadioAPI/radioAPI.h"

class RadioINIT{
public:

	const int GROUND_ADDRESS_INT = 4794;
	const String GROUND_ADDRESS_HEX_STR = "12BA";

	const int TRACKING_ADDRESS_INT = 4795;
	const String TRACKING_ADDRESS_HEX_STR = "12BB";

	const int PIL_ADDRESS_INT = 4796;
	const String PIL_ADDRESS_HEX_STR = "12BC";

	const String HOPPING_CHANNEL = "7";
	const String AES_KEY = "FFBB8AA17A185DBDEEE03DFC46C0EDFB0FD7674185BD0B5FD8DDFD8E714C03AB";

	RadioINIT();
	RadioINIT(HardwareSerial * serial, int shutdownPin);

	bool InitGround();
	bool InitPIL();
	bool InitTracking();

	void Flush();
	
	String GenerateRandomKey(int keySize);

private:
	bool Reset();
	bool SetRadioSpeed(int speed);
	bool SetMyAddress(String address);
	bool SetHoppingChannel(String hopping);
	bool SetKey(String key);
	bool SetSerial115200();
	bool EnableAPI();
	bool DisableAPI();

	bool SendATCommand(String commandName, bool write);
	bool SendATCommand(String commandName, String parameters, bool write);

	byte CalculateChecksum(byte * data, int dataLength);

	bool CheckOK();
	bool WaitAndCheckOK();
	int guardTime;

	HardwareSerial * hardwareSerial;

	RadioAPI * apiRadio;
};
#endif