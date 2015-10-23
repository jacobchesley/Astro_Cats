#ifndef RADIO_SIGNAL_STRENGTH_H
#define RADIO_SIGNAL_STRENGTH_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dcbuffer.h"

/**
RadioSignalStrength is the GUI panel that indicates the radios signal strength.
This GUI diplays bars similar to cell phone bar icons, and displays a numercial value
1-7 describing the strength of the last received packet.
*/
class RadioSignalStrength : public wxPanel {

	public:
		/**
		Constructor for the radio signal strength panel.
		*/
		RadioSignalStrength(wxWindow * parent);

		/**
		Set the signal strength.  Display is automatically updated.
		*/
		void SetRadioSignalStrength(int strength);

		/**
		Repaint the GUI.
		*/
		void PaintNow();
	private:

		int signalStrength;
		void Render(wxDC& dc);
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