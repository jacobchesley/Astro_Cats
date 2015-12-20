#include "Temperature.h"

// Radio Signal Strength Class
LinearDisplay::LinearDisplay(wxWindow * parent, wxString title, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnit, wxString rightUnit, int numMarks, float m, float b) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	bar = new LinearBar(this, minValue, maxValue, barColor, leftValue, rightValue, leftUnit, rightUnit, numMarks, m, b);

	titleText = new wxTextCtrl(this, -1);
	titleText->SetBackgroundColour(wxColor(0, 0, 0));
	titleText->SetForegroundColour(wxColor(255, 255, 255));
	titleText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleText->SetValue(title);
	titleText->SetEditable(false);

	bottomText = new wxTextCtrl(this, -1);
	bottomText->SetBackgroundColour(wxColor(0, 0, 0));
	bottomText->SetForegroundColour(wxColor(255, 255, 255));
	bottomText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	bottomText->SetEditable(false);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(bar, 1, wxEXPAND);
	this->GetSizer()->Add(bottomText, 0, wxEXPAND);

	this->GetSizer()->Layout();
}

void LinearDisplay::SetValue(float newValue) {
	bar->SetValue(newValue);

	// NEED TO DO
	//bottomText->SetValue(std::to_string(temperature) + " C,   " + std::to_string((temperature*1.8) + 32.0f) + " F");
}

void LinearDisplay::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

// Temperature Bar class
LinearBar::LinearBar(wxWindow * parent, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnitIn, wxString rightUnitIn, int numMarks, float m, float b) : IndicatorPanel(parent) {

	curVal = minValue;
	minVal = minValue;
	maxVal = maxValue;
	enableLeft = leftValue;
	enableRight = rightValue;
	leftUnit = leftUnitIn;
	rightUnit = rightUnitIn;
	barCol = barColor;
	numMarkers = numMarks;
	mapM = m;
	mapB = b;
	this->PaintNow();
}

void LinearBar::SetValue(float newValue) {
	
	curVal = newValue;
	this->PaintNow();
}

void LinearBar::Render(wxDC& dc) {

	// Make sure the dc is okay, if not, return
	if (!dc.IsOk()) {
		return;
	}

	wxBrush blackBrush(wxColor(0, 0, 0));
	dc.SetBackground(blackBrush);
	dc.Clear();

	// Get size of panel to calculate drawing dimmensions
	int width = this->GetSize().GetWidth();
	int height = this->GetSize().GetHeight();

	// Map from current custom range to 0-100
	float shiftVal = curVal - minVal;
	float difference = maxVal - minVal;
	float multiplier = 100.0f / difference;

	float newVal = shiftVal * multiplier;

	dc.SetPen(wxPen(barCol));
	dc.SetBrush(wxBrush(barCol));

	dc.DrawRectangle(width/4, height, width/2, -((newVal/100.0f)* height));

	dc.SetTextForeground(wxColor(255, 255, 255));
	dc.SetPen(wxColor(255, 255, 255));
	dc.SetBrush(wxColor(255, 255, 255));
	// Draw left hand side numbers
	if (enableLeft) {
		float leftInc = difference / (float)numMarkers;

		for (int i = 0; i < numMarkers; i++) {
			float leftVal = leftInc * (float)(numMarkers - i);
			leftVal += minVal;
			wxString leftString = wxNumberFormatter::ToString(leftVal, 2);

			int drawHeight = i * ((float)height / (float)numMarkers);
			dc.DrawText(leftString, 0, drawHeight);
			dc.DrawLine(0, drawHeight, width/3, drawHeight);
		}		
	}
	if (enableRight) {

		float mapMax = (maxVal * mapM) + mapB;
		float mapMin = (minVal * mapM) + mapB;

		float mapDifference = mapMax - mapMin;
		float rightInc = mapDifference / (float)numMarkers;

		for (int i = 0; i < numMarkers; i++) {
			float rightVal = rightInc * (float)(numMarkers - i);
			rightVal += mapMin;
			wxString rightString = wxNumberFormatter::ToString(rightVal, 2);
			
			int drawHeight = i * ((float)height / (float)numMarkers);
			dc.DrawText(rightString, width - 50, drawHeight);
			dc.DrawLine((2*width) / 3, drawHeight, width, drawHeight);
		}
		wxString minValString;
		minValString << minVal;
		dc.DrawText(minValString, 0, height);
	}
}

LinearWindow::LinearWindow(wxWindow * parent, wxString name, float minValue, float maxValue, wxColor barColor, bool leftValue, bool rightValue, wxString leftUnit, wxString rightUnit, int numMarks, float m, float b) :wxFrame(parent, -1, name) {

	inside = new LinearDisplay(this, name, minValue, maxValue, barColor, leftValue, rightValue, leftUnit, rightUnit, numMarks, m, b);
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&LinearWindow::OnClose, this);
}
void LinearWindow::SetValue(float newValue) {
	inside->SetValue(newValue);
}

void LinearWindow::OnClose(wxCloseEvent& evt){
	this->Hide();
}