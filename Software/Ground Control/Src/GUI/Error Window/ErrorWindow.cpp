#include "ErrorWindow.h"

ErrorWindow::ErrorWindow(wxWindow * parent, wxString name) : wxFrame(parent, -1, name) {
	// Create the notebook
	notebook = new wxNotebook(this, -1);

	// Create the rocket and PIL successful Errors windows, each will have their own page in the notebook
	rocketErrors = new wxListCtrl(notebook, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	pilErrors = new wxListCtrl(notebook, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

	// Style the command lists
	rocketErrors->SetBackgroundColour(wxColor(0, 0, 0));
	rocketErrors->SetForegroundColour(wxColor(255, 255, 255));
	rocketErrors->InsertColumn(0, "Error Title");
	rocketErrors->InsertColumn(1, "Error Description");

	pilErrors->SetBackgroundColour(wxColor(0, 0, 0));
	pilErrors->SetForegroundColour(wxColor(255, 255, 255));
	pilErrors->InsertColumn(0, "Error Title");
	pilErrors->InsertColumn(1, "Error Description");

	// Add the pages
	notebook->InsertPage(0, rocketErrors, "Rocket Errors", true);
	notebook->InsertPage(1, pilErrors, "PIL Errors");

	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(notebook, 1, wxEXPAND);

	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&ErrorWindow::OnClose, this);
}

void ErrorWindow::RecieveErrorRocket(wxString errorTitle, wxString errorDescription) {
	long index = pilErrors->InsertItem(100000000, errorTitle);
	pilErrors->SetItem(index, 1, errorDescription);
}

void ErrorWindow::RecieveErrorPIL(wxString errorTitle, wxString errorDescription) {
	long index = pilErrors->InsertItem(100000000, errorTitle);
	pilErrors->SetItem(index, 1, errorDescription);
}

void ErrorWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}