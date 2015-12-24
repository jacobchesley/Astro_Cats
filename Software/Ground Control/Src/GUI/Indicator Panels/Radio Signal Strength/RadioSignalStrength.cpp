#include "RadioSignalStrength.h"

// Radio Signal Strength Class
RadioSignalStrength::RadioSignalStrength(wxWindow * parent, wxString title) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	bars = new RadioSignalStrengthBar(this);

	titleText = new wxTextCtrl(this, -1);
	titleText->SetBackgroundColour(wxColor(0, 0, 0));
	titleText->SetForegroundColour(wxColor(255, 255, 255));
	titleText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleText->SetValue(title);
	titleText->SetEditable(false);

	signalText = new wxTextCtrl(this, -1);
	signalText->SetBackgroundColour(wxColor(0, 0, 0));
	signalText->SetForegroundColour(wxColor(255, 255, 255));
	signalText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	signalText->SetEditable(false);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(bars, 1, wxEXPAND);
	this->GetSizer()->Add(signalText, 0, wxEXPAND);

	this->GetSizer()->Layout();
}

void RadioSignalStrength::SetRadioSignalStrength(int signalStrength) {
	bars->SetNumBars(signalStrength);
	signalText->SetValue(std::to_string(signalStrength));
}

void RadioSignalStrength::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

// Radio Signal Strength Bars class
RadioSignalStrengthBar::RadioSignalStrengthBar(wxWindow * parent) : IndicatorPanel(parent){
	signalStrength = 0;
	this->PaintNow();
}

void RadioSignalStrengthBar::SetNumBars(int newSignalStrength){
	if (newSignalStrength > 7) {
		signalStrength = 7;
	}
	else if (newSignalStrength < 0) {
		signalStrength = 0;
	}
	else {
		signalStrength = newSignalStrength;
	}
	this->PaintNow();
}

void RadioSignalStrengthBar::Render(wxDC& dc){

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

	// Determine color of signal strengh bars based on signal strength
	// Signal Strength - Color
	// 1 - Red
	// 2 - Dark Orange
	// 3 - Light Orange
	// 4 - Yellow
	// 5 - Yellow Green
	// 6 - Mostly Green, little yellow
	// 7 - Green
	wxColor barColor;
	switch (signalStrength) {

		// Set strike through to red, no signal
		case 0:
			barColor.SetRGB(0x000000FF);
			break;

		// Set barColor to red
		case 1:
			barColor.SetRGB(0x000000FF);
			break;

		// Set barColor to dark orange
		case 2:
			barColor.SetRGB(0x000066FF);
			break;

		// Set barColor to light orange
		case 3:
			barColor.SetRGB(0x0000AAFF);
			break;

		// Set barColor to yellow
		case 4:
			barColor.SetRGB(0x0000FFFF);
			break;

		// Set barColor to yellow green
		case 5:
			barColor.SetRGB(0x00FFAA);
			break;

		// Set barColor to mostly green, little yellow
		case 6:
			barColor.SetRGB(0x0000FF66);
			break;

		// Set barColor to green
		case 7:
			barColor.SetRGB(0x0000FF00);
			break;

		// Blue is default to show error
		default:
			barColor.SetRGB(0x00FF0000);
	}

	// Set color of drawing
	dc.SetPen(wxPen(barColor));
	dc.SetBrush(wxBrush(barColor));

	// If there is a signal, draw signal bars
	if(signalStrength > 0){

		// Determine bar width and base height
		int barWidth = width / 10;
		int barOffset = barWidth + barWidth/5;
		int baseBarHeight = height / 10;

		// Draw signal strength bars.  Increase hieght of bar as signal increases
		int xOffset = 0;
		for(int bar = 0; bar < signalStrength + 1; bar++){

			int startX = xOffset;
			int startY = height;
			dc.DrawRectangle(startX, startY, barWidth, -1* baseBarHeight * (bar));
			xOffset += barOffset;
		
		}
	}
	else {

		// Draw outside of the circle that is red.  A full red circle is drawn
		int circleRadius = 0;
		if (width < height) {
			circleRadius = width / 2;
		}
		else {
			circleRadius = height / 2;
		}
		
		int circleX = width / 2;
		int circleY = height / 2;

		dc.DrawCircle(circleX, circleY, circleRadius);

		// Draw a black circle that is smaller in radious, to give the effect of a red outline circle.
		dc.SetBrush(wxBrush(wxColor(0, 0, 0)));
		dc.DrawCircle(circleX, circleY, circleRadius - 5);

		// Set color of drawing back to original color, red.
		dc.SetBrush(wxBrush(barColor));

		// x = r sin (theta) - theta = 1.25 pi (halfway between pi and 3pi / 2.
		// y = r cos (theta) - theta = 1.25 pi (halfway between pi and 3pi / 2.
		int startX = ((float)circleRadius * -0.7071067811f);
		int startY = ((float)circleRadius * -0.7071067811f);
		startX += circleX;
		startY += circleY;

		// x = r sin (theta) - theta = 0.25 pi (halfway between 0 and pi / 2.
		// y = r cos (theta) - theta = 0.25 pi (halfway between 0 and pi / 2.
		int endX = ((float)circleRadius * 0.7071067811f);
		int endY = ((float)circleRadius * 0.7071067811f);
		endX += circleX;
		endY += circleY;

		wxPen linePen(barColor);
		linePen.SetWidth(5);
		dc.SetPen(linePen);
		dc.DrawLine(startX, startY, endX, endY);
	}
}

RadioSignalStrengthWindow::RadioSignalStrengthWindow(wxWindow * parent, wxString name) :wxFrame(parent, -1, name){

	inside = new RadioSignalStrength(this, name);
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&RadioSignalStrengthWindow::OnClose, this);
}

void RadioSignalStrengthWindow::SetNumBars(int numBars) {
	inside->SetRadioSignalStrength(numBars);
}

void RadioSignalStrengthWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Skip();
}