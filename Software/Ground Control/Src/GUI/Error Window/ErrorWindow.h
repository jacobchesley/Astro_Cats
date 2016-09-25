#ifndef ERROR_WINDOW_H
#define ERROR_WINDOW_H

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

class ErrorWindow : public wxFrame {

public:
	ErrorWindow(wxWindow * parent, wxString name);

	void RecieveErrorRocket(wxString errorTitle, wxString errorDescription);
	void RecieveErrorPIL(wxString errorTitle, wxString errorDescription);

private:

	wxBoxSizer * layout;
	wxNotebook * notebook;

	wxListCtrl * pilErrors;
	wxListCtrl * rocketErrors;

	void OnClose(wxCloseEvent& evt);

};
#endif