#ifndef SERIAL_PORT_CONNECTION_H
#define SERIAL_PORT_CONNECTION_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Serial\SerialController.h"

class SerialWatcherThread;

class SerialPortConnection : public wxFrame {
public:
	SerialPortConnection(wxWindow * parent, SerialController * controller);
	void GetAvailableSerialPorts();
	void UpdateAvailableSerialPortsCombo();

private:
	bool TestPort(std::string portName);
	bool CheckIfPortInList(std::string portName);
	int GetIndexOfPort(std::string portName);
	void OnClose(wxCloseEvent& closeEvent);
	void Connect(wxCommandEvent& WXUNUSED(event));
	wxVector<std::string> allSerialPort;

	wxBoxSizer * layout;

	wxComboBox * serialBox;
	wxComboBox * baudBox;
	wxComboBox * parityBox;
	wxComboBox * dataBox;
	wxComboBox * stopBox;

	wxButton * connectButton;

	SerialWatcherThread * serialWatcher;
	SerialController * serialController;
	bool isSafeToClose;

	enum Actions {
		ID_CONNECT
	};
};

// Thread to keep checking on serial ports
class SerialWatcherThread : wxThread {

public:
	SerialWatcherThread(SerialPortConnection * window);
	void StopThread();

protected:
	virtual ExitCode Entry();

private:
	bool continueWatching;
	SerialPortConnection * serialWindow;

};
#endif