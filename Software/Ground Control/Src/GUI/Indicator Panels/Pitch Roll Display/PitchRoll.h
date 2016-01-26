#ifndef PITCH_ROLL_DISPLAY
#define PITCH_ROLL_DISPLAY

#include "GUI/Indicator Panels/Indicator Panel/IndicatorPanel.h"
#include <wx\glcanvas.h>
#include <gl\GLU.h>

class PitchRollView;

class PitchRollDisplay : public wxPanel {
public:
	PitchRollDisplay(wxWindow * parent, wxString title);
	void SetXAcceleration(float x);
	void SetYAcceleration(float y);
	void SetZAcceleration(float z);
	void SetAccelerationData(float x, float y, float z);
	void UpdateDisplay();
	void SetTitle(wxString newTitle);

private:
	wxBoxSizer * layout;

	wxTextCtrl * titleText;
	PitchRollView * view;
	wxTextCtrl * bottomText;

};

class PitchRollView : public wxGLCanvas {
public:
	PitchRollView(wxWindow * parent);
	void SetXAcceleration(float x);
	void SetYAcceleration(float y);
	void SetZAcceleration(float z);
	void SetAccelerationData(float x, float y, float z);
	void PaintNow();

private:

	void Render();
	void OnPaint(wxPaintEvent& paintEvent);
	void OnSize(wxSizeEvent& sizeEvent);

	wxGLContext * pitchRollContext;
	float xAccel;
	float yAccel;
	float zAccel;

};

class PitchRollWindow : public wxFrame {
public:
	PitchRollWindow(wxWindow * parent, wxString name);
	void SetXAcceleration(float x);
	void SetYAcceleration(float y);
	void SetZAcceleration(float z);
	void SetAccelerationData(float x, float y, float z);
	void UpdateDisplay();

private:
	PitchRollDisplay * inside;
	wxBoxSizer * layout;
	void OnClose(wxCloseEvent& evt);
};
#endif