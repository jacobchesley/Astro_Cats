#include "SerialController.h"

SerialController::SerialController() : wxThread(wxTHREAD_DETACHED){}

SerialController::SerialController(std::string portName, std::string hardwareInfo) : wxThread(wxTHREAD_DETACHED){

	std::wstring serialName(portName.length(), L' ');
	std::copy(portName.begin(), portName.end(), serialName.begin());

	std::wstring dcbWideStr(hardwareInfo.length(), L' ');
	std::copy(hardwareInfo.begin(), hardwareInfo.end(), dcbWideStr.begin());
	
	// Open the serial port
	serialPort = ::CreateFile(serialName.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	
	if (serialPort == INVALID_HANDLE_VALUE){
		const DWORD error = GetLastError();
		OutputDebugStringA("Serial Port Failed Connecting");
	}
	else {
		OutputDebugStringA("Serial Port Connected!");
	}

	// create events
	overLapped = { 0 };
	overLapped.Offset = 0;
	overLapped.OffsetHigh = 0;

	overLapped.hEvent = NULL;
	writeEvent = NULL;
	shutdownEvent = NULL;

	overLapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overLapped.hEvent == NULL) {
		OutputDebugStringA("Issue creating Overlapped event");
	}

	if (writeEvent != NULL) {
		ResetEvent(writeEvent);
	}
	writeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (shutdownEvent != NULL) {
		ResetEvent(shutdownEvent);
	}
	shutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialize the event objects
	events[0] = shutdownEvent;	// highest priority
	events[1] = overLapped.hEvent;
	events[2] = writeEvent;

	// set the timeout values
	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 1000;
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 1000;
	timeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	DWORD dwCommEvents;
	if(!SetCommTimeouts(serialPort, &timeouts)) {
		OutputDebugStringA("Set Comm Timeout Fail");
	}

	if (!SetCommMask(serialPort, EV_RXCHAR | EV_TXEMPTY | EV_RXFLAG)) {
		OutputDebugStringA("Set Comm Mask Failed");
	}

	//dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
	if (!BuildCommDCB(dcbWideStr.c_str(), &dcb)) {
		OutputDebugStringA("Build Comm DCB Failed");
	}

	if (!SetCommState(serialPort, &dcb)) {
		OutputDebugStringA("Set Com State Failed");
	}

	if (!GetCommState(serialPort, &dcb)) {
		OutputDebugStringA("Get Comm State Failed");
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
	DWORD errorCode = 0;
	DWORD readEvent = 0;
	COMSTAT comstat;
	memset(&comstat, 0, sizeof(COMSTAT));

	while (true) {

		BOOL waitResult = WaitCommEvent(serialPort, &resultType, &overLapped);
		
		if (!waitResult) {

			// Switch the the error codes that are common
			switch (errorCode = GetLastError()) {

				// No bytes to read
			case ERROR_IO_PENDING:
				//OutputDebugStringA("No bytes to read");
				break;

				// Incorrect Parameter
			case 87:
				//OutputDebugStringA("Error 87?");
				break;

				// Other error
			default:
				//OutputDebugStringA("OH NO! Error - ");
				//OutputDebugStringA(std::to_string(errorCode).c_str());
				break;
			}
		}
		else {
			// Clear the com error code
			ClearCommError(serialPort, &errorCode, &comstat);

			// No bytes to read yet
			if (comstat.cbInQue == 0) {
				//OutputDebugStringA("No bytes to read again");
				continue;
			}
		}

		//OutputDebugStringA("Waiting for objects...");
		DWORD serialEvent = WaitForMultipleObjects(3, events, FALSE, INFINITE);
		//OutputDebugStringA("Objects fired!");

		// Switch through the events that could have occrued
		switch (serialEvent) {

			// Shutdown Event
			case 0:
				//OutputDebugStringA("Shutdown");
				return 0;

			// Read Event
			case 1:

				GetCommMask(serialPort, &readEvent);
				//OutputDebugStringA("Reading!!!");

				if (readEvent & EV_RXCHAR) {

					this->RecieveChar(comstat);

				}
				break;

			// Write Event
			case 2:
				//OutputDebugStringA("Writing!");
				break;
		}
	}
	return 0;
}

wxVector<char> SerialController::GetAllData(){
	return wxVector<char>(allData);
}

void SerialController::RecieveChar(COMSTAT comstat){

	BOOL  continueRead = TRUE;
	BOOL  readResult = TRUE;
	DWORD errorCode = 0;
	DWORD bytesRead = 0;
	const DWORD reciverBufferSize = 512;
	char recieveBuffer[512] = "";

	while(true){

		ClearCommError(serialPort, &errorCode, &comstat);

		if (comstat.cbInQue == 0){
			// break out when all bytes have been read
			OutputDebugStringA("Comstat in queue is empty, all bytes have been read");
			break;
		}


		if (continueRead){

			// Read 1 byte from RX Buffer
			OutputDebugStringA("Reading from buffer!");
			readResult = ReadFile(serialPort, recieveBuffer, reciverBufferSize, &bytesRead, &overLapped);
			
			OutputDebugStringA("Num Bytes Read");
			OutputDebugStringA(std::string(std::to_string(bytesRead)).c_str());

			for (int i = 0; i < bytesRead; i++) {
				allData.push_back(recieveBuffer[i]);
			}

			// Did read fail?
			if (!readResult){
				switch (errorCode = GetLastError()){
				case ERROR_IO_PENDING:
				
					// asynchronous i/o is still in progress 
					// Proceed on to GetOverlappedResults();
					OutputDebugStringA("ReadFile - IO PENDING");
					continueRead = FALSE;

					readResult = GetOverlappedResult(serialPort, &overLapped, &bytesRead, FALSE);

					// Did overlap result fail?
					if (!readResult)
					{
						OutputDebugStringA("Overlapped Result failed..");
						switch (errorCode = GetLastError()) {
						case ERROR_IO_INCOMPLETE:
							OutputDebugStringA("INCOMPLETE!!!!");
							CancelIoEx(serialPort, &overLapped);
						}
					}

					break;
				
				default:
				
					// Another error has occured.  Process this error.
					OutputDebugStringA("ReadFile - Other Error:");
					OutputDebugStringA(std::string(std::to_string(errorCode)).c_str());
					break;
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				OutputDebugStringA("Read Was Good");
				continueRead = TRUE;
			}
		}  // close if (bRead)


		if (!continueRead)
		{
			continueRead = TRUE;
			readResult = GetOverlappedResult(serialPort, &overLapped, &bytesRead, TRUE);

			// Did overlap result fail?
			if (!readResult)
			{

			}
		} 

	} 

	OutputDebugStringA("###########################Data So Far#########################");
	std::string temp = "";
	for (int i = 0; i < allData.size(); i++) {
		temp += allData[i];
	}
	OutputDebugStringA(temp.c_str());
}

void SerialController::StopSerial() {

	SetEvent(shutdownEvent);
}