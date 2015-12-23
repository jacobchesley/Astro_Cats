#ifndef LINEAR_GUAGE_H
#define LINEAR_GUAGE_H

// Include generic indicator pane;
#include "GUI/Indicator Panels/Indicator Panel/IndicatorPanel.h"
#include "wx/numformatter.h"
class LinearBar;

class LinearDisplay : public wxPanel {
public:
	LinearDisplay(wxWindow * parent, wxString title, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnit, wxString rightUnit, int numMarks, float m, float b);
	void SetValue(float newValue);
	void SetTitle(wxString newTitle);

private:
	wxBoxSizer * layout;
	LinearBar * bar;
	wxTextCtrl * titleText;
	wxTextCtrl * bottomText;

};
/**
RadioSignalStrengthBar is the GUI panel that indicates the radios signal strength.
This GUI diplays bars similar to cell phone bar icons, and displays a numercial value
1-7 describing the strength of the last received packet.
*/
class LinearBar : public IndicatorPanel {

public:
	/**
	Constructor for the radio signal strength panel.
	*/
	LinearBar(wxWindow * parent, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnitIn, wxString rightUnitIn, int numMarksIn, float m, float b);

	/**
	Set the number of bars to display.  0 - 7.
	*/
	void SetValue(float newValue);
	float GetLeft();
	float GetRight();
	wxString GetLeftUnits();
	wxString GetRightUnits();

	bool LeftEnabled();
	bool RightEnabled();

	//wxString GetRightUnit();

private:

	float curVal;
	float minVal;
	float maxVal;
	float mapM;
	float mapB;
	wxString leftUnit;
	wxString rightUnit;
	wxColor barCol;
	bool enableLeft;
	bool enableRight;
	int numMarkers;
	void Render(wxDC& dc);
};

class LinearWindow : public wxFrame {
public:
	LinearWindow(wxWindow * parent, wxString name, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnit, wxString rightUnit, int numMarks, float m = 1.0f, float b = 0.0f);
	void SetValue(float temperature);
	//void LeftValue(bool newLeftValue);
	//void RightValue(bool newRightValue);

private:
	LinearDisplay * inside;
	wxBoxSizer * layout;
	void OnClose(wxCloseEvent& evt);
};
#endif