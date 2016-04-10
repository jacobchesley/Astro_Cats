#ifndef MAIN_COMMAND_WINDOW_H
#define MAIN_COMMAND_WINDOW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/notebook.h"
#include "wx/listctrl.h"

#include "JSON/json.cpp"
#include "Serial/SerialController.h"

class CommandDisplay : public wxPanel {
public:
	CommandDisplay(wxWindow * parent, SerialController * controller, wxString destination, wxString commandNameTitle, wxString commandNameJSON, wxVector<wxString> parametersListDisplay, wxVector<wxString> parametersListJSON, int defaultParam = 0);
	wxString GetCommandNameTitle();
private:

	SerialController * control;

	void OnFireCommand(wxCommandEvent& WXUNUSED(event));
	wxString BuildJSONCommandString(bool indent = false);
	
	wxString titleName;

	wxString destination;
	wxString jsonCommand;
	wxVector<wxString> jsonParams;

	wxBoxSizer * layout;

	wxTextCtrl * commandNameDisplay;
	wxComboBox * parametersBox;
	wxButton * fireCommand;

	bool parametersShown;

	enum {
		FIRE_COMMAND
	};
};

class PILCommandDisplay : public wxPanel {

public:
	PILCommandDisplay(wxWindow * parent, SerialController * controller);

private:
	wxBoxSizer * layout;
	wxNotebook * notebook;

	CommandDisplay * setRadioPowerCommand;
	CommandDisplay * setUpdateRateCommand;
	CommandDisplay * takePhotoCommand;
	CommandDisplay * lockParachuteCommand;
	CommandDisplay * releaseParachuteCommand;
};

class RocketCommandDisplay : public wxPanel {
public:
	RocketCommandDisplay(wxWindow * parent, SerialController * controller);

private:
	wxBoxSizer * layout;
	wxNotebook * notebook;

	CommandDisplay * setRadioPowerCommand;
	CommandDisplay * setUpdateRateCommand;
};

class MainCommandWindow : public wxFrame {
public:
	MainCommandWindow(wxWindow * parent, wxString name, SerialController * controller);

private:
	
	wxBoxSizer * layout;
	wxNotebook * notebook;

	RocketCommandDisplay * rocketCommands;
	PILCommandDisplay * pilCommands;

	void OnClose(wxCloseEvent& evt);
};

class CommandResponseWindow : public wxFrame {
public:
	CommandResponseWindow(wxWindow * parent, wxString name);

	void RecieveResponseRocket(wxString commandName, wxString commandValue);
	void RecieveResponsePIL(wxString commandName, wxString commandValue);

private:

	wxBoxSizer * layout;
	wxNotebook * notebook;

	wxListCtrl * pilCommands;
	wxListCtrl * rocketCommands;

	void OnClose(wxCloseEvent& evt);
};
#endif 
