#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(NULL, -1, "Astro Cats Ground Control", wxDefaultPosition, wxSize(800,600)){

	isClosed = false;

	// Create and add menu bar with menus
	menuBar = new wxMenuBar;
	menuFile = new wxMenu;
	menuSerial = new wxMenu;
	menuCommand = new wxMenu;
	menuView = new wxMenu;
	menuHelp = new wxMenu;

	// File Menu
	menuFile->Append(MainWindow::MenuBar::ID_SET_LOG_FILE, _("Set Recording File"));
	menuFile->AppendCheckItem(MainWindow::MenuBar::ID_LOG, _("Record Data"));
	menuFile->AppendSeparator();
	menuFile->Append(MainWindow::MenuBar::ID_SHOW_SETTINGS, _("Settings"));
	menuFile->AppendSeparator();
	menuFile->Append(MainWindow::MenuBar::ID_CLOSE, _("Exit"));

	// Serial Menu
	menuSerial->Append(MainWindow::MenuBar::ID_CONNECT_SERIAL, _("Connect to a Serial Port"));
	menuSerial->Append(MainWindow::MenuBar::ID_SHOW_PLAYBACK, _("Playback Data"));

	// Command and Error Menu
	menuCommand->Append(MainWindow::MenuBar::ID_VIEW_COMMANDS, _("Send Commands to Rocket and PIL"));
	menuCommand->Append(MainWindow::MenuBar::ID_VIEW_COMMANDS_RESPONSE, _("Command Response from Rocket and PIL"));
	menuCommand->AppendSeparator();
	menuCommand->Append(MainWindow::MenuBar::ID_VIEW_ERRORS, _("Errors from Rocket and PIL"));

	// View Menu
	menuView->Append(MainWindow::MenuBar::ID_VIEW_PILSTRENGTH, _("PIL Signal Strength"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_ROCKETSTRENGTH, _("Rocket Signal Strength"));
	menuView->AppendSeparator();
	menuView->Append(MainWindow::MenuBar::ID_VIEW_TEMP, _("Temperature"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_HUMID, _("Humidity"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_UV, _("UV Intensity"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_SOLAR, _("Solar Irradiance"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_PRESSURE_ALTITUDE, _("Air Pressure & Altitude"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_PITCH_ROLL, _("Pitch & Roll"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_GPS_ROCKET, _("Rocket GPS"));
	menuView->Append(MainWindow::MenuBar::ID_VIEW_GPS_PIL, _("PIL GPS"));
	menuView->AppendSeparator();
	menuView->Append(MainWindow::MenuBar::ID_VIEW_ALL, _("Show All"));
	menuView->Append(MainWindow::MenuBar::ID_HIDE_ALL, _("Hide All"));
	menuView->Append(MainWindow::MenuBar::ID_REPO_ALL, _("Reposition All"));

	// Help Menu
	menuHelp->Append(MainWindow::MenuBar::ID_DOC, _("Documentation"));
	menuHelp->Append(MainWindow::MenuBar::ID_ABOUT, _("About"));

	// Append menus to menu bar
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSerial, _("&Serial Connections"));
	menuBar->Append(menuCommand, _("&Commands"));
	menuBar->Append(menuView, _("&View"));
	menuBar->Append(menuHelp, _("&Help"));

	// Set the menu bar
	this->SetMenuBar(menuBar);

	// Set the status bar
	this->CreateStatusBar();

	this->SetIcon(wxIcon("IDI_ICON1"));

	serialController = new SerialController();
	serialPortConnection = new SerialPortConnection(this, serialController);
	
	dataWindow = new IncomingDataStream(this, "Radio Data", 100);

	// Set main layout
	mainLayout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainLayout);

	// Add items to main layout
	this->GetSizer()->Add(dataWindow, 1, wxEXPAND, 0);
	this->GetSizer()->Layout();

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSetLogFile, this, MainWindow::MenuBar::ID_SET_LOG_FILE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSettings, this, MainWindow::MenuBar::ID_SHOW_SETTINGS);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::OnClose, this, MainWindow::MenuBar::ID_CLOSE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSerialConnection, this, MainWindow::MenuBar::ID_CONNECT_SERIAL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPlayback, this, MainWindow::MenuBar::ID_SHOW_PLAYBACK);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowCommands, this, MainWindow::MenuBar::ID_VIEW_COMMANDS);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowCommandsResponse, this, MainWindow::MenuBar::ID_VIEW_COMMANDS_RESPONSE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowErrors, this, MainWindow::MenuBar::ID_VIEW_ERRORS);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPilSignalStrength, this, MainWindow::MenuBar::ID_VIEW_PILSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowRocketSignalStrength, this, MainWindow::MenuBar::ID_VIEW_ROCKETSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowTemperature, this, MainWindow::MenuBar::ID_VIEW_TEMP);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowHumidity, this, MainWindow::MenuBar::ID_VIEW_HUMID);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowUV, this, MainWindow::MenuBar::ID_VIEW_UV);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSolar, this, MainWindow::MenuBar::ID_VIEW_SOLAR);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPressureAltitude, this, MainWindow::MenuBar::ID_VIEW_PRESSURE_ALTITUDE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPitchRoll, this, MainWindow::MenuBar::ID_VIEW_PITCH_ROLL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowGPSRocket, this, MainWindow::MenuBar::ID_VIEW_GPS_ROCKET);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowGPSPIL, this, MainWindow::MenuBar::ID_VIEW_GPS_PIL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowAll, this, MainWindow::MenuBar::ID_VIEW_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::HideAll, this, MainWindow::MenuBar::ID_HIDE_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::RepositionAll, this, MainWindow::MenuBar::ID_REPO_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowAbout, this, MainWindow::MenuBar::ID_ABOUT);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowDoc, this, MainWindow::MenuBar::ID_DOC);
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainWindow::OnClose, this);

	//hexToJpeg = new HexToJpeg();

	logger = new Logger();
	uiUpdater = new UIUpdateThread(this);

	mainWindowSettings = new MainWindowSettings(this, dataWindow);
	mainCommandWindow = new MainCommandWindow(this, "Send Commands to Rocket and PIL", serialController);
	commandResponseWindow = new CommandResponseWindow(this, "Command Reponse from Rocket and PIL");
	errorWindow = new ErrorWindow(this, "Errors from Rocket and PIL");

	pilRadioStrength = new RadioSignalStrengthWindow(this, "PIL Radio Strength");
	rocketRadioStrength = new RadioSignalStrengthWindow(this, "Rocket Radio Strength");
	temperatureWindow = new LinearWindow(this, "Temperature", -20.0f, 40.0f, wxColor(255,0,0), true, true, " C", " F", 10, 1.8f, 32.0f);
	humidityWindow = new LinearWindow(this, "Humidity", 0.0f, 100.0f, wxColor(0, 0, 255), true, false, "%", "", 10);
	uvWindow = new RadialWindow(this, "UV Intensity", 0.0f, 150.0f, wxColor(150, 0, 255), " W/M2", 10);
	solarWindow = new RadialWindow(this, "Solar Irradiance", 0.0f, 1250.0f, wxColor(255, 255, 0), " W/M2", 10);
	pressureAltitudeWindow = new PressureAltitudeWindow(this, "Air Pressure & Altitude", 600.0f, 1100.0f, 10, 0, 5500, 10);
	pitchRollWindow = new PitchRollWindow(this, "Pitch & Roll");
	gpsViewRocket = new GPSViewWindow(this, "Rocket GPS", wxIcon("IDI_ICON2"), wxIcon("IDI_ICON3"));
	gpsViewPIL= new GPSViewWindow(this, "PIL GPS", wxIcon("IDI_ICON2"), wxIcon("IDI_ICON4"));
	playbackWindow = new PlaybackWindow(this, this);
	docWindow = new DocumentationWindow(this);
	aboutWindow = new AboutWindow(this);

	// Fire a reposition event so the windows are positioned correctly from the start
	wxCommandEvent repositionEvent(wxEVT_COMMAND_MENU_SELECTED, MainWindow::MenuBar::ID_REPO_ALL);
	repositionEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(repositionEvent);

	this->IsUpdatesAvailable();

}

