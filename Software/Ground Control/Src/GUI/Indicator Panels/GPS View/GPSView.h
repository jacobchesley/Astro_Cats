#ifndef GPS_VIEW_H
#define GPS_VIEW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/webview.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"

class GPSView : public wxFrame {
public:
	GPSView(wxWindow * parent, wxString title);
	void UpdateGPSPos();

private:
	void CreateHTML();
	wxBoxSizer * layout;
	wxTextCtrl * titleText;
	wxWebView * web;
	wxString htmlText;
};
#endif