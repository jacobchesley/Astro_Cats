#include "SerialPortConnection.h"

SerialPortConnection::SerialPortConnection(wxWindow * parent, SerialController * controller) : wxFrame(parent, -1, "Serial Port Connection") {

	serialController = controller;

	this->SetBackgroundColour(wxColor(45, 45, 45));

	// Create serial port selection row
	serialBoxLayout = new wxBoxSizer(wxHORIZONTAL);
	serialBoxLabel = new wxStaticText(this, -1, _("Available Serial Ports "));
	serialBoxLabel->SetBackgroundColour(this->GetBackgroundColour());
	serialBoxLabel->SetForegroundColour(wxColor(255, 255, 255));
	serialBox = new wxComboBox(this, -1);
	serialBoxLayout->Add(serialBoxLabel);
	serialBoxLayout->Add(serialBox);

	int maxWidth = serialBoxLabel->GetSize().GetWidth();

	// Create baud rate selection row
	baudBoxLayout = new wxBoxSizer(wxHORIZONTAL);
	baudBoxLabel = new wxStaticText(this, -1, _("Baud Rate"));
	baudBoxLabel->SetBackgroundColour(this->GetBackgroundColour());
	baudBoxLabel->SetForegroundColour(wxColor(255, 255, 255));

	// Create baud rate combo box and append choices.
	baudBox = new wxComboBox(this, -1);
	baudBox->AppendString(_T("110"));
	baudBox->AppendString(_T("300"));
	baudBox->AppendString(_T("600"));
	baudBox->AppendString(_T("1200"));
	baudBox->AppendString(_T("2400"));
	baudBox->AppendString(_T("4800"));
	baudBox->AppendString(_T("9600"));
	baudBox->AppendString(_T("14400"));
	baudBox->AppendString(_T("19200"));
	baudBox->AppendString(_T("38400"));
	baudBox->AppendString(_T("57600"));
	baudBox->AppendString(_T("115200"));
	baudBox->AppendString(_T("128000"));
	baudBox->AppendString(_T("256000"));
	baudBox->SetValue(_T("115200"));

	baudBoxLayout->Add(baudBoxLabel);
	int space = maxWidth - baudBoxLabel->GetSize().GetWidth();
	baudBoxLayout->AddSpacer(space);
	baudBoxLayout->Add(baudBox);

	// Create parity bit selection row
	parityBoxLayout = new wxBoxSizer(wxHORIZONTAL);
	parityBoxLabel = new wxStaticText(this, -1, _("Parity"));
	parityBoxLabel->SetBackgroundColour(this->GetBackgroundColour());
	parityBoxLabel->SetForegroundColour(wxColor(255, 255, 255));

	// Create parity bit combo box and append choices
	parityBox = new wxComboBox(this, -1);
	parityBox->AppendString(_T("none"));
	parityBox->AppendString(_T("even"));
	parityBox->AppendString(_T("odd"));
	parityBox->AppendString(_T("mark"));
	parityBox->AppendString(_T("space"));
	parityBox->SetValue(_T("none"));

	parityBoxLayout->Add(parityBoxLabel);
	space = maxWidth - parityBoxLabel->GetSize().GetWidth();
	parityBoxLayout->AddSpacer(space);
	parityBoxLayout->Add(parityBox);

	// Create byte size selection row
	dataBoxLayout = new wxBoxSizer(wxHORIZONTAL);
	dataBoxLabel = new wxStaticText(this, -1, _("Byte Size"));
	dataBoxLabel->SetBackgroundColour(this->GetBackgroundColour());
	dataBoxLabel->SetForegroundColour(wxColor(255, 255, 255));

	// Create byte size box
	dataBox = new wxComboBox(this, -1);
	dataBox->AppendString(_T("7"));
	dataBox->AppendString(_T("8"));
	dataBox->SetValue(_T("8"));

	dataBoxLayout->Add(dataBoxLabel);
	space = maxWidth - dataBoxLabel->GetSize().GetWidth();
	dataBoxLayout->AddSpacer(space);
	dataBoxLayout->Add(dataBox);

	// Create stop bits selection row
	stopBoxLayout = new wxBoxSizer(wxHORIZONTAL);
	stopBoxLabel = new wxStaticText(this, -1, _("Stop Bits"));
	stopBoxLabel->SetBackgroundColour(this->GetBackgroundColour());
	stopBoxLabel->SetForegroundColour(wxColor(255, 255, 255));

	// Create stop bit box
	stopBox = new wxComboBox(this, -1);
	stopBox->AppendString(_T("1"));
	stopBox->AppendString(_T("2"));
	stopBox->SetValue(_T("1"));

	stopBoxLayout->Add(stopBoxLabel);
	space = maxWidth - stopBoxLabel->GetSize().GetWidth();
	stopBoxLayout->AddSpacer(space);
	stopBoxLayout->Add(stopBox);

	connectButton = new wxButton(this, SerialPortConnection::Actions::ID_CONNECT, _T("Connect"));

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	// Add all of the layouts created above to the window
	this->GetSizer()->Add(serialBoxLayout);
	this->GetSizer()->AddSpacer(10);
	this->GetSizer()->Add(baudBoxLayout);
	this->GetSizer()->AddSpacer(10);
	this->GetSizer()->Add(parityBoxLayout);
	this->GetSizer()->AddSpacer(10);
	this->GetSizer()->Add(dataBoxLayout);
	this->GetSizer()->AddSpacer(10);
	this->GetSizer()->Add(stopBoxLayout);
	this->GetSizer()->AddSpacer(30);
	this->GetSizer()->Add(connectButton);

	serialWatcher = new SerialWatcherThread(this);
	isSafeToClose = false;

	this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&SerialPortConnection::Connect, this, SerialPortConnection::Actions::ID_CONNECT);

	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&SerialPortConnection::OnClose, this);

	this->SetIcon(wxIcon("IDI_ICON1"));
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
	for (int i = 0; i < (int)allSerialPort.size(); i++) {
		if (portName == allSerialPort[i]){
			return true;
		}
	}
	return false;
}

