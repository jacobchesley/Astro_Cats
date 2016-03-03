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
	void StopThread();

private:
	bool TestPort(std::string portName);
	bool CheckIfPortInList(std::string portName);
	int GetIndexOfPort(std::string portName);
	void OnClose(wxCloseEvent& closeEvent);
	void Connect(wxCommandEvent& WXUNUSED(event));
	void Disconnect(wxCommandEvent& WXUNUSED(event));
	wxVector<std::string> allSerialPort;

	wxBoxSizer * layout;

	wxBoxSizer * serialBoxLayout;
	wxStaticText * serialBoxLabel;
	wxComboBox * serialBox;

	wxBoxSizer * baudBoxLayout;
	wxStaticText * baudBoxLabel;
	wxComboBox * baudBox;

	wxBoxSizer * parityBoxLayout;
	wxStaticText * parityBoxLabel;
	wxComboBox * parityBox;

	wxBoxSizer * dataBoxLayout;
	wxStaticText * dataBoxLabel;
	wxComboBox * dataBox;

	wxBoxSizer * stopBoxLayout;
	wxStaticText * stopBoxLabel;
	wxComboBox * stopBox;

	wxBoxSizer * dtrBoxLayout;
	wxStaticText * dtrBoxLabel;
	wxComboBox * dtrBox;

	wxBoxSizer * buttonLayout;
	wxButton * connectButton;
	wxButton * disconnectButton;

	SerialWatcherThread * serialWatcher;
	SerialController * serialController;
	bool isSafeToClose;

	enum Actions {
		ID_CONNECT,
		ID_DISCONNECT
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