#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"
 
// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/hyperlink.h"

class AboutWindow : public wxFrame {
public:
	AboutWindow(wxWindow * parent);
	wxString GetVersion();

private:
	void OnClose(wxCloseEvent& evt);
	
	wxBoxSizer * layout;
	wxStaticText * title;
	wxStaticText * description;
	wxHyperlinkCtrl * ucRocketry;
	wxStaticText * author;
	wxStaticText * buildDate;
	wxStaticText * buildNumber;
	wxString versionText;

};

#endif