void MainWindow::OnClose(wxCloseEvent& evt) {
	isClosed = true;

	if (serialController->IsConnected()) {
		serialController->StopSerial();
		serialController->Stop();
	}
	uiUpdater->StopThread();
	playbackWindow->StopThread();
	serialPortConnection->StopThread();
	Sleep(200);
	evt.Skip();
	this->Destroy();
}

void MainWindow::IsUpdatesAvailable() {

	// Get version of this software
	wxString thisVersion = aboutWindow->GetVersion();

	// Get latest version from website
	wxURL url(wxT("http://www.ucrocketry.com/software/GroundControlLatestVersion.txt"));
	if (url.GetError() == wxURL_NOERR)
	{
		wxString latestVersion;
		wxInputStream *in = url.GetInputStream();

		if (in && in->IsOk())
		{
			wxStringOutputStream latestVersionStream(&latestVersion);
			in->Read(latestVersionStream);

			// Compare this version to latest version
			// If versions are not equal, display update dialog
			if (latestVersion.CmpNoCase(thisVersion) != 0) {

				wxString warningString = "This Ground Control Software is out of date! \n";
				warningString += "Please download latest version from www.ucrocketry.com/software \n";
				warningString += "Would you like me to open this page for you?";

				wxMessageDialog * warning = new wxMessageDialog(this, warningString, "Update Ground Control?", wxYES_NO | wxICON_WARNING);
				if (warning->ShowModal() == wxID_YES) {
					wxLaunchDefaultBrowser("www.ucrocketry.com/software");
				}
			}

		}
		delete in;
	}
}

