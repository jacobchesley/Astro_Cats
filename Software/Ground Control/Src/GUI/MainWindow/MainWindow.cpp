#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(NULL, -1, "Astro Cats Ground Control", wxDefaultPosition, wxSize(800,600)){

	// Create and add menu bar with menus
	wxMenuBar * menuBar = new wxMenuBar;
	wxMenu * menuFile = new wxMenu;
	wxMenu * menuSerial = new wxMenu;
	wxMenu * menuPIL = new wxMenu;
	wxMenu * menuHelp = new wxMenu;

	// Serial Menu
	menuSerial->Append(MainWindow::MenuBar::ID_CONNECT_SERIAL, _("Connect to a Serial Port"));

	// Pil Menu
	menuPIL->Append(MainWindow::MenuBar::ID_SEND_PIL_COMMAND, _("Send Commands to the PIL"));
	menuPIL->AppendSeparator();
	menuPIL->Append(MainWindow::MenuBar::ID_READ_PIL_STATUS, _("Get PIL Status"));

	// Help Menu
	menuHelp->Append(MainWindow::MenuBar::ID_DOC, _("Documentation"));
	menuHelp->Append(MainWindow::MenuBar::ID_ABOUT, _("About"));

	// Append menus to menu bar
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSerial, _("&Serial Connections"));
	menuBar->Append(menuPIL, _("&PIL"));
	menuBar->Append(menuHelp, _("&Help"));

	// Set the menu bar
	this->SetMenuBar(menuBar);

	// Set the status bar
	this->CreateStatusBar();

	this->SetIcon(wxIcon("IDI_ICON1"));

	serialController = new SerialController();
	
	radioSignalAndDataSplitter = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
	radioSignalAndDataSplitter->SetSashGravity(0.5);
	radioSignalAndDataSplitter->SetMinimumPaneSize(50);

	// Set up radio signal strength panels in their own splitter window
	radioSplitter = new wxSplitterWindow(radioSignalAndDataSplitter, -1, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
	radioSplitter->SetSashGravity(0.5);
	radioSplitter->SetMinimumPaneSize(20);

	radioSignalStrengthPil = new RadioSignalStrength(radioSplitter, "Signal From PIL");
	radioSignalStrengthRocket = new RadioSignalStrength(radioSplitter, "Signal From Rocket");
	radioSplitter->SplitVertically(radioSignalStrengthPil, radioSignalStrengthRocket);

	dataWindow = new IncomingDataStream(radioSignalAndDataSplitter, "Radio Data");
	radioSignalAndDataSplitter->SplitHorizontally(radioSplitter, dataWindow);

	// Set main layout
	mainLayout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainLayout);

	// Add items to main layout
	this->GetSizer()->Add(radioSignalAndDataSplitter, 1, wxEXPAND, 0);
	this->GetSizer()->Layout();


	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSerialConnection, this, MainWindow::MenuBar::ID_CONNECT_SERIAL);

	hexToJpeg = new HexToJpeg();

	uiUpdater = new UIUpdateThread(this);
}

void MainWindow::UpdateData(int dataParameter, int dataValue){

	switch (dataParameter) {
		case MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_PIL:
			radioSignalStrengthPil->SetRadioSignalStrength(dataValue);
			break;

		case MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_TRACKING:
			radioSignalStrengthRocket->SetRadioSignalStrength(dataValue);
			break;
	}
}

void MainWindow::ReciveSerialData(wxString serialData){

	dataWindow->AppendText(serialData);

	if (serialData == "END") {
		hexToJpeg->WriteJpegFile("C:\\Development\\UC Rocketry\\Nasa SLI\\Software\\Ground Control\\Build\\Release\\Test.jpeg");
		OutputDebugStringA("Wrote to file");
		return;
	}

	char jpegData[16384];
	for (int i = 0; i < serialData.size(); i++) {
		jpegData[i] = serialData[i];
	}
	hexToJpeg->AppendHex(jpegData, serialData.size());
}

SerialController * MainWindow::GetSerialController() {
	return serialController;
}

void MainWindow::ShowSerialConnection(wxCommandEvent& WXUNUSED(event)) {
	serialPortConnection = new SerialPortConnection(this, serialController);
	serialPortConnection->Show();
}


UIUpdateThread::UIUpdateThread(MainWindow * window) : wxThread(wxTHREAD_DETACHED){
	mainWindow = window;
	this->Run();
}

wxThread::ExitCode UIUpdateThread::Entry(){

	int i = 0;
	int dataLen = 0;
	while (true) {
		mainWindow->UpdateData(MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_PIL, i%8);
		mainWindow->UpdateData(MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_TRACKING, i%8);

		// Data has been aquired
		if (mainWindow->GetSerialController()->GetAllData().size() > 0) {

			dataLen += mainWindow->GetSerialController()->GetAllData().size();
			wxVector<char> incomingData = mainWindow->GetSerialController()->GetDataStartingAtIndex();

			if (incomingData.size() < 1) {
				this->Sleep(100);
				continue;
			}
			wxString append = "";
			for (int i = 0; i < incomingData.size(); i++) {
				append += incomingData[i];
			}
			mainWindow->ReciveSerialData(append);
		}

		i += 1;
		this->Sleep(10);
	}
}