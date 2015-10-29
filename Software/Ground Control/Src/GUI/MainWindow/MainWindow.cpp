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

	serialController = new SerialController("\\\\.\\COM3", "baud=9600 parity=N data=8 stop=1");
	
	// Set up radio signal strength panels in their own splitter window
	radioSplitter = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
	radioSplitter->SetSashGravity(0.5);
	radioSplitter->SetMinimumPaneSize(20);

	radioSignalStrengthPil = new RadioSignalStrength(radioSplitter, "Signal From PIL");
	radioSignalStrengthRocket = new RadioSignalStrength(radioSplitter, "Signal From Rocket");
	radioSplitter->SplitVertically(radioSignalStrengthPil, radioSignalStrengthRocket);

	// Set main layout
	mainLayout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainLayout);

	// Add items to main layout
	this->GetSizer()->Add(radioSplitter, 1, wxEXPAND, 0);
	this->GetSizer()->Layout();

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








UIUpdateThread::UIUpdateThread(MainWindow * window) : wxThread(wxTHREAD_DETACHED){
	mainWindow = window;
	this->Run();
}

wxThread::ExitCode UIUpdateThread::Entry(){

	int i = 0;
	while (true) {
		mainWindow->UpdateData(MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_PIL, i%8);
		mainWindow->UpdateData(MainWindow::DataParam::DATA_RADIO_SIGNAL_STRENGTH_FROM_TRACKING, i%8);
		i += 1;
		this->Sleep(800);
	}
}