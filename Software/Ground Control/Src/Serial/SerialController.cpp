#include "SerialController.h"

SerialController::SerialController() : wxThread(wxTHREAD_DETACHED){}

SerialController::SerialController(std::string portName, std::string hardwareInfo) : wxThread(wxTHREAD_DETACHED){

	std::wstring serialName(portName.length(), L' ');
	std::copy(portName.begin(), portName.end(), serialName.begin());

	std::wstring dcbWideStr(hardwareInfo.length(), L' ');
	std::copy(hardwareInfo.begin(), hardwareInfo.end(), dcbWideStr.begin());
	
	// Open the serial port
	serialPort = ::CreateFile(serialName.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, 0, OPEN_EXISTING, NULL, 0);
	
	if (serialPort == INVALID_HANDLE_VALUE){
		const DWORD error = GetLastError();
		OutputDebugStringA("Serial Port Failed Connecting");
		return;
	}
	else {
		OutputDebugStringA("Serial Port Connected!");
	}

	shutdownEvent = NULL;
	if (shutdownEvent != NULL) {
		ResetEvent(shutdownEvent);
	}
	shutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	

	// set the timeout values
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;

	// configure
	DWORD dwCommEvents;
	if(!SetCommTimeouts(serialPort, &timeouts)) {
		OutputDebugStringA("Set Comm Timeout Fail");
		return;
	}

	if (!SetCommMask(serialPort, EV_RXCHAR | EV_TXEMPTY | EV_RXFLAG)) {
		OutputDebugStringA("Set Comm Mask Failed");
		return;
	}

	//dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
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
	
	OutputDebugStringA("Done configuring");

	// Notify us when the serial port finishes sending data, or when data is recieved
	//SetCommMask(serialPort, EV_TXEMPTY | EV_RXCHAR);

	// Clear the serial port
	PurgeComm(serialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	this->Run();
}

wxThread::ExitCode SerialController::Entry() {

	// Clear the serial port
	if (serialPort) {
		PurgeComm(serialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	DWORD resultType = 0;
	DWORD eventType = 0;
	DWORD errorCode = 0;
	COMSTAT status = {0};
	memset(&status, 0, sizeof(COMSTAT));

	while (true) {

		OutputDebugStringA("Wating on serial event...");
		BOOL waitResult = WaitCommEvent(serialPort, &resultType, NULL);

		if (waitResult) {

			ClearCommError(serialPort, &errorCode, &status);

			DWORD eventType = 0;
			GetCommMask(serialPort, &eventType);

			// Recieved data, read the buffer
			if (eventType & EV_RXCHAR) {
				ReadBuffer(status);
			}
		}
		else {
		}
	}
}

wxVector<char> SerialController::GetAllData(){
	return wxVector<char>(allData);
}

void SerialController::ReadBuffer(COMSTAT status){

	DWORD bytesToRead = status.cbInQue;
	char buffer[1024] = { 0 };
	DWORD bytesRead = 0;

	if (ReadFile(serialPort, &buffer, bytesToRead, &bytesRead, NULL)) {

		for (int i = 0; i < bytesRead; i++) {
			allData.push_back(buffer[i]);
		}
		//OutputDebugStringA(std::string(buffer).c_str());
	}

}

int SerialController::WriteBuffer(char * dataToWrite) {

	DWORD bytesWritten = 0;
	if (!WriteFile(serialPort, &dataToWrite, sizeof(dataToWrite), &bytesWritten, NULL)) {
		return GetLastError();
	}

}

void SerialController::StopSerial() {

	SetEvent(shutdownEvent);
}