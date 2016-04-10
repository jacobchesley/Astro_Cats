#include "SerialController.h"

SerialController::SerialController() : wxThread(wxTHREAD_DETACHED){
	isConnected = false;
	threadRunning = false;
	currentIndex = 0;
}

SerialController::SerialController(std::string portName, std::string hardwareInfo) : wxThread(wxTHREAD_DETACHED){
	isConnected = false;
	threadRunning = false;
	this->Connect(portName, hardwareInfo);
	currentIndex = 0;
	this->Run();
}

void SerialController::Connect(std::string portName, std::string hardwareInfo) {

	stop = false;
	// Close current connection if it exists
	if (isConnected) {

		// Close serial port in Windows
		#ifdef _WIN32
			
			return;
			//CloseHandle(serialPort);

		// Close serial port in OSX
		#elif __APPLE__
			close(serialPort)
		#endif
	}

	// Open the serial port in Windows
	#ifdef _WIN32
		// Copy port name from string to wide string
		std::wstring serialName(portName.length(), L' ');
		std::copy(portName.begin(), portName.end(), serialName.begin());

		// Copy hardware info from string to wide string
		std::wstring dcbWideStr(hardwareInfo.length(), L' ');
		std::copy(hardwareInfo.begin(), hardwareInfo.end(), dcbWideStr.begin());

		// Create the serial port
		serialPort = ::CreateFile(serialName.c_str(), GENERIC_READ | GENERIC_WRITE,
			0, 0, OPEN_EXISTING, NULL, 0);

		// Exit, we could not connect!
		if (serialPort == INVALID_HANDLE_VALUE) {
			isConnected = false;
			return;
		}
		serialPortName = portName;
		isConnected = true;
		shutdownEvent = NULL;
		if (shutdownEvent != NULL) {
			ResetEvent(shutdownEvent);
		}
		shutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		// set the timeout values
		timeouts.ReadIntervalTimeout = 10;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 0;
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 0;

		if (!SetCommTimeouts(serialPort, &timeouts)) {
			OutputDebugStringA("Set Comm Timeout Fail");
			return;
		}

		if (!SetCommMask(serialPort, EV_RXCHAR | EV_TXEMPTY | EV_RXFLAG | EV_ERR | EV_BREAK)) {
			OutputDebugStringA("Set Comm Mask Failed");
			return;
		}

		memset(&dcb, 0, sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		if (!BuildCommDCB(dcbWideStr.c_str(), &dcb)) {
			OutputDebugStringA("Build Comm DCB Failed");
			return;
		}

		if (!SetCommState(serialPort, &dcb)) {
			OutputDebugStringA("Set Com State Failed");
			return;
		}

		if (!GetCommState(serialPort, &dcb)) {
			OutputDebugStringA("Get Comm State Failed");
			return;
		}

		isConnected = true;

		// Clear the serial port
		PurgeComm(serialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// Open the serial port in OSX
	#elif __APPLE__
		// Create the serial port
		serialPort = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

		// Exit, we could not connect!
		if (serialPort == -1) {
			isConnected = false;
			return;
		}

		// Open the device in nonblocking mode
		fcntl(fd, F_SETFL, FNDELAY);

		// Get and clear options for the serial port
		struct termios options;
		tcgetattr(serialPort, &options);
		bzero(&options, sizeof(options));

		
		// Convert input hardware info string into baud rate integer
		std::string baudString = GetStringAfterComma(hardwareInfo, 0);
		int baudRateInt = atoi(baudString.c_str());

		// Switch through and set speed baud
		speed_t baud;
		switch (baudRateInt) {
			case 110: baud = B110; break;
			case 300: baud = B300; break;
			case 600: baud = B600; break;
			case 1200: baud = B1200; break;
			case 2400: baud = B2400; break;
			case 4800: baud = B4800; break;
			case 9600: baud = B9600; break;
			case 14400: baud = B14400; break;
			case 19200: baud = B19200; break;
			case 38400:	baud = B38400; break;
			case 57600: baud = B57600; break;
			case 115200: baud = B115200; break;
			case 128000: baud = B128000; break;
			case 256000: baud = B256000; break;
		}

		// Set baud rate to options
		cfsetispeed(&options, baud);
		cfsetospeed(&options, baud);

		options.c_cc[VTIME] = 0;
		options.c_cc[VMIN] = 0;

		// Set options to serial port
		tcsetattr(serialPort, TCSANOW, &options);

	#endif
	if (!threadRunning) {
		this->Run();
	}
}

void SerialController::Disconnect() {
	// Close current connection if it exists
	if (isConnected) {

		isConnected = false;

		// Close serial port in Windows
		#ifdef _WIN32

			if (serialPort != INVALID_HANDLE_VALUE) {
				CloseHandle(serialPort);
			}

		// Close serial port in OSX
		#elif __APPLE__
			close(serialPort)
		#endif
	}
}
wxThread::ExitCode SerialController::Entry() {

	threadRunning = true;

	// Clear the serial port
	#ifdef _WIN32
		if (serialPort){
			PurgeComm(serialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		}
	#elif __APPLE__
		if (serialPort > 0) {

		}
	#endif

	#ifdef _WIN32

		DWORD errorCode = 0;
		COMSTAT status = {0};
		memset(&status, 0, sizeof(COMSTAT));

	#elif __APPLE__

	#endif

	while (true) {

		if (isConnected == false) {
			this->Sleep(100);
			continue;
		}

		#ifdef _WIN32

			ClearCommError(serialPort, &errorCode, &status);
			ReadBuffer();
				
		#elif __APPLE__

		#endif
	}
}

bool SerialController::IsConnected() {
	return isConnected;
}

wxString SerialController::GetSerialPortName() {
	return serialPortName;
}

wxVector<char> SerialController::GetDataStartingAtIndex() {

	wxVector<char> output;
	int endIndex = allData.size();
	for (int i = currentIndex; i < (int)allData.size(); i++) {
		output.push_back(allData[i]);
	}
	currentIndex = endIndex;
	return output;
}


wxVector<char> SerialController::GetAllData(){
	return wxVector<char>(allData);
}

void SerialController::ClearAllData(){
	allData.clear();
}

void SerialController::ClearReadData() {
	allData.erase(allData.begin(), allData.begin() + currentIndex);
	currentIndex = 0;
}

void SerialController::WriteData(wxString data) {

	DWORD bytesWritten = 0;
	char buffer[16384];
	for (int i = 0; i < (int)data.Length(); i++) {
		buffer[i] = data[i];
	}
	WriteFile(serialPort, &buffer, data.Length(), &bytesWritten, NULL);
}

void SerialController::ReadBuffer(){

	char buffer[16384] = { 0 };
	memset(buffer, 0, 16384);

	#ifdef _WIN32

		// Get number of bytes to read
		DWORD bytesRead = 0;

		// Read all bytes
		if (ReadFile(serialPort, buffer, 16384, &bytesRead, NULL)) {

			// Push data onto vector
			for (int i = 0; i < (int)bytesRead; i++) {
				allData.push_back(buffer[i]);
			}
		}
	#elif __APPLE__
		
		// Get number of bytes to read
		int bytesToRead = 0;
		ioctl(serialPort, FIONREAD, &bytesToRead);

		if (bytesToRead > 0) {

			// Read all bytes
			int bytesRead = read(serialPort, buffer, bytesToRead);

			// Push data onto vector
			for (int i = 0; i < bytesRead; i++) {
				allData.push_back(buffer[i]);
			}
		}
	#endif
}

int SerialController::WriteBuffer(char * dataToWrite) {

	#ifdef _WIN32

		DWORD bytesWritten = 0;
		if (!WriteFile(serialPort, &dataToWrite, sizeof(dataToWrite), &bytesWritten, NULL)) {
			return GetLastError();
		}

	#elif __APPLE__

	#endif
	return 0;
}

void SerialController::StopSerial() {

	SetEvent(shutdownEvent);
}

void SerialController::Stop() {
	stop = true;
}

bool SerialController::TestPort(wxString portName) {
	return true;
}
#ifdef __APPLE__
std::string SerialController::GetStringAfterComma(std::string inString, int numCommas) {

	int beginPos = 0;
	int endPos = 0;
	int commaNum = 0;

	// Data up until first comma
	if (numCommas == 0) {

		// Find position of the first comma
		for (int i = 0; i < inString.length(); i++) {
			if (inString[i] == ',') {
				endPos = i;
				break;
			}
		}

		return inString.substr(0, endPos);
	}

	// Iterate through all data, looking for commas
	for (int i = 0; i < inString.length(); i++) {
		if (inString[i] == ',') {

			// Found a comma, increment comma number
			commaNum += 1;

			// Found first comma looking for, start the position of result here.
			if (commaNum == numCommas) {
				beginPos = i + 1;
			}

			// Found another comma after first comma.
			if (beginPos > 0 && (commaNum == numCommas + 1)) {
				endPos = i;
			}
		}
	}

	// Get differnece to calculate string length
	int length = endPos = beginPos;
	return inString.substr(beginPos, length);
}
#endif