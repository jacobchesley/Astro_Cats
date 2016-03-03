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
		void Connect(std::string portName, std::string hardwareInfo);
		void Disconnect();

		wxString GetSerialPortName();

		wxVector<char> GetDataStartingAtIndex();
		void ClearReadData();
		wxVector<char> GetAllData();
		void ClearAllData();

		void StopSerial();
		void Stop();
		bool IsConnected();

	protected:
		virtual ExitCode Entry();

	private:

		void ReadBuffer(COMSTAT comstat);
		int WriteBuffer(char * dataToWrite);
		wxString serialPortName;

		#ifdef _WIN32
			HANDLE serialPort;
			DCB dcb;
			COMMTIMEOUTS timeouts;
			DWORD commEvents;
			HANDLE shutdownEvent;

		#elif __APPLE__
			int serialPort;
			std::string GetStringAfterComma(std::string inString, int numCommas);
		#endif

		wxVector<char> allData;
		
		bool isConnected;
		long int currentIndex;
		bool stop;
};

#endif