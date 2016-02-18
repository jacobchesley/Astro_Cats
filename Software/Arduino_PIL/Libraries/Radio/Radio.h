/**
*	@author Jacob Chesley
*	@date 11/20/2015
*/

#ifndef Radio_H
#define Radio_H

#include <arduino.h>

/**
*	
*	This class encapsulates the Digi International XTEND 900 1 Watt Radio
*	
*	For use with Arduino - Serial Connection
*	
*	@brief The Radio Class for Arduino
*/
class Radio{
public:

	/**
	* 	Constructor for radio
	*
	*	@param serial - Hardware Serial used to conenct the radio
	*/
	Radio(HardwareSerial * serial);

	/**
	* 	Constructor for radio
	*
	*	@param serial - Hardware Serial used to conenct the radio
	*	@param shutdownPin - pin used for shutdown
	*/
	Radio(HardwareSerial * serial, int shutdownPin);

	/**
	* 	Constructor for radio
	*
	*	@param serial - Hardware Serial used to conenct the radio
	*	@param shutdownPin - pin used for shutdown
	*	@param binaryPin - pin used to select binary command mode
	*/
	Radio(HardwareSerial * serial, int shutdownPin, int binaryPin);

	/**
	*	Destructor for radio
	*/
	~Radio();

	/**
	* Sends a message over radio
	*/
	void SendMessage(char * message, int length);

	void SendMessage(String message);

	/**
	* Returns number of bytes of data is available.
	*/
	int CheckIncomingMessages();

	/**
	* Copies the message in buffer to the char * supplied.
	*/
	void CopyMessage(char * outMessage, int len);

	/**
	* Shutdowns the radio for power saving
	*/
	void Shutdown();

	/**
	* Wakes up the radio from shutdown
	*/
	void Wakeup();

	/**
	* Tests the radio for connectivity by asking to go to command mode, 
	* and checking for response
	*/
	bool Test();

	/**
	* Updates the serial port baud rate between device and radio
	*/
	bool UpdateSerialBaudRate(int baudRate);

	bool EnableHighSpeedRadio();

	int GetLastSignalStrength();

	int ChangeBaseUnits(int baseUnit);

	bool SetGuardTime(int millisecondsHundred);

	bool EnableBinaryCommands();

	bool DisableBinaryCommands();

	enum {
		default_base_units,
		hex_no_units,
		default_base_no_units
	};

	enum {
		baud_1200,
		baud_2400,
		baud_4800,
		baud_9600,
		baud_19200,
		baud_38400,
		baud_57600,
		baud_115200,
		baud_230400
	};

private:

	bool CheckOK();
	bool WaitAndCheckOK();
	int guardTime;
	bool binaryCommands;

	HardwareSerial * hardwareSerial;
	int shutdownP;
	int binaryP;
};

#endif