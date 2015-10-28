#ifndef INDICATOR_PANEL_H
#define INDICATOR_PANEL_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dcbuffer.h"

/**
Indicator Panel is a generic panel with class specific drawing code.  This generic panel
catches paint and resize events to redraw the class specific shapes
*/
class IndicatorPanel : public wxPanel {

public:
	/**
	Constructor for the radio signal strength panel.
	*/
	IndicatorPanel(wxWindow * parent);

	/**
	Repaint the GUI.
	*/
	void PaintNow();
private:

	/**
	Custom drawing code goes in Render method
	*/
	virtual void Render(wxDC& dc);
	void OnPaint(wxPaintEvent& paintEvent);
	void OnSize(wxSizeEvent& sizeEvent);
};

/**
Enumumerations for main window.
*/
enum {
	ID_CONNECT_SERIAL,
	ID_SEND_PIL_COMMAND,
	ID_READ_PIL_STATUS,
	ID_DOC,
	ID_ABOUT
};
#endif