#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(NULL, -1, "Astro Cats Ground Control", wxDefaultPosition, wxSize(800,600)){

	// Create and add menu bar with menus
	menuBar = new wxMenuBar;
	menuFile = new wxMenu;
	menuSerial = new wxMenu;
	menuPIL = new wxMenu;
	menuView = new wxMenu;
	menuHelp = new wxMenu;

	// File Menu
	menuFile->Append(MainWindow::MenuBar::ID_SET_LOG_FILE, _("Set Recording File"));
	menuFile->AppendCheckItem(MainWindow::MenuBar::ID_LOG, _("Record Data"));

	// Serial Menu
	menuSerial->Append(MainWindow::MenuBar::ID_CONNECT_SERIAL, _("Connect to a Serial Port"));
	menuSerial->Append(MainWindow::MenuBar::ID_SHOW_PLAYBACK, _("Playback Data"));

	// Pil Menu
	menuPIL->Append(MainWindow::MenuBar::ID_SEND_PIL_COMMAND, _("Send Commands to the PIL"));

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
	menuView->Append(MainWindow::MenuBar::ID_VIEW_GPS, _("Rocket and PIL GPS"));
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
	menuBar->Append(menuPIL, _("&PIL"));
	menuBar->Append(menuView, _("&View"));
	menuBar->Append(menuHelp, _("&Help"));

	// Set the menu bar
	this->SetMenuBar(menuBar);

	// Set the status bar
	this->CreateStatusBar();

	this->SetIcon(wxIcon("IDI_ICON1"));

	serialController = new SerialController();
	serialPortConnection = new SerialPortConnection(this, serialController);
	
	dataWindow = new IncomingDataStream(this, "Radio Data", 20);

	// Set main layout
	mainLayout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainLayout);

	// Add items to main layout
	this->GetSizer()->Add(dataWindow, 1, wxEXPAND, 0);
	this->GetSizer()->Layout();

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSetLogFile, this, MainWindow::MenuBar::ID_SET_LOG_FILE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSerialConnection, this, MainWindow::MenuBar::ID_CONNECT_SERIAL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPlayback, this, MainWindow::MenuBar::ID_SHOW_PLAYBACK);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPilSignalStrength, this, MainWindow::MenuBar::ID_VIEW_PILSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowRocketSignalStrength, this, MainWindow::MenuBar::ID_VIEW_ROCKETSTRENGTH);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowTemperature, this, MainWindow::MenuBar::ID_VIEW_TEMP);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowHumidity, this, MainWindow::MenuBar::ID_VIEW_HUMID);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowUV, this, MainWindow::MenuBar::ID_VIEW_UV);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowSolar, this, MainWindow::MenuBar::ID_VIEW_SOLAR);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPressureAltitude, this, MainWindow::MenuBar::ID_VIEW_PRESSURE_ALTITUDE);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowPitchRoll, this, MainWindow::MenuBar::ID_VIEW_PITCH_ROLL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowGPS, this, MainWindow::MenuBar::ID_VIEW_GPS);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowAll, this, MainWindow::MenuBar::ID_VIEW_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::HideAll, this, MainWindow::MenuBar::ID_HIDE_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::RepositionAll, this, MainWindow::MenuBar::ID_REPO_ALL);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowAbout, this, MainWindow::MenuBar::ID_ABOUT);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainWindow::ShowDoc, this, MainWindow::MenuBar::ID_DOC);
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainWindow::OnClose, this);

	//hexToJpeg = new HexToJpeg();

	logger = new Logger();
	uiUpdater = new UIUpdateThread(this);

	pilRadioStrength = new RadioSignalStrengthWindow(this, "PIL Radio Strength");
	rocketRadioStrength = new RadioSignalStrengthWindow(this, "Rocket Radio Strength");
	temperatureWindow = new LinearWindow(this, "Temperature", -20.0f, 40.0f, wxColor(255,0,0), true, true, " C", " F", 10, 1.8f, 32.0f);
	humidityWindow = new LinearWindow(this, "Humidity", 0.0f, 100.0f, wxColor(0, 0, 255), true, false, "%", "", 10);
	uvWindow = new RadialWindow(this, "UV Intensity", 0.0f, 150.0f, wxColor(150, 0, 255), " W/M2", 10);
	solarWindow = new RadialWindow(this, "Solar Irradiance", 0.0f, 1250.0f, wxColor(255, 255, 0), " W/M2", 10);
	pressureAltitudeWindow = new PressureAltitudeWindow(this, "Air Pressure & Altitude", 600.0f, 1100.0f, 10, 0, 5500, 10);
	pitchRollWindow = new PitchRollWindow(this, "Pitch & Roll");
	gpsView = new GPSView(this, "Rocket and PIL GPS");
	playbackWindow = new PlaybackWindow(this, this);
	docWindow = new DocumentationWindow(this);
	aboutWindow = new AboutWindow(this);

	// Fire a reposition event so the windows are positioned correctly from the start
	wxCommandEvent repositionEvent(wxEVT_COMMAND_MENU_SELECTED, MainWindow::MenuBar::ID_REPO_ALL);
	repositionEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(repositionEvent);

	int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int width = this->GetSize().GetWidth();
	wxPoint topRight(screenWidth - width, 0);
	this->SetPosition(topRight);

	this->IsUpdatesAvailable();





	pilRadioStrength->SetNumBars(7);
	rocketRadioStrength->SetNumBars(5);
}

void MainWindow::OnClose(wxCloseEvent& evt) {

	if (serialController->IsConnected()) {
		serialController->StopSerial();
		serialController->Stop();
	}
	playbackWindow->StopThread();
	serialPortConnection->StopThread();
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
		OutputDebugStringA("LOGGING!");
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

			// Update the UI based on the latest JSON data
			temperatureWindow->SetValue(jsonData["Temp"]);
			humidityWindow->SetValue(jsonData["Humidity"]);
			pressureAltitudeWindow->SetPressure(jsonData["Pressure"]);
			uvWindow->SetValue((float)jsonData["UV"] * 10.0f);
			pitchRollWindow->SetAccelerationData((float)jsonData["AccelerationX"], (float)jsonData["AccelerationY"], (float)jsonData["AccelerationZ"]);
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

void MainWindow::ShowSetLogFile(wxCommandEvent& WXUNUSED(event)) {

	wxFileDialog saveFileDialog(this, _("Save Playback File"), "", "", "Astro Cat Files (*.astrocat)|*.astrocat", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	logger->SetLogFile(saveFileDialog.GetPath());
	OutputDebugStringA(saveFileDialog.GetPath().char_str());
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

void MainWindow::ShowGPS(wxCommandEvent& WXUNUSED(event)) {
	//gpsView->Show();
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
	//gpsView->Show();
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
	gpsView->Hide();
}

void MainWindow::RepositionAll(wxCommandEvent& WXUNUSED(event)) {
	int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	// Size and position the PIL Radio Strength
	pilRadioPos = wxPoint(0, 0);
	pilRadioSize = wxSize(screenWidth / 7.0f, screenHeight / 4.0f);
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

			wxString append = "";
			for (int i = 0; i < (int)incomingData.size(); i++) {
				append += incomingData[i];
			}
			mainWindow->ReciveSerialData(append);
		}

		this->Sleep(10);
	}
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

	for (int i = 0; i < reader->GetNumLines(); i++){

		// Spin wait while paused
		while (pause) {
			this->Sleep(100);
		}
		mainWindow->ReciveSerialData(reader->ReadLine(i));
		
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