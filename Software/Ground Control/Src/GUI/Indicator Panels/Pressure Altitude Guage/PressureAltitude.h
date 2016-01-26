#ifndef PRESSURE_ALTITUDE_GUAGE
#define PRESSURE_ALTITUDE_GUAGE

// Include generic indicator pane;
#include "GUI/Indicator Panels/Indicator Panel/IndicatorPanel.h"
#include "GUI/Indicator Panels/Linear Guage/LinearGuage.h"
#include "wx/numformatter.h"

class PressureAltitudeDisplay : public wxPanel {
public:
	PressureAltitudeDisplay(wxWindow * parent, wxString title, float minPressure, float maxPressure, int numPressureMarks, float minAltitude, float maxAltitude, int numAltitudeMarks);
	void SetPressure(float newValue);
	void SetTitle(wxString newTitle);
	void SetGround(wxCommandEvent& WXUNUSED(event));

private:

	wxBoxSizer * barLayout;
	wxBoxSizer * layout;

	LinearBar * pressureBar;
	LinearBar * altitudeBar;
	wxTextCtrl * titleText;
	wxTextCtrl * bottomText;
	wxButton * setGround;

	float lastFeet;
	float groundFeet;

	enum
	{
		ID_SET_GROUND
	};

};


class PressureAltitudeWindow : public wxFrame {
public:
	PressureAltitudeWindow(wxWindow * parent, wxString name, float minPressure, float maxPressure, int numPressureMarks, float minAltitude, float maxAltitude, int numAltitudeMarks);
	void SetPressure(float pressure);

private:
	PressureAltitudeDisplay * inside;
	wxBoxSizer * layout;
	void OnClose(wxCloseEvent& evt);
};
#endif
