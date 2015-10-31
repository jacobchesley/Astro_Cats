#ifndef SERIAL_PORT_CONNECTION_H
#define SERIAL_PORT_CONNECTION_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class SerialWatcherThread;

class SerialPortConnection : public wxFrame {
public:
	SerialPortConnection(wxWindow * parent);
	void GetAvailableSerialPorts();
	void UpdateAvailableSerialPortsCombo();

private:
	bool TestPort(std::string portName);
	bool CheckIfPortInList(std::string portName);
	int GetIndexOfPort(std::string portName);
	wxVector<std::string> allSerialPort;

	wxBoxSizer * layout;
	wxComboBox * serialBox;

	SerialWatcherThread * serialWatcher;
};

// Thread to keep checking on serial ports
class SerialWatcherThread : wxThread {

public:
	SerialWatcherThread(SerialPortConnection * window);

protected:
	virtual ExitCode Entry();

private:
	SerialPortConnection * serialWindow;

};
#endif