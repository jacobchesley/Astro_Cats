#include "LinearGuage.h"

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

	wxString leftString = wxNumberFormatter::ToString(bar->GetLeft(), 2);
	wxString rightString = wxNumberFormatter::ToString(bar->GetRight(), 2);

	if (bar->LeftEnabled() && bar->RightEnabled()) {
		bottomText->SetValue(leftString + bar->GetLeftUnits() + "   " + rightString + bar->GetRightUnits());
	}
	if (bar->LeftEnabled() && !bar->RightEnabled()) {
		bottomText->SetValue(leftString + bar->GetLeftUnits());
	}
	if (!bar->LeftEnabled() && bar->RightEnabled()) {
		bottomText->SetValue(rightString + bar->GetRightUnits());
	}
}

void LinearDisplay::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

// Linear Bar class used to draw the guage along with left and right hand side units
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

float LinearBar::GetLeft() {
	return curVal;
}

float LinearBar::GetRight() {
	return (curVal * mapM) + mapB;
}

wxString LinearBar::GetLeftUnits() {
	return leftUnit;
}

wxString LinearBar::GetRightUnits() {
	return rightUnit;
}

bool LinearBar::LeftEnabled() {
	return enableLeft;
}

bool LinearBar::RightEnabled() {
	return enableRight;
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

	// Set the color of the guage, and draw the linear bar guage itself
	dc.SetPen(wxPen(barCol));
	dc.SetBrush(wxBrush(barCol));
	dc.DrawRectangle(width/4, height, width/2, -((newVal/100.0f)* height));

	// Set the color of dc text, to draw the units on left and or right side
	dc.SetTextForeground(wxColor(255, 255, 255));
	dc.SetPen(wxColor(255, 255, 255));
	dc.SetBrush(wxColor(255, 255, 255));

	// Draw left hand side units
	if (enableLeft) {

		// Calculate how big the increment is in left units
		float leftInc = difference / (float)numMarkers;
	
		for (int i = 0; i < numMarkers; i++) {

			// Find new value of left units display and store in string
			float leftVal = leftInc * (float)(numMarkers - i);
			leftVal += minVal;
			wxString leftString = wxNumberFormatter::ToString(leftVal, 2);

			// Calculate where to draw string, draw the string and line marker on the guage
			int drawHeight = i * ((float)height / (float)numMarkers);
			dc.DrawText(leftString + leftUnit, 0, drawHeight);
			dc.DrawLine(0, drawHeight, width/3, drawHeight);
		}		
	}

	// Draw right hand side units
	if (enableRight) {

		// Map the left units to the right units, using y = mx +b
		float mapMax = (maxVal * mapM) + mapB;
		float mapMin = (minVal * mapM) + mapB;
		float mapDifference = mapMax - mapMin;

		// Calculate how big the increment is in left units
		float rightInc = mapDifference / (float)numMarkers;

		for (int i = 0; i < numMarkers; i++) {

			// Find new value of right units display and store in string
			float rightVal = rightInc * (float)(numMarkers - i);
			rightVal += mapMin;
			wxString rightString = wxNumberFormatter::ToString(rightVal, 2);
			
			// Calculate where to draw string, draw the string and line marker on the guage
			int drawHeight = i * ((float)height / (float)numMarkers);
			dc.DrawText(rightString + rightUnit, width - 50, drawHeight);
			dc.DrawLine((2*width) / 3, drawHeight, width, drawHeight);
		}
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