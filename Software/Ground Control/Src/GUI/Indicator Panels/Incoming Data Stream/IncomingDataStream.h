#ifndef INCOMING_DATA_STREAM_H
#define INCOMING_DATA_STREAM_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


class IncomingDataStream : public wxPanel {

public:
	IncomingDataStream(wxWindow * parent, wxString title, int maxLines = 50);
	void SetMaxLines(int maxLines);
	void SetAutoscroll(bool setAutoscroll);
	void AppendText(wxString appendedText);
	void ClearText();

private:

	wxBoxSizer * layout;
	wxTextCtrl * titleText;
	wxTextCtrl * dataText;
	
	wxString allText;
	wxVector<wxString> allLines;

	int maxLineNum;
	bool autoScroll;

	long numLines;
};

#endif