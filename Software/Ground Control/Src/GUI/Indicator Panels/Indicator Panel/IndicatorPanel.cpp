#include "IndicatorPanel.h"

IndicatorPanel::IndicatorPanel(wxWindow * parent) : wxPanel(parent) {

	// Bind events needed for the panel
	this->Bind(wxEVT_PAINT, (wxObjectEventFunction)&IndicatorPanel::OnPaint, this);
	this->Bind(wxEVT_SIZE, (wxObjectEventFunction)&IndicatorPanel::OnSize, this);
	//this->Bind(wxEVT_ERASE_BACKGROUND, (wxObjectEventFunction)&RadioSignalStrength::OnEraseBackGround, this);
}

void IndicatorPanel::PaintNow() {

	// Get the drawing context, then the buffered drawing context.
	// Paint with buffered drawing context to avoid flicker.

	if (this->IsShown()) {
		wxClientDC dc(this);
		// Make sure the client dc is okay, if not, return
		if (!dc.IsOk()) {
			return;
		}
		Render(dc);
	}
}

void IndicatorPanel::OnPaint(wxPaintEvent& paintEvent) {

	// Get the the the buffered paint drawing context.
	// Paint with buffered paint drawing context to avoid flicker.
	wxBufferedPaintDC dcBuf(this);

	// Make sure the dc is okay, if not, return
	if(!dcBuf.IsOk()) {
		return;
	}
	Render(dcBuf);
	paintEvent.Skip();
}

void IndicatorPanel::OnSize(wxSizeEvent& sizeEvent) {

	// Repaint the window when it is resized.
	this->Refresh();
	sizeEvent.Skip();
}

void IndicatorPanel::Render(wxDC& dc) {

	wxBrush blackBrush(wxColor(0, 0, 0));
	dc.SetBackground(blackBrush);
	dc.Clear();
}