void MainWindow::ReciveSerialData(wxString serialData){

	dataWindow->AppendText(serialData);
	tempJsonData += serialData;

	if (!playbackWindow->IsPlaying() && menuFile->IsChecked(MainWindow::MenuBar::ID_LOG)) {
		logger->Log(serialData);
	}

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
	for (int i = 0; i < (int)allJsonData.size(); i++) {

		// Parse the current JSON string
		try {
			fullJsonData = allJsonData[i];
			std::string jsonString = std::string(fullJsonData);
			auto jsonData = nlohmann::json::parse(jsonString);

			wxString source = jsonData["Source"];
			wxString messageType = jsonData["MessageType"];

			// If we are recieving GPS Data from the rocket...
			if(source == "Rocket" && messageType == "Data") {
				// Update coordinates of Rocket GPS
				GPSCoord rocketCoord; 
				rocketCoord.Lat = (float)jsonData["Lat"];
				rocketCoord.Lon = (float)jsonData["Lon"];
				rocketCoord.NS = (wxString)jsonData["NS"];
				rocketCoord.EW = (wxString)jsonData["EW"];
				gpsViewRocket->UpdateGPSPos(rocketCoord);
				
				gpsViewRocket->UpdateAltitude((float)jsonData["Altitude"]);
				gpsViewRocket->UpdateQuality((int)jsonData["Quality"]);
				gpsViewRocket->UpdateNumSat((int)jsonData["NumSat"]);
				gpsViewRocket->UpdatePDOP((float)jsonData["PDOP"]);
				gpsViewRocket->UpdateHDOP((float)jsonData["HDOP"]);
				gpsViewRocket->UpdateVDOP((float)jsonData["VDOP"]);
				gpsViewRocket->UpdateSatList((wxString)jsonData["SatList"]);
				gpsViewRocket->UpdateTime((wxString)jsonData["Time"]);
			}

			// If we are recieving data from PIL...
			else if (source == "PIL" && messageType == "Data") {
				temperatureWindow->SetValue(jsonData["Temp"]);
				humidityWindow->SetValue(jsonData["Humidity"]);
				pressureAltitudeWindow->SetPressure(jsonData["Pressure"]);
				uvWindow->SetValue((float)jsonData["UV"]);
				solarWindow->SetValue((float)jsonData["Solar"]);
				pitchRollWindow->SetAccelerationData((float)jsonData["AccelerationX"], (float)jsonData["AccelerationZ"], (float)jsonData["AccelerationY"]);

				// Update coordinates of PIL GPS
				/*
				GPSCoord pilCoord;
				pilCoord.Lat = (float)jsonData["Lat"];
				pilCoord.Lon = (float)jsonData["Lon"];
				pilCoord.NS = (wxString)jsonData["NS"];
				pilCoord.EW = (wxString)jsonData["EW"];
				gpsViewPIL->UpdateGPSPos(pilCoord);

				gpsViewPIL->UpdateAltitude((float)jsonData["Altitude"]);
				gpsViewPIL->UpdateQuality((int)jsonData["Quality"]);
				gpsViewPIL->UpdateNumSat((int)jsonData["NumSat"]);
				gpsViewPIL->UpdatePDOP((float)jsonData["PDOP"]);
				gpsViewPIL->UpdateHDOP((float)jsonData["HDOP"]);
				gpsViewPIL->UpdateVDOP((float)jsonData["VDOP"]);
				gpsViewPIL->UpdateSatList((wxString)jsonData["SatList"]);
				gpsViewPIL->UpdateTime((wxString)jsonData["Time"]);
				*/
			}	

			// If we are recieving a command response from rocket...
			else if (source == "Rocket" && messageType == "CommandResponse") {
				commandResponseWindow->RecieveResponseRocket((wxString)jsonData["Command"], (wxString)jsonData["Value"]);
			}

			// If we are recieving a command response from PIL...
			else if (source == "PIL" && messageType == "CommandResponse") {
				commandResponseWindow->RecieveResponsePIL((wxString)jsonData["Command"], (wxString)jsonData["Value"]);
			}

			// If we are recieving a file chunk from PIL...
			else if (source == "PIL" && messageType == "File") {
				wxString test = (wxString)jsonData["Chunk"];
			}

			// If we are recieving an error from rocket...
			else if (source == "Rocket" && messageType == "Error") {
				errorWindow->RecieveErrorRocket((wxString)jsonData["Error"], (wxString)jsonData["Description"]);
			}

			// If we are recieving an error from PIL...
			else if (source == "PIL" && messageType == "Error") {
				errorWindow->RecieveErrorPIL((wxString)jsonData["Error"], (wxString)jsonData["Description"]);
			}

			// If we are receving Signal Strength Info from the ground..
			else if (source == "Ground" && messageType == "SignalStrength") {

				wxString signalSource = (wxString)jsonData["SignalSource"];
				int signalStrength = (int)jsonData["SignalStrength"];

				if (signalSource == "Tracking") {
					rocketRadioStrength->SetSignalStrength(signalStrength);
				}
				else if(signalSource == "PIL"){
					pilRadioStrength->SetSignalStrength(signalStrength);
				}
			}
			// Update the UI based on the latest JSON data
			//temperatureWindow->SetValue(jsonData["Temp"]);
			//humidityWindow->SetValue(jsonData["Humidity"]);
			//pressureAltitudeWindow->SetPressure(jsonData["Pressure"]);
			//uvWindow->SetValue((float)jsonData["UV"] * 10.0f);
			//pitchRollWindow->SetAccelerationData((float)jsonData["AccelerationX"], (float)jsonData["AccelerationY"], (float)jsonData["AccelerationZ"]);
		}
		catch (...) {

		}
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

void MainWindow::UpdateStatusBar(wxString statusMessage) {
	if (!isClosed) {
		this->SetStatusText(statusMessage);
	}
}

void MainWindow::ShowSetLogFile(wxCommandEvent& WXUNUSED(event)) {

	wxFileDialog saveFileDialog(this, _("Save Playback File"), "", "", "Astro Cat Files (*.astrocat)|*.astrocat", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	logger->SetLogFile(saveFileDialog.GetPath());
	OutputDebugStringA(saveFileDialog.GetPath().char_str());
}

void MainWindow::ShowSettings(wxCommandEvent& WXUNUSED(event)) {
	mainWindowSettings->Show();
}

void MainWindow::ShowSerialConnection(wxCommandEvent& WXUNUSED(event)) {
	serialPortConnection->Show();
}

void MainWindow::ShowPlayback(wxCommandEvent& WXUNUSED(event)) {

	int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	int width = playbackWindow->GetSize().GetWidth();
	int height = playbackWindow->GetSize().GetHeight();
	wxPoint bottomRight(screenWidth - width, screenHeight - height);
	playbackWindow->SetPosition(bottomRight);

	playbackWindow->Show();
}

void MainWindow::ShowCommands(wxCommandEvent& WXUNUSED(event)) {
	mainCommandWindow->Show();
}

void MainWindow::ShowCommandsResponse(wxCommandEvent& WXUNUSED(event)) {
	commandResponseWindow->Show();
}

void MainWindow::ShowErrors(wxCommandEvent& WXUNUSED(event)) {
	errorWindow->Show();
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

void MainWindow::ShowHumidity(wxCommandEvent& WXUNUSED(event)) {
	humidityWindow->Show();
}

void MainWindow::ShowUV(wxCommandEvent& WXUNUSED(event)) {
	uvWindow->Show();
}

void MainWindow::ShowSolar(wxCommandEvent& WXUNUSED(event)) {
	solarWindow->Show();
}

void MainWindow::ShowPressureAltitude(wxCommandEvent& WXUNUSED(event)) {
	pressureAltitudeWindow->Show();
}

void MainWindow::ShowPitchRoll(wxCommandEvent& WXUNUSED(event)) {
	pitchRollWindow->Show();
}

void MainWindow::ShowGPSPIL(wxCommandEvent& WXUNUSED(event)) {
	gpsViewPIL->Show();
}

void MainWindow::ShowGPSRocket(wxCommandEvent& WXUNUSED(event)) {
	gpsViewRocket->Show();
}

void MainWindow::ShowAbout(wxCommandEvent& WXUNUSED(event)) {
	aboutWindow->Show();
}

void MainWindow::ShowDoc(wxCommandEvent& WXUNUSED(event)) {
	docWindow->DisplayContents();
}

void MainWindow::ShowAll(wxCommandEvent& WXUNUSED(event)) {
	rocketRadioStrength->Show();
	pilRadioStrength->Show();
	temperatureWindow->Show();
	humidityWindow->Show();
	uvWindow->Show();
	solarWindow->Show();
	pressureAltitudeWindow->Show();
	pitchRollWindow->Show();
	gpsViewRocket->Show();
	gpsViewPIL->Show();
}

void MainWindow::HideAll(wxCommandEvent& WXUNUSED(event)) {
	rocketRadioStrength->Hide();
	pilRadioStrength->Hide();
	temperatureWindow->Hide();
	humidityWindow->Hide();
	uvWindow->Hide();
	solarWindow->Hide();
	pressureAltitudeWindow->Hide();
	pitchRollWindow->Hide();
	gpsViewRocket->Hide();
	gpsViewPIL->Hide();
}

void MainWindow::RepositionAll(wxCommandEvent& WXUNUSED(event)) {
	int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	// Size and position the PIL Radio Strength
	pilRadioPos = wxPoint(0, 0);
	pilRadioSize = wxSize(screenWidth / 6.2f, screenHeight / 4.0f);
	pilRadioStrength->SetPosition(pilRadioPos);
	pilRadioStrength->SetSize(pilRadioSize);

	// Size and position the Rocket Radio Strength (to the right of PIL Radio Strength)
	rocketRadioPos = wxPoint(pilRadioStrength->GetSize().GetWidth(), 0);
	rocketRadioSize = pilRadioSize;
	rocketRadioStrength->SetPosition(rocketRadioPos);
	rocketRadioStrength->SetSize(rocketRadioSize);

	// Size and position Temperature Guage (below PIL Radio Strength)
	tempPos = wxPoint(0, pilRadioStrength->GetSize().GetHeight());
	tempSize = wxSize(pilRadioStrength->GetSize().GetWidth(), pilRadioStrength->GetSize().GetHeight() * 2);
	temperatureWindow->SetPosition(tempPos);
	temperatureWindow->SetSize(tempSize);

	// Size and position Temperature Guage (below PIL Radio Strength)
	humidPos = wxPoint(temperatureWindow->GetSize().GetWidth(), rocketRadioStrength->GetSize().GetHeight());
	humidSize = tempSize;
	humidityWindow->SetPosition(humidPos);
	humidityWindow->SetSize(humidSize);

	// Size and position UV Guage (below Temp Guage)
	uvPos = wxPoint(0, rocketRadioStrength->GetSize().GetHeight() + temperatureWindow->GetSize().GetHeight());
	uvSize = rocketRadioSize;
	uvWindow->SetPosition(uvPos);
	uvWindow->SetSize(uvSize);

	// Size and position Solar Guage (below Humidity Guage)
	solarPos = wxPoint(uvWindow->GetSize().GetWidth(), pilRadioStrength->GetSize().GetHeight() + humidityWindow->GetSize().GetHeight());
	solarSize = rocketRadioSize;
	solarWindow->SetPosition(solarPos);
	solarWindow->SetSize(solarSize);

	// Size and position Pressure Altitude (next to Rocket Signal Strength and Temp and Humidity)
	pressurePos = wxPoint(rocketRadioStrength->GetSize().GetWidth() + pilRadioStrength->GetSize().GetWidth(),0);
	pressureSize = wxSize(rocketRadioSize.GetWidth(), rocketRadioStrength->GetSize().GetHeight() + humidityWindow->GetSize().GetHeight());
	pressureAltitudeWindow->SetPosition(pressurePos);
	pressureAltitudeWindow->SetSize(pressureSize);

	// Size and position Pitch Roll (next to Solar Irradiance)
	pitchPos = wxPoint(uvWindow->GetSize().GetWidth() + solarWindow->GetSize().GetWidth(), pressureAltitudeWindow->GetSize().GetHeight());
	pitchSize = solarSize;
	pitchRollWindow->SetPosition(pitchPos);
	pitchRollWindow->SetSize(pitchSize);

	// Size and position Rocket GPS
	rocketGPSPos = wxPoint(uvWindow->GetSize().GetWidth() + solarWindow->GetSize().GetWidth() + pressureAltitudeWindow->GetSize().GetWidth(), 0);
	rocketGPSSize = wxSize(tempSize.GetWidth() * 3, tempSize.GetHeight()*0.75);
	gpsViewRocket->SetPosition(rocketGPSPos);
	gpsViewRocket->SetSize(rocketGPSSize);

	// Size and position PIL GPS
	pilGPSPos = wxPoint(gpsViewRocket->GetPosition().x, gpsViewRocket->GetPosition().y + gpsViewRocket->GetSize().GetHeight());
	pilGPSSize = rocketGPSSize;
	gpsViewPIL->SetPosition(pilGPSPos);
	gpsViewPIL->SetSize(pilGPSSize);

	// Size and position of main window
	mainWindowPoint = wxPoint(gpsViewRocket->GetPosition().x, gpsViewPIL->GetPosition().y + gpsViewPIL->GetSize().GetHeight());
	mainWindowSize = wxSize(gpsViewPIL->GetSize().GetWidth(), pitchSize.GetHeight());
	this->SetPosition(mainWindowPoint);
	this->SetSize(mainWindowSize);
}

MainWindowSettings::MainWindowSettings(wxWindow * parent, IncomingDataStream * inDataStream) : wxFrame(parent, -1, "Settings"){

	dataStream = inDataStream;

	this->SetBackgroundColour(wxColor(45, 45, 45));

	numLinesLayout = new wxBoxSizer(wxHORIZONTAL);
	numLinesLabel = new wxStaticText(this, -1, "Number of Data Lines to Display ");
	numLinesLabel->SetBackgroundColour(this->GetBackgroundColour());
	numLinesLabel->SetForegroundColour(wxColor(200, 200, 200));
	numLinesLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	numLinesText = new wxTextCtrl(this, -1, "100");
	numLinesText->SetBackgroundColour(this->GetBackgroundColour());
	numLinesText->SetForegroundColour(wxColor(255, 255, 255));
	numLinesText->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	numLinesLayout->Add(numLinesLabel);
	numLinesLayout->Add(numLinesText);

	applyButton = new wxButton(this, ID_APPLY, "Apply Settings");
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(numLinesLayout);
	this->GetSizer()->AddStretchSpacer();
	this->GetSizer()->Add(applyButton);

	this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&MainWindowSettings::ApplySettings, this, MainWindowSettings::ID_APPLY);
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainWindowSettings::OnClose, this);

	this->SetIcon(wxIcon("IDI_ICON1"));

}

void MainWindowSettings::OnClose(wxCloseEvent& closeEvent) {
	this->Hide();
	closeEvent.Veto();
}


void MainWindowSettings::ApplySettings(wxCommandEvent& WXUNUSED(event)) {
	wxString numLinesStr = numLinesText->GetValue();
	int numLinesInt = wxAtoi(numLinesStr);
	dataStream->SetMaxLines(numLinesInt);
}

UIUpdateThread::UIUpdateThread(MainWindow * window) : wxThread(wxTHREAD_DETACHED){
	mainWindow = window;
	continueWatching = true;
	this->Run();
}

wxThread::ExitCode UIUpdateThread::Entry(){

	while (continueWatching) {

		// Data has been aquired
		if (mainWindow->GetSerialController()->GetAllData().size() > 0) {

			wxVector<char> incomingData = mainWindow->GetSerialController()->GetDataStartingAtIndex();
			mainWindow->GetSerialController()->ClearReadData();

			wxString append = "";
			for (int i = 0; i < (int)incomingData.size(); i++) {
				append += incomingData[i];
			}
			mainWindow->ReciveSerialData(append);
		}

		// Serial controller is connected to a port, update main windows status bar to show serial port it's connected to
		if (mainWindow->GetSerialController()->IsConnected()) {
			mainWindow->UpdateStatusBar("Connected to: " + mainWindow->GetSerialController()->GetSerialPortName());
		}
		// Serial controller is not connected to a port, update main windows status bar to show serial port not connected
		else {
			mainWindow->UpdateStatusBar("Not connected");
		}

		this->Sleep(10);
	}
	return 0;
}

void UIUpdateThread::StopThread() {
	continueWatching = false;
}
PlaybackThread::PlaybackThread(MainWindow * window) : wxThread(wxTHREAD_DETACHED) {
	mainWindow = window;
	pause = false;
	stop = false;
}

void PlaybackThread::SetPlaybackFile(wxString playbackFile) {
	playback = playbackFile;
}

void PlaybackThread::Start() {
	this->Run();
}

void PlaybackThread::Stop() {
	stop = true;
}

void PlaybackThread::PauseResume() {
	pause = !pause;
}

void PlaybackThread::SetHZ(int hz) {
	double seconds = 1.0 / hz;
	sleepTime = seconds * 1000.0;
}
wxThread::ExitCode PlaybackThread::Entry() {

	reader = new Logger(playback);

	for (int i = 0; i < reader->GetNumLines(); i += 2){

		// Spin wait while paused
		while (pause) {
			this->Sleep(100);
		}
		mainWindow->ReciveSerialData(reader->ReadLine(i));
		mainWindow->ReciveSerialData(reader->ReadLine(i+1));
		// Stop, return from loop
		if (stop) {
			return 0;
		}
		this->Sleep(sleepTime);
	}
	return 0;
}

PlaybackWindow::PlaybackWindow(wxWindow * parent, MainWindow * mainParent) : wxFrame(parent, -1, "Playback Window") {

	par = mainParent;
	playThread = new PlaybackThread(par);

	this->SetBackgroundColour(wxColor(45, 45, 45));
	
	hzLabel = new wxStaticText(this, -1, "Update Frequency");
	hzLabel->SetForegroundColour(wxColor(255, 255, 255));

	fileLabel = new wxStaticText(this, -1, "Playback File");
	fileLabel->SetForegroundColour(wxColor(255, 255, 255));
	fileName = new wxTextCtrl(this, - 1);

	fileButton = new wxButton(this, ID_SHOW_FILE, "Show Files");

	fileLayout = new wxBoxSizer(wxHORIZONTAL);
	fileLayout->Add(fileLabel);
	fileLayout->AddSpacer(10);
	fileLayout->Add(fileName);
	fileLayout->AddSpacer(5);
	fileLayout->Add(fileButton);

	hzText = new wxComboBox(this, -1);
	hzText->AppendString("1 HZ");
	hzText->AppendString("2 HZ");
	hzText->AppendString("3 HZ");
	hzText->AppendString("5 HZ");
	hzText->AppendString("10 HZ");
	hzText->SetValue("3 HZ");

	hzLayout = new wxBoxSizer(wxHORIZONTAL);
	hzLayout->Add(hzLabel);
	hzLayout->AddSpacer(10);
	hzLayout->Add(hzText);

	playButton = new wxButton(this, ID_PLAY, "Play");
	playButton->SetBackgroundColour(wxColor(0, 255, 0));
	pauseButton = new wxButton(this, ID_PAUSE, "Pause / Resume");
	pauseButton->SetBackgroundColour(wxColor(255, 255, 0));
	stopButton = new wxButton(this, ID_STOP, "Stop");
	stopButton->SetBackgroundColour(wxColor(255, 0, 0));

	buttonLayout = new wxBoxSizer(wxHORIZONTAL);
	buttonLayout->Add(playButton);
	buttonLayout->AddSpacer(10);
	buttonLayout->Add(pauseButton);
	buttonLayout->AddSpacer(10);
	buttonLayout->Add(stopButton);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(fileLayout, 1, wxEXPAND);
	this->GetSizer()->Add(hzLayout, 1, wxEXPAND);
	this->GetSizer()->Add(buttonLayout, 1, wxEXPAND);

	this->SetIcon(wxIcon("IDI_ICON1"));

	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PlaybackWindow::ShowFiles, this, ID_SHOW_FILE);
	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PlaybackWindow::OnPlay, this, ID_PLAY);
	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PlaybackWindow::OnPause, this, ID_PAUSE);
	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PlaybackWindow::OnStop, this, ID_STOP);
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&PlaybackWindow::OnClose, this);

	isPlaying = false;
}

