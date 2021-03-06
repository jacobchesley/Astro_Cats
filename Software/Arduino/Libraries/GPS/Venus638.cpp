#include "Venus638.h"

Venus638::Venus638(HardwareSerial * serialIn){

	gpsSerial = serialIn;
	currentDataIndex = 0;

	ggaLen = 0;
	gllLen = 0;
	gsaLen = 0;
	gsvLen = 0;
	rmcLen = 0;
	vtgLen = 0;

	memset(currentData, ' ', 200);
	memset(gga, ' ', 200);
	memset(gll, ' ', 200);
	memset(gsa, ' ', 200);
	memset(gsv, ' ', 200);
	memset(rmc, ' ', 200);
	memset(vtg, ' ', 200);

	while(gpsSerial->available()){
		gpsSerial->read();
	}

	lastGSATime = 0;
	firstGSA = false;
}

String Venus638::GetTime(){
	return GetDataAfterComma(gga, ggaLen, 1);
}

String Venus638::GetLatitude(){
	return GetDataAfterComma(gga, ggaLen, 2);
}

char Venus638::GetNorthSouth(){
	return GetDataAfterComma(gga, ggaLen, 3)[0];
}

String Venus638::GetLongitude(){
	
	return GetDataAfterComma(gga, ggaLen, 4);
}

char Venus638::GetEastWest(){
	return GetDataAfterComma(gga, ggaLen, 5)[0];
}

int Venus638::GetQuality(){
	return GetDataAfterComma(gga, ggaLen, 6).toInt();
}

int Venus638::GetNumSatellites(){
	return GetDataAfterComma(gga, ggaLen, 7).toInt();
}

String Venus638::GetSatelliteList(){
	
	return GetDataAfterCommaGSA(gsa, gsaLen, 3);
}

float Venus638::GetPDOP(){
	char PDOP[8] = {' '};
	GetDataAfterCommaGSA(gsa, gsaLen, 4).toCharArray(PDOP, sizeof(PDOP));
	return atof(PDOP);
}

float Venus638::GetHDOP(){
	char HDOP[8] = {' '};
	GetDataAfterCommaGSA(gsa, gsaLen, 5).toCharArray(HDOP, sizeof(HDOP));
	return atof(HDOP);
}

float Venus638::GetVDOP(){
	char VDOP[8] = {' '};
	GetDataAfterCommaGSA(gsa, gsaLen, 6).toCharArray(VDOP, sizeof(VDOP));
	return atof(VDOP);
}

float Venus638::GetAltitude(){
	char altitude[16] = {' '};
	GetDataAfterComma(gga, ggaLen, 9).toCharArray(altitude, sizeof(altitude));
	return atof(altitude);
}

int Venus638::GetStationID(){
	return GetDataAfterComma(gga, ggaLen, 10).toInt();
}

void Venus638::FillInGPSData(GPSData * data){

	data->Latitude = this->GetLatitude();
	data->Longitude = this->GetLongitude();
	data->NorthSouth = this->GetNorthSouth();
	data->EastWest = this->GetEastWest();
	data->Time = this->GetTime();
	data->Quality = this->GetQuality();
	data->NumSatellites = this->GetNumSatellites();
	data->Altitude = this->GetAltitude();
	data->StationID = this->GetStationID();
	data->PDOP = this->GetPDOP();
	data->HDOP = this->GetHDOP();
	data->VDOP = this->GetVDOP();
	data->SatelliteList = this->GetSatelliteList();
}

void Venus638::InitGPSData(GPSData * data){
	
	data->Latitude = "";
	data->Longitude = "";
	data->NorthSouth = ' ';
	data->EastWest = ' ';
	data->Time = "";
	data->Quality = 0;
	data->NumSatellites = 0;
	data->HDOP = 0.0f;
	data->Altitude = 0.0f;
	data->StationID = 0;
}

