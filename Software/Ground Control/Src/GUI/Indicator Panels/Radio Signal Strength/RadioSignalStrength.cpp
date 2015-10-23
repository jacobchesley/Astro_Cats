#include "RadioSignalStrength.h"

RadioSignalStrength::RadioSignalStrength(wxWindow * parent) : wxPanel(parent){
	signalStrength = 0;

	// Bind events needed for the panel
	this->Bind(wxEVT_PAINT, (wxObjectEventFunction)&RadioSignalStrength::OnPaint, this);
	this->Bind(wxEVT_SIZE, (wxObjectEventFunction)&RadioSignalStrength::OnSize, this);
	//this->Bind(wxEVT_ERASE_BACKGROUND, (wxObjectEventFunction)&RadioSignalStrength::OnEraseBackGround, this);
}

void RadioSignalStrength::SetRadioSignalStrength(int newSignalStrength){
	if (newSignalStrength > 7) {
		signalStrength = 7;
	}
	else if (newSignalStrength < 1) {
		signalStrength = 1;
	}
	else {
		signalStrength = newSignalStrength;
	}
}

void RadioSignalStrength::PaintNow(){

	// Get the drawing context, then the buffered drawing context.
	// Paint with buffered drawing context to avoid flicker.
	wxClientDC dc(this);
	wxBufferedDC dcBuf(&dc);
	Render(dcBuf);
}

void RadioSignalStrength::OnPaint(wxPaintEvent& paintEvent) {

	// Get the the the buffered paint drawing context.
	// Paint with buffered paint drawing context to avoid flicker.
	wxBufferedPaintDC dcBuf(this);
	Render(dcBuf);
	paintEvent.Skip();
}

void RadioSignalStrength::OnSize(wxSizeEvent& sizeEvent) {

	// Repaint the window when it is resized.
	this->Refresh();
	sizeEvent.Skip();
}

void RadioSignalStrength::Render(wxDC& dc){

	OutputDebugStringA("RENDERING");
	wxBrush blackBrush(wxColor(0, 0, 0));
	dc.SetBackground(blackBrush);
	dc.Clear();
}