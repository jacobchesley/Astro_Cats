/**
*	@author Jacob Chesley
*	@date 11/10/2015
*/

#include <Camera.h>

Camera::Camera(HardwareSerial * serial){

	hardwareSerial = serial;
	index = 0x0000;
	continueRead = true;
	firstRead = true;
	memset(tempBuffer, 0, 32);
}

Camera::~Camera(){
	
}

bool Camera::Capture(){

	hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x36);
    hardwareSerial->write(0x01);
    hardwareSerial->write(0x00); 
}

bool Camera::Reset(){

	hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x26);
    hardwareSerial->write(0x00);
}

bool Camera::SetSize(int size){
	hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);  
    hardwareSerial->write(0x54);
    hardwareSerial->write(0x01);

    switch(size){
    	case Camera::size_160_120:
    		hardwareSerial->write(0x22);
    		break;
    	case Camera::size_320_240:
    		hardwareSerial->write(0x11);
    		break;
    	case Camera::size_640_480:
    		hardwareSerial->write(0x00);
    		break;
    	case Camera::size_800_600:
    		hardwareSerial->write(0x1D);
    		break;
    	case Camera::size_1024_768:
    		hardwareSerial->write(0x1C);
    		break;
    	case Camera::size_1280_960:
    		hardwareSerial->write(0x1B);
    		break;
    	case Camera::size_1600_1200:
    		hardwareSerial->write(0x21);
    		break;

    	default:
    		hardwareSerial->write(0x21);
    		break;
    }

    //while(!hardwareSerial->available()){}
    //while(hardwareSerial->available()){
    //	Serial.println(hardwareSerial->read(), HEX);
    //}

}

bool Camera::SetCompression(byte compression){
	hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);  
    hardwareSerial->write(0x31);
    hardwareSerial->write(0x05);
    hardwareSerial->write(0x01);  
    hardwareSerial->write(0x01);
    hardwareSerial->write(0x12);
    hardwareSerial->write(0x04);
    hardwareSerial->write(compression);

}
bool Camera::SetBaud(int baud){
	hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);  
    hardwareSerial->write(0x24);
    hardwareSerial->write(0x03);
    hardwareSerial->write(0x01);

    switch(baud){
    	case Camera::baud_9600:
    		hardwareSerial->write(0xAE);
    		break;
    	case Camera::baud_38400:
    		hardwareSerial->write(0x2A);
    		break;
    	case Camera::baud_57600:
    		hardwareSerial->write(0x1C);
    		break;
    	case Camera::baud_115200:
    		hardwareSerial->write(0x0D);
    		break;
    	case Camera::baud_128000:
    		hardwareSerial->write(0x7E);
    		break;
		case Camera::baud_230400:
			hardwareSerial->write(0x23);
			break;
		case Camera::baud_256000:
			hardwareSerial->write(0x56);
			break;
    	default:
    		hardwareSerial->write(0x0D);
    		break;
    }
}

void Camera::ReadPartOfFile(){

	uint8_t MH=index/0x100;
    uint8_t ML=index%0x100;

    hardwareSerial->write(0x56);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x32);
    hardwareSerial->write(0x0c);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x0a);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x00);
    hardwareSerial->write(MH);
    hardwareSerial->write(ML);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x20);
    hardwareSerial->write(0x00);
    hardwareSerial->write(0x0a);

    index += 0x20;
}

void Camera::SendNextPartOfFile(HardwareSerial * outputSerial){

	// Clear the cameras serial port if this is the first call
	if(firstRead){
		while(hardwareSerial->available() > 0){
			hardwareSerial->read();
		}
		firstRead = false;
	}

	if(continueRead == false){
		return;
	}

	int j=0;
    int k=0;
    int count=0;
    byte incomingbyte;

	this->ReadPartOfFile();
	delay(20);

	while(hardwareSerial->available() > 0){
		incomingbyte = hardwareSerial->read();
		k++;
		if((k > 5) && (j < 32) && (continueRead)){
			tempBuffer[j] = incomingbyte;

			if((tempBuffer[j-1] == 0xFF) && (tempBuffer[j] == 0xD9)){

				continueRead = false;
			}
			j++;
			count++;
		}
	}
 
	for(j = 0; j < count; j++){
		outputSerial->write(tempBuffer[j]);
	}
}

bool Camera::ContinueRead(){
	return continueRead;
}