void Venus638::Update(){

	bool stopAll = false;
	char tempChar = ' ';

	bool fieldsFound[6] = {false};
	
	while(!stopAll){

		// Read next character if availble and add to current data
		if(gpsSerial->available()){
			tempChar = gpsSerial->read();
			currentData[currentDataIndex] = tempChar;
			currentDataIndex += 1;
		}

		// If last character received is a newline, check if we have data we need
		if(currentData[currentDataIndex - 1] == '\n'){
			
			fieldsFound[this->UpdateSpecificData(currentData, currentDataIndex)] = true;

			unsigned long deltaTime = millis() - lastGSATime;
			// We have all the fields we are looking for!
			if(fieldsFound[0] && firstGSA && (deltaTime < 5000)){
				stopAll = true;

				if(fieldsFound[2]){
					lastGSATime = millis();
					firstGSA = true;
				}
			}

			else if(fieldsFound[0] && fieldsFound[2]){
				stopAll = true;
				lastGSATime = millis();
				firstGSA = true;
			}
			// Reset the currentData
			for(int i = 0; i < currentDataIndex; i++){
				currentData[i] = ' ';
			}
			currentDataIndex = 0;
		}
	}
}
void Venus638::SetBaudRate(int baudRate){

	while(gpsSerial->available()){
		gpsSerial->read();
	}

	byte payloadChecksumCalc[4] = {0x05, 0x00, 0x00, 0x00};

	gpsSerial->write(0xA0);
	gpsSerial->write(0xA1);

	gpsSerial->write(0x00);
	gpsSerial->write(0x04);

	gpsSerial->write(0x05);
	gpsSerial->write(0x00);

	switch(baudRate){
		case baud_4800:
			gpsSerial->write(0x00);
			payloadChecksumCalc[2] = 0x00; 
			break;

		case baud_9600:
			gpsSerial->write(0x01);
			payloadChecksumCalc[2] = 0x01; 
			break;

		case baud_19200:
			gpsSerial->write(0x02);
			payloadChecksumCalc[2] = 0x02; 
			break;

		case baud_38400:
			gpsSerial->write(0x03);
			payloadChecksumCalc[2] = 0x03; 
			break;

		case baud_57600:
			gpsSerial->write(0x04);
			payloadChecksumCalc[2] = 0x04; 
			break;

		case baud_115200:
			gpsSerial->write(0x05);
			payloadChecksumCalc[2] = 0x05; 
			break;
	}

	gpsSerial->write(0x00);

	char checksum = this->ComputeChecksum(payloadChecksumCalc, 4);
	gpsSerial->write(checksum);
	gpsSerial->write(0x0D);
	gpsSerial->write(0x0A);

	while(!gpsSerial->available()){}
	while(gpsSerial->available()){
		gpsSerial->read();
	}
}

void Venus638::SetUpdateRate(int updateRate){

	while(gpsSerial->available()){
		gpsSerial->read();
	}

	byte payloadChecksumCalc[3] = {0x0E, 0x00, 0x00};

	gpsSerial->write(0xA0);
	gpsSerial->write(0xA1);

	gpsSerial->write(0x00);
	gpsSerial->write(0x03);

	gpsSerial->write(0x0E);
	

	switch(updateRate){
		case update_1:
			gpsSerial->write(0x01);
			payloadChecksumCalc[1] = 0x01; 
			break;

		case update_2:
			gpsSerial->write(0x02);
			payloadChecksumCalc[1] = 0x02; 
			break;

		case update_4:
			gpsSerial->write(0x04);
			payloadChecksumCalc[1] = 0x04; 
			break;

		case update_5:
			gpsSerial->write(0x05);
			payloadChecksumCalc[1] = 0x05; 
			break;

		case update_8:
			gpsSerial->write(0x08);
			payloadChecksumCalc[1] = 0x08; 
			break;

		case update_10:
			gpsSerial->write(0x0A);
			payloadChecksumCalc[1] = 0x0A; 
			break;

		case update_20:
			gpsSerial->write(0x14);
			payloadChecksumCalc[1] = 0x14; 
			break;
	}

	gpsSerial->write(0x00);

	char checksum = this->ComputeChecksum(payloadChecksumCalc, 3);
	gpsSerial->write(checksum);
	gpsSerial->write(0x0D);
	gpsSerial->write(0x0A);

	while(!gpsSerial->available()){}
	while(gpsSerial->available()){
		gpsSerial->read();
	}
}

int Venus638::UpdateSpecificData(char * input, int len){

	String ggaTest = "$GPGGA";
	String gllTest = "$GPGLL";	
	String gsaTest = "$GPGSA";
	String gsvTest = "$GPGSV";
	String rmcTest = "$GPRMC";
	String vtgTest = "$GPVTG";

	// copy the data to gga
	if(this->Compare(ggaTest, input, 6)){
		for(int i = 0; i < len; i++){
			gga[i] = input[i];
		}
		ggaLen = len;
		return 0;
	}

	// copy the data to gll
	if(this->Compare(gllTest, input, 6)){
		for(int i = 0; i < len; i++){
			gll[i] = input[i];
		}
		gllLen = len;
		return 1;
	}

	// copy the data to gsa
	if(this->Compare(gsaTest, input, 6)){
		for(int i = 0; i < len; i++){
			gsa[i] = input[i];
		}
		gsaLen = len;
		return 2;
	}

	// copy the data to gsv
	if(this->Compare(gsvTest, input, 6)){
		for(int i = 0; i < len; i++){
			gsv[i] = input[i];
		}
		gsvLen = len;
		return 3;
	}

	// copy the data to rmc
	if(this->Compare(rmcTest, input, 6)){
		for(int i = 0; i < len; i++){
			rmc[i] = input[i];
		}
		rmcLen = len;
		return 4;
	}

	// copy the data to vtg
	if(this->Compare(vtgTest, input, 6)){
		for(int i = 0; i < len; i++){
			vtg[i] = input[i];
		}
		vtgLen = len;
		return 5;
	}
	return false;
}