void PlaybackWindow::ShowFiles(wxCommandEvent& WXUNUSED(event)) {

	wxFileDialog openFileDialog(this, _("Open Playback File"), "", "","Astro Cat Files (*.astrocat)|*.astrocat", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	fileName->SetValue(openFileDialog.GetPath());
}

void PlaybackWindow::OnPlay(wxCommandEvent& WXUNUSED(event)) {
	playThread->Stop();
	
	playThread = new PlaybackThread(par);
	playThread->SetPlaybackFile(fileName->GetValue());

	// Get HZ string
	wxString hzT = hzText->GetValue();
	wxString hzV = hzT.substr(0, 2);
	// Convert to Long
	long hzL = 1;
	hzV.ToLong(&hzL);

	playThread->SetHZ((int)hzL);

	playThread->Start();
	isPlaying = true;
}

void PlaybackWindow::OnPause(wxCommandEvent& WXUNUSED(event)) {

	// Get HZ string
	wxString hzT = hzText->GetValue();
	wxString hzV = hzT.substr(0, 2);
	// Convert to Long
	long hzL = 1;
	hzV.ToLong(&hzL);


	playThread->SetHZ((int)hzL);

	playThread->PauseResume();

	isPlaying = !isPlaying;
}

void PlaybackWindow::StopThread() {
	
	playThread->Stop();
}

void PlaybackWindow::OnStop(wxCommandEvent& WXUNUSED(event)) {
	playThread->Stop();
	isPlaying = false;
}

bool PlaybackWindow::IsPlaying() {
	return isPlaying;
}

void PlaybackWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}