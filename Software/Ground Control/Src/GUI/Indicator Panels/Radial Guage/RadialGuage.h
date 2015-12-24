#ifndef RADIAL_GUAGE_H
#define RADIAL_GUAGE_H

// Include generic indicator pane;
#include "GUI/Indicator Panels/Indicator Panel/IndicatorPanel.h"
#include "wx/numformatter.h"

class RadialBar;

class RadialDisplay : public wxPanel {
public:
	RadialDisplay(wxWindow * parent, wxString title, float minValue, float maxValue, wxColor barColor, wxString unit, int numMarks);
	void SetValue(float newValue);
	void SetTitle(wxString newTitle);

private:
	wxBoxSizer * layout;
	RadialBar * bar;
	wxTextCtrl * titleText;
	wxTextCtrl * bottomText;

};

class RadialBar : public IndicatorPanel {

public:
	/**
	Constructor for the radio signal strength panel.
	*/
	RadialBar(wxWindow * parent, float minValue, float maxValue, wxColor barColor, wxString unitIn, int numMarksIn);

	/**
	Set the number of bars to display.  0 - 7.
	*/
	void SetValue(float newValue);
	float GetValue();
	wxString GetUnits();

	//wxString GetRightUnit();

private:

	float curVal;
	float minVal;
	float maxVal;
	wxString unit;
	wxColor barCol;
	wxColor backColor;
	int numMarkers;
	void Render(wxDC& dc);
};

class RadialWindow : public wxFrame {
public:
	RadialWindow(wxWindow * parent, wxString name, float minValue, float maxValue, wxColor barColor, wxString units, int numMarks);
	void SetValue(float temperature);
	//void LeftValue(bool newLeftValue);
	//void RightValue(bool newRightValue);

private:
	RadialDisplay * inside;
	wxBoxSizer * layout;
	void OnClose(wxCloseEvent& evt);
};
#endif