/**
*	@author Jacob Chesley
*	@date 11/10/2015
*/

#ifndef Camera_H
#define Camera_H

#include <arduino.h>

/**
*	
*	This class encapsulates the Linksprite JPEG Color Camera.
*	
*	For use with Arduino - Serial Connection
*	
*	@brief The Camera Class for Arduino
*/
class Camera{
public:

	/**
	* 	Constructor for Camera
	*
	*	@param serial - Hardware Serial used to conenct the camera
	*/
	Camera(HardwareSerial * serial);

	/**
	*	Destructor for Camera
	*/
	~Camera();

	/**
	*	Reset Command
	*/
	bool Reset();

	/**
	*	Take Photo
	*/
	bool Capture();

	/**
	*	Set Photo Size Command
	*/
	bool SetSize(int size);

	/**
	*	Set Baud Rate Command
	*/
	bool SetBaud(int baud);

	bool SetCompression(byte compression);

	void ReadPartOfFile();

	void SendNextPartOfFile(HardwareSerial * outputSerial);

	bool ContinueRead();

	enum {
		size_160_120,
		size_320_240,
		size_640_480,
		size_800_600,
		size_1024_768,
		size_1280_960,
		size_1600_1200
	};

	enum {
		baud_9600,
		baud_38400,
		baud_57600,
		baud_115200,
		baud_128000,
		baud_230400,
		baud_256000,
	};
private:

	HardwareSerial * hardwareSerial;
	long int index;

	bool continueRead;
	bool firstRead;
	byte tempBuffer[32];
	bool ClearPort;
};

#endif