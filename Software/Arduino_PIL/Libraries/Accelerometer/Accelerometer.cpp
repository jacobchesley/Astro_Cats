/**
*	@author Jacob Chesley
*	@date 3/12/2015
*/

#include <Accelerometer.h>

Accelerometer::Accelerometer(bool SDOGrounded){

	if(SDOGrounded == true){
		address = 0x53;
	}
	else{
		address = 0x1D;
	}

	// Acceleration of Gravity
	gravity = 9.80665;

	// Device Registers
	powerControl = 0x2D;  //Power Control Register
	dataFormat = 0x31;
	dataX0 = 0x32; //X-Axis Data 0
	dataX1 = 0x33; //X-Axis Data 1
	dataY0 = 0x34; //Y-Axis Data 0
	dataY1 = 0x35; //Y-Axis Data 1
	dataZ0 = 0x36; //Z-Axis Data 0
	dataZ1 = 0x37; //Z-Axis Data 1
}

Accelerometer::~Accelerometer(){
	
}

void Accelerometer::Configure(){

	//Set up the Chip Select pin to be an output from the Arduino.
  	pinMode(CS, OUTPUT);
  	//Before communication starts, the Chip Select pin needs to be set high.
  	digitalWrite(CS, HIGH);
	//Put the ADXL345 into +/- 16G range by writing the value 0x0B to the dataFormat register.
	WriteTo(dataFormat, 0x0B);

	//Put the ADXL345 into Measurement Mode by writing 0x08 to the power control register.
	WriteTo(powerControl, 0x08);
}

float Accelerometer::GetAccelerationX(){

	uint8_t numBytes = 6;
	ReadFrom(dataX0, numBytes, buffer); //read the acceleration data from the ADXL345
 
	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first
	// thus we are converting both bytes in to one int
	float x = (((int)buffer[1]) << 8) | buffer[0];   
	float y = (((int)buffer[3]) << 8) | buffer[2];
	float z = (((int)buffer[5]) << 8) | buffer[4];
 
	// convert the raw data to float's representing g's
	x *= 0.0039;
   
	// return the x axis acceleration
	return x * gravity;
}

float Accelerometer::GetAccelerationY(){

	uint8_t numBytes = 6;
	ReadFrom(dataX0, numBytes, buffer); //read the acceleration data from the ADXL345
 
	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	float x = (((int)buffer[1]) << 8) | buffer[0];   
	float y = (((int)buffer[3]) << 8) | buffer[2];
	float z = (((int)buffer[5]) << 8) | buffer[4];
 
	// convert the raw data to float's representing g's
	y *= 0.0039;
   
	// return the y axis acceleration
	return y * gravity;
}

float Accelerometer::GetAccelerationZ(){

	uint8_t numBytes = 6;
	ReadFrom(dataX0, numBytes, buffer); //read the acceleration data from the ADXL345
 
	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	float x = (((int)buffer[1]) << 8) | buffer[0];   
	float y = (((int)buffer[3]) << 8) | buffer[2];
	float z = (((int)buffer[5]) << 8) | buffer[4];
 
	// convert the raw data to float's representing g's
	z *= 0.0039;
   
	// return the z axis acceleration
	return z * gravity;
}

void Accelerometer::WriteTo(byte reg_address, byte val){

	Wire.beginTransmission(address);			// start transmission to device 
	Wire.write(reg_address);					// send register address
	Wire.write(val);							// send value to write
	Wire.endTransmission();						// end transmission
}

void Accelerometer::ReadFrom(byte reg_address, int num, byte in_buffer[]){

	Wire.beginTransmission(address); 		// start transmission to device 
	Wire.write(reg_address);             				// sends address to read from
	Wire.endTransmission();         				// end transmission
 
	Wire.beginTransmission(reg_address);		// start transmission to device
	Wire.requestFrom((char)address, num);			// request 6 bytes from device
 
	int i = 0;

	while(Wire.available()){      

    	in_buffer[i] = Wire.read();    // receive a byte
		i++;
	}
	Wire.endTransmission();		// end transmission
}