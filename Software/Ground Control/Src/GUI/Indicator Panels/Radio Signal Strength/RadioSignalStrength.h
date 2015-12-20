#ifndef RADIO_SIGNAL_STRENGTH_H
#define RADIO_SIGNAL_STRENGTH_H

// Include generic indicator pane;
#include "GUI/Indicator Panels/Indicator Panel/IndicatorPanel.h"

class RadioSignalStrengthBar;

class RadioSignalStrength : public wxPanel {
	public:
		RadioSignalStrength(wxWindow * parent, wxString title);
		void SetRadioSignalStrength(int signalStrength);
		void SetTitle(wxString newTitle);

private:
	wxBoxSizer * layout;
	RadioSignalStrengthBar * bars;
	wxTextCtrl * titleText;
	wxTextCtrl * signalText;

};
/**
RadioSignalStrengthBar is the GUI panel that indicates the radios signal strength.
This GUI diplays bars similar to cell phone bar icons, and displays a numercial value
1-7 describing the strength of the last received packet.
*/
class RadioSignalStrengthBar : public IndicatorPanel {

	public:
		/**
		Constructor for the radio signal strength panel.
		*/
		RadioSignalStrengthBar(wxWindow * parent);

		/**
		Set the number of bars to display.  0 - 7.
		*/
		void SetNumBars(int numBars);

	private:

		int signalStrength;
		void Render(wxDC& dc);
};

class RadioSignalStrengthWindow : public wxFrame {
	public:
		RadioSignalStrengthWindow(wxWindow * parent, wxString name);
		void SetNumBars(int numBars);

	private:
		RadioSignalStrength * inside;
		wxBoxSizer * layout;
		void OnClose(wxCloseEvent& evt);
};
#endif