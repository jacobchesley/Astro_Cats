/**
*	@author Jacob Chesley
*	@date 3/12/2015
*/

#ifndef Accelerometer_H
#define Accelerometer_H

#include <arduino.h>
#include <Wire.h>

/**
*	
*	This reads accelerometer data from the ADXL345.
*	
*	For use with Arduino - i2c connection
*	
*	@brief The Accelerometer Class for Arduino
*/
class Accelerometer{
public:

	/**
	* 	Constructor for Accelerometer
	*
	*	@param SDOGrounded - True if SDO is tied to ground, false if it tied to VCC
	*/
	Accelerometer(bool SDOGrounded);

	/**
	*	Destructor for Accelerometer
	*/
	~Accelerometer();

	/**
	*	Configures the ADXL345 to +/-16G format and enables reading
	*/
	void Configure();

	/**
	*	Get acceleration data from X axis
	*
	*	@return X Axis acceleration
	*/
	float GetAccelerationX();

	/**
	*	Get acceleration data from Y axis
	*
	*	@return Y Axis acceleration
	*/
	float GetAccelerationY();

	/**
	*	Get acceleration data from Z axis
	*
	*	@return Z Axis acceleration
	*/
	float GetAccelerationZ();

private:

	void WriteTo(byte _regaddress, byte val);
	void ReadFrom(byte reg_address, int num, byte ADXL_345_buffer[]);

	float gravity;

	byte buffer[6];
 
 	char address;
	char powerControl;
	char dataFormat;
	char dataX0;
	char dataX1;
	char dataY0;
	char dataY1;
	char dataZ0;
	char dataZ1;
};

#endif