int SerialPortConnection::GetIndexOfPort(std::string portName) {
	for (int i = 0; i < (int)allSerialPort.size(); i++) {
		if (portName == allSerialPort[i]) {
			return i;
		}
	}
	return -1;
}

void SerialPortConnection::UpdateAvailableSerialPortsCombo() {

	// Add all serial ports not yet in the combo box, to the combo box
	for (int i = 0; i < (int)allSerialPort.size(); i++) {
		if (wxNOT_FOUND == serialBox->FindString(wxString(allSerialPort[i]))) {
			serialBox->AppendString(wxString(allSerialPort[i]));
		}
	}

	// If port is in combo box but no longer in the actual list, remove it from list
	for (int i = 0; i < (int)serialBox->GetCount(); i++) {
		if (!CheckIfPortInList(std::string(serialBox->GetString(i)))) {
			serialBox->Delete(i);
		}
	}
	serialBox->SetSelection(serialBox->GetCount());
}

void SerialPortConnection::Connect(wxCommandEvent& WXUNUSED(event)) {

	std::string baud = baudBox->GetValue().ToStdString();
	std::string parity = parityBox->GetValue().ToStdString();
	std::string data = dataBox->GetValue().ToStdString();
	std::string stop = stopBox->GetValue().ToStdString();

	std::string hardwareInfo = "baud=" + baud + " parity=" + parity[0] + " data=" + data + " stop=" + stop;
	serialController->Connect(std::string(serialBox->GetValue().ToStdString()), hardwareInfo);
}

void SerialPortConnection::OnClose(wxCloseEvent& closeEvent){
	this->Hide();
	closeEvent.Veto();
}

void SerialPortConnection::StopThread() {

	// Send signal to stop thread, and wait for the thread to notify window it is safe to close.
	serialWatcher->StopThread();

}

SerialWatcherThread::SerialWatcherThread(SerialPortConnection * window) : wxThread(wxTHREAD_DETACHED) {
	serialWindow = window;
	this->Run();
	continueWatching = true;
}

void SerialWatcherThread::StopThread() {
	continueWatching = false;
}
wxThread::ExitCode SerialWatcherThread::Entry() {

	while (continueWatching) {
		serialWindow->GetAvailableSerialPorts();
		serialWindow->UpdateAvailableSerialPortsCombo();
		this->Sleep(100);
	}
	return 0;
}