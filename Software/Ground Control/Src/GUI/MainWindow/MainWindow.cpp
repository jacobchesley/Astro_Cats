#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(NULL, -1, "Astro Cats Ground Control", wxDefaultPosition, wxSize(800,600)){

	// Create and add menu bar with menus
	wxMenuBar * menuBar = new wxMenuBar;
	wxMenu * menuFile = new wxMenu;
	wxMenu * menuSerial = new wxMenu;
	wxMenu * menuPIL = new wxMenu;
	wxMenu * menuView = new wxMenu;
	wxMenu * menuHelp = new wxMenu;

	// Serial Menu
	menuSerial->Append(MainWindow::MenuBar::ID_CONNECT_SERIAL, _("Connect to a Serial Port"));

	// Pil Menu
	menuPIL->Append(MainWindow::MenuBar::ID_SEND_PIL_COMMAND, _("Send Commands to the PIL"));
	menuPIL->AppendSeparator();
	menuPIL->Append(MainWindow::MenuBar::ID_READ_PIL_STATUS, _("Get PIL Status"));

	// View Menu
	menuView->Append(MainWindow::MenuBar::ID_VIEW_PILSTRENGTH, _("PIL Signal Strength"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_ROCKETSTRENGTH, _("Rocket Signal Strength"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_TEMP, _("Temperature"));

	// Help Menu
	menuHelp->Append(MainWindow::MenuBar::ID_DOC, _("Documentation"));
	menuHelp->Append(MainWindow::MenuBar::ID_ABOUT, _("About"));

	// Append menus to menu bar
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSerial, _("&Serial Connections"));
	menuBar->Append(menuPIL, _("&PIL"));
	menuBar->Append(menuView, _("&View"));
	menuBar->Append(menuHelp, _("&Help"));

	// Set the menu bar
	this->SetMenuBar(menuBar);

	// Set the status bar
	this->CreateStatusBar();

	this->SetIcon(wxIcon("IDI_ICON1"));

	serialController = new SerialController();
	
	dataWindow = new IncomingDataStream(this, "Radio Data");

	// Set main layout
	mainLayout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainLayout);

	// Add items to main layout
	this->GetSizer()->Add(dataWindow, 1, wxEXPAND, 0);
	this->GetSizer()->Layout();

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSerialConnection, this, MainWindow::MenuBar::ID_CONNECT_SERIAL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPilSignalStrength, this, MainWindow::MenuBar::ID_VIEW_PILSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowRocketSignalStrength, this, MainWindow::MenuBar::ID_VIEW_ROCKETSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowTemperature, this, MainWindow::MenuBar::ID_VIEW_TEMP);

	//hexToJpeg = new HexToJpeg();

	uiUpdater = new UIUpdateThread(this);

	pilRadioStrength = new RadioSignalStrengthWindow(this, "PIL Radio Strength");
	rocketRadioStrength = new RadioSignalStrengthWindow(this, "Rocket Radio Strength");
	temperatureWindow = new LinearWindow(this, "Temperature", -10.0f, 40.0f, wxColor(255,0,0), true, true, " C", " F", 10, 1.8f, 32.0f);
}

void MainWindow::UpdateData(int dataParameter, int dataValue){

}

void MainWindow::ReciveSerialData(wxString serialData){

	dataWindow->AppendText(serialData);
	tempJsonData += serialData;

	wxVector<wxString> allJsonData;
	wxString fullJsonData;

	// Extract all COMPLETE JSON strings from the temporary json data.
	while(tempJsonData.Contains("}")){

		fullJsonData = tempJsonData.BeforeFirst('}');
		fullJsonData += "}";
		allJsonData.push_back(fullJsonData);
		tempJsonData = tempJsonData.AfterFirst('}');
	}

	// Iterate through all complete JSON strings
	for (int i = 0; i < allJsonData.size(); i++) {

		// Parse the current JSON string
		fullJsonData = allJsonData[i];
		std::string jsonString = std::string(fullJsonData);
		auto jsonData = nlohmann::json::parse(jsonString);

		// Update the UI based on the latest JSON data
		temperatureWindow->SetValue(jsonData["Temp"]);
	}

	if (serialData == "END") {
		//hexToJpeg->WriteJpegFile("C:\\Development\\UC Rocketry\\Nasa SLI\\Software\\Ground Control\\Build\\Release\\Test.jpeg");
		OutputDebugStringA("Wrote to file");
		return;
	}

	//char jpegData[16384];
	//for (int i = 0; i < serialData.size(); i++) {
	//	jpegData[i] = serialData[i];
	//}
}

SerialController * MainWindow::GetSerialController() {
	return serialController;
}

void MainWindow::ShowSerialConnection(wxCommandEvent& WXUNUSED(event)) {
	serialPortConnection = new SerialPortConnection(this, serialController);
	serialPortConnection->Show();
}

void MainWindow::ShowRocketSignalStrength(wxCommandEvent& WXUNUSED(event)) {
	rocketRadioStrength->Show();
}
void MainWindow::ShowPilSignalStrength(wxCommandEvent& WXUNUSED(event)) {
	pilRadioStrength->Show();
}

void MainWindow::ShowTemperature(wxCommandEvent& WXUNUSED(event)) {
	temperatureWindow->Show();
}

UIUpdateThread::UIUpdateThread(MainWindow * window) : wxThread(wxTHREAD_DETACHED){
	mainWindow = window;
	this->Run();
}

wxThread::ExitCode UIUpdateThread::Entry(){

	while (true) {

		// Data has been aquired
		if (mainWindow->GetSerialController()->GetAllData().size() > 0) {

			wxVector<char> incomingData = mainWindow->GetSerialController()->GetDataStartingAtIndex();
			mainWindow->GetSerialController()->ClearReadData();

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

		this->Sleep(10);
	}
}