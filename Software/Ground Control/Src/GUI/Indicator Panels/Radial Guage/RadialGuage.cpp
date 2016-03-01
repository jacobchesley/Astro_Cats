#include "RadialGuage.h"

RadialDisplay::RadialDisplay(wxWindow * parent, wxString title, float minValue, float maxValue, wxColor barColor, wxString unit, int numMarks) : wxPanel(parent) {

	this->SetBackgroundColour(parent->GetBackgroundColour());

	bar = new RadialBar(this, minValue, maxValue, barColor, unit, numMarks);
	bar->SetBackgroundColour(this->GetBackgroundColour());

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

void RadialDisplay::SetValue(float newValue) {
	bar->SetValue(newValue);

	wxString valString = wxNumberFormatter::ToString(bar->GetValue(), 2);

	bottomText->SetValue(valString + bar->GetUnits());
}

void RadialDisplay::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

// Linear Bar class used to draw the guage along with left and right hand side units
RadialBar::RadialBar(wxWindow * parent, float minValue, float maxValue, wxColor barColor, wxString unitIn, int numMarks) : IndicatorPanel(parent) {

	curVal = minValue;
	minVal = minValue;
	maxVal = maxValue;
	barCol = barColor;
	backColor = parent->GetBackgroundColour();
	numMarkers = numMarks;
	unit = unitIn;
	this->PaintNow();
}

void RadialBar::SetValue(float newValue) {
	
	curVal = newValue;
	this->PaintNow();
}

float RadialBar::GetValue() {
	return curVal;
}

wxString RadialBar::GetUnits() {
	return unit;

}
void RadialBar::Render(wxDC& dc) {

	// Make sure the dc is okay, if not, return
	if (!dc.IsOk()) {
		return;
	}
	
	wxBrush backgroundBrush(backColor);
	dc.SetBackground(backgroundBrush);
	dc.Clear();

	// Get size of panel to calculate drawing dimmensions
	int width = this->GetSize().GetWidth();
	int height = this->GetSize().GetHeight();

	// Map from current custom range to 0-270
	float shiftVal = curVal - minVal;
	float difference = maxVal - minVal;
	float multiplier = 270.0f / difference;

	float newVal = shiftVal * multiplier;

	// Set the color of the guage
	dc.SetPen(wxPen(barCol));
	dc.SetBrush(wxBrush(barCol));

	// Determine radius of circle for outter guage
	int circleRadius = 0;
	if (width < height) {
		circleRadius = width / 2;
	}
	else {
		circleRadius = height / 2;
	}

	// Draw outer guage
	dc.DrawCircle(width / 2, height / 2, circleRadius);

	// Draw inner background color to create circle outline
	dc.SetPen(wxPen(backColor));
	dc.SetBrush(wxBrush(backColor));
	dc.DrawCircle(width / 2, height / 2, circleRadius - 3);

	// Draw rectange across bottom of circle, to give looks of 3/4 of a circle.
	// 4 / (sqrt(2)/2) = 5.65686
	dc.DrawRectangle(0, height - (height / 5.65686f), width, height / 5);

	// Set the color of the guage
	dc.SetPen(wxPen(barCol));
	dc.SetBrush(wxBrush(barCol));

	// Calculate the points of the line representing the guage
	int guageStartX = width / 2;
	int guageStartY = height / 2;

	float pi = 3.14159265358979323846;

	// Draw between 225 degrees and -45 degrees, 270 degrees total movement
	float degreeVal = 225 - newVal;
	wxString degreeStr;
	degreeStr << degreeVal;
	OutputDebugStringA(degreeStr.char_str());

	float guageLen = circleRadius * 0.92f;

	int guageEndX = guageStartX;
	int guageEndY = guageStartY;

	guageEndX += guageLen * cos(pi * (degreeVal/180.0f));
	guageEndY -= guageLen * sin(pi * (degreeVal/180.0f));

	//Draw guage middle circle
	dc.DrawCircle(guageStartX, guageStartY, 3);

	// Draw guage line
	dc.DrawLine(guageStartX, guageStartY, guageEndX, guageEndY);
	
	// Set the color of dc text, to draw the units on left and or right side
	dc.SetTextForeground(wxColor(255, 255, 255));
	dc.SetPen(wxColor(255, 255, 255));
	dc.SetBrush(wxColor(255, 255, 255));
	
}

RadialWindow::RadialWindow(wxWindow * parent, wxString name, float minValue, float maxValue, wxColor barColor,wxString unit,int numMarks) :wxFrame(parent, -1, name) {

	this->SetBackgroundColour(wxColor(0, 0, 0));
	inside = new RadialDisplay(this, name, minValue, maxValue, barColor, unit,  numMarks);
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&RadialWindow::OnClose, this);
	
}
void RadialWindow::SetValue(float newValue) {
	inside->SetValue(newValue);
}

void RadialWindow::OnClose(wxCloseEvent& evt){
	this->Hide();
	evt.Veto();
}