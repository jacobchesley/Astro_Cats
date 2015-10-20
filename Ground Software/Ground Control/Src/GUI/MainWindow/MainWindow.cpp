#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(NULL, -1, "Astro Cats Ground Control"){

	// Create and add menu bar with menus

	wxMenuBar * menuBar = new wxMenuBar;
	wxMenu * menuFile = new wxMenu;
	wxMenu * menuSerial = new wxMenu;
	wxMenu * menuPIL = new wxMenu;
	wxMenu * menuHelp = new wxMenu;

	// Serial Menu
	menuSerial->Append(ID_CONNECT_SERIAL, _("Connect to a Serial Port"));

	// Pil Menu
	menuPIL->Append(ID_SEND_PIL_COMMAND, _("Send Commands to the PIL"));
	menuPIL->AppendSeparator();
	menuPIL->Append(ID_READ_PIL_STATUS, _("Get PIL Status"));

	// Help Menu
	menuHelp->Append(ID_DOC, _("Documentation"));
	menuHelp->Append(ID_ABOUT, _("About"));

	// Append menus to menu bar
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSerial, _("&Serial Connections"));
	menuBar->Append(menuPIL, _("&PIL"));
	menuBar->Append(menuHelp, _("&Help"));

	// Set the menu bar
	this->SetMenuBar(menuBar);

	// Set the status bar
	this->CreateStatusBar();
}