float Venus638::DMSToDecimal(String DMS){

	// Latitude
	if(DMS.length() == 9){
		String degrees = DMS.substring(0, 2);
		String minutes = DMS.substring(2, 7);

		float degreesFloat = atof(degrees.c_str());
		float minutesFloat = atof(minutes.c_str()) / 60.0f;
		float decimalFloat = degreesFloat + minutesFloat;
		return decimalFloat;
	}

	// Longitude
	else if(DMS.length() == 10){
		String degrees = DMS.substring(0, 3);
		String minutes = DMS.substring(3, 7);

		float degreesFloat = atof(degrees.c_str());
		float minutesFloat = atof(minutes.c_str()) / 60.0f;
		float decimalFloat = degreesFloat + minutesFloat;
		return decimalFloat;

	}
	else{
		return 0.0f;
	}
}

bool Venus638::Compare(char * comp1, char * comp2, int len){
	for(int i = 0; i < len; i++){
		if(!(comp1[i] == comp2[i])){
			return false;
		}
	}
	return true;
}

bool Venus638::Compare(String comp1, char * comp2, int len){
	for(int i = 0; i < len; i++){
		if(!(comp1[i] == comp2[i])){
			return false;
		}
	}
	return true;
}

bool Venus638::Compare(char * comp1, String comp2, int len){
	for(int i = 0; i < len; i++){
		if(!(comp1[i] == comp2[i])){
			return false;
		}
	}
	return true;
}

bool Venus638::Compare(String comp1, String comp2, int len){
	for(int i = 0; i < len; i++){
		if(!(comp1[i] == comp2[i])){
			return false;
		}
	}
	return true;
}

String Venus638::GetDataAfterComma(char * data, int maxDataLen, int numCommas){

	int beginPos = 0;
	int endPos = 0;
	int commaNum = 0;

	// Iterate through all data, looking for commas
	for(int i = 0; i < maxDataLen; i++){
		if(data[i] == ','){

			// Found a comma, increment comma number
			commaNum += 1;
			// Found first comma looking for, start the position of result here.
			if(commaNum == numCommas){
				beginPos = i + 1;
			}

			// Found another comma after first comma.
			if(beginPos > 0 && (commaNum == numCommas + 1)){
				endPos = i;
				break;
			}
		}
	}
	String result = "";
	for(int i = beginPos; i < endPos; i++){
		result += data[i];
	}
	return result;
}

String Venus638::GetDataAfterComma(String data, int maxDataLen, int numCommas){

	int beginPos = 0;
	int endPos = 0;
	int commaNum = 0;

	if(numCommas == 0){
		for(int i = 0; i < maxDataLen; i++){
			if(data[i] == ',' || data[i] == '*'){
				endPos = i;
				break;
			}
		}
		String result = "";
		for(int i = beginPos; i < endPos; i++){
			result += data[i];
		}
		return result;
	}

	// Iterate through all data, looking for commas
	for(int i = 0; i < maxDataLen; i++){
		if(data[i] == ',' || data[i] == '*'){

			// Found a comma, increment comma number
			commaNum += 1;
			// Found first comma looking for, start the position of result here.
			if(commaNum == numCommas){
				beginPos = i + 1;
			}

			// Found another comma after first comma.
			if(beginPos > 0 && (commaNum == numCommas + 1)){
				endPos = i;
				break;
			}
		}
	}
	String result = "";
	for(int i = beginPos; i < endPos; i++){
		result += data[i];
	}
	return result;
}

String Venus638::GetDataAfterCommaGSA(char * data, int maxDataLen, int numCommas){

	int doubleCommaPos = 0;

	// Find double comma.
	for(int i = 0; i < maxDataLen; i++){
		if(data[i] == ',' && data[i+1] == ','){
			doubleCommaPos = i + 1;
			break;
		}
	}

	if(numCommas < 3){
		return this->GetDataAfterComma(data, maxDataLen, numCommas);
	}
	if(numCommas == 3){

		int commaNum = 0;
		int beginPos = 0;

		// Iterate through all data, looking for commas
		for(int i = 0; i < maxDataLen; i++){
			if(data[i] == ',' || data[i] == '*'){

				// Found a comma, increment comma number
				commaNum += 1;
				// Found first comma looking for, start the position of result here.
				if(commaNum == numCommas){
					beginPos = i + 1;
					break;
				}
			}
		}
		String result = "";
		for(int i = beginPos; i < doubleCommaPos - 1; i++){
			result += data[i];
		}
		return result;
	}

	if(numCommas > 3){
		String dataString = "";
		for(int i = 0; i < maxDataLen; i++){
			dataString += data[i];
		}

		String afterDoubleComma = dataString.substring(doubleCommaPos, maxDataLen);

		// Remove leading commas
		int finalBegin = 0;
		for(int i = 0; i < afterDoubleComma.length(); i++){
			if(afterDoubleComma[i] != ','){
				finalBegin = i;
				break;
			}
		}
		String removedLeadingCommas = afterDoubleComma.substring(finalBegin, afterDoubleComma.length());
		return this->GetDataAfterComma(removedLeadingCommas, removedLeadingCommas.length(), numCommas - 4);
	}
}

byte Venus638::ComputeChecksum(byte * payload, int len){

	byte checksum = 0;
	for(int i = 0; i < len; i++){
		checksum ^= payload[i];
	}
	return checksum;
}