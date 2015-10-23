#ifndef SERIAL_CONTROLLER
#define SERIAL_CONTROLLER

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class SerialController : wxThread{

	public:
		SerialController();
		SerialController(std::string portName, std::string hardwareInfo);
		wxVector<char> GetAllData();

		void StopSerial();

	protected:
		virtual ExitCode Entry();

	private:

		void ReadBuffer(COMSTAT comstat);
		int WriteBuffer(char * dataToWrite);

		HANDLE serialPort;
		wxVector<char> allData;

		DCB dcb;
		COMMTIMEOUTS timeouts;
		DWORD commEvents;

		// events that fire the wait for multiple objects
		HANDLE writeEvent;
		HANDLE shutdownEvent;
		HANDLE  events[3];
		
};

#endif