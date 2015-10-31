#include "SerialPortConnection.h"

SerialPortConnection::SerialPortConnection(wxWindow * parent) : wxFrame(parent, -1, "Serial Port Connection") {
	this->SetBackgroundColour(wxColor(45, 45, 45));

	serialBox = new wxComboBox(this, -1);
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(serialBox);
	serialWatcher = new SerialWatcherThread(this);
}

void SerialPortConnection::GetAvailableSerialPorts() {

	// Iterate through first 50 serial ports, testing each one to see if its active
	for (int i = 0; i < 50; i++) {

		// Generate serial port name
		std::string portName = "COM" + std::to_string(i);

		// This is a good serial port
		if (this->TestPort(portName)){

			// add it to the list if it's not already there
			if(!this->CheckIfPortInList(portName)){
				allSerialPort.push_back(portName);
			}
		}

		// Not a good serial port
		else {
			// remove it from the list if it's there
			if (this->CheckIfPortInList(portName)){
				int indexToRemove = this->GetIndexOfPort(portName);
				allSerialPort.erase(allSerialPort.begin() + indexToRemove);
			}
		}
	}
}

bool SerialPortConnection::TestPort(std::string portName) {

	// Convert input port name to wide string
	std::wstring serialName(portName.length(), L' ');
	std::copy(portName.begin(), portName.end(), serialName.begin());

	// Open the serial port
	HANDLE serialPort = ::CreateFile(serialName.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, 0, OPEN_EXISTING, NULL, 0);

	if (serialPort == INVALID_HANDLE_VALUE) {
		CloseHandle(serialPort);
		return false;
	}
	else {
		CloseHandle(serialPort);
		return true;
	}
}

bool SerialPortConnection::CheckIfPortInList(std::string portName) {
	for (int i = 0; i < allSerialPort.size(); i++) {
		if (portName == allSerialPort[i]){
			return true;
		}
	}
	return false;
}

int SerialPortConnection::GetIndexOfPort(std::string portName) {
	for (int i = 0; i < allSerialPort.size(); i++) {
		if (portName == allSerialPort[i]) {
			return i;
		}
	}
	return -1;
}

void SerialPortConnection::UpdateAvailableSerialPortsCombo() {

	// Add all serial ports not yet in the combo box, to the combo box
	for (int i = 0; i < allSerialPort.size(); i++) {
		if (wxNOT_FOUND == serialBox->FindString(wxString(allSerialPort[i]))) {
			serialBox->AppendString(wxString(allSerialPort[i]));
		}
	}

	// If port is in combo box but no longer in the actual list, remove it from list
	for (int i = 0; i <serialBox->GetCount(); i++) {
		if (!CheckIfPortInList(std::string(serialBox->GetString(i)))) {
			serialBox->Delete(i);
		}
	}
}


SerialWatcherThread::SerialWatcherThread(SerialPortConnection * window) : wxThread(wxTHREAD_DETACHED) {
	serialWindow = window;
	this->Run();
}

wxThread::ExitCode SerialWatcherThread::Entry() {

	while (true) {
		serialWindow->GetAvailableSerialPorts();
		serialWindow->UpdateAvailableSerialPortsCombo();
		this->Sleep(500);
	}
}