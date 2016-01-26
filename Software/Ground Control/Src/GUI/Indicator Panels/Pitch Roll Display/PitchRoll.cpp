#include "PitchRoll.h"

PitchRollDisplay::PitchRollDisplay(wxWindow * parent, wxString title) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

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

	view = new PitchRollView(this);

	layout = new wxBoxSizer(wxVERTICAL);

	this->SetSizer(layout);
	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(view, 1, wxEXPAND);
	this->GetSizer()->Add(bottomText, 0, wxEXPAND);
	this->GetSizer()->Layout();
}

void PitchRollDisplay::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

void PitchRollDisplay::SetXAcceleration(float x) {
	view->SetXAcceleration(x);
	this->UpdateDisplay();
}

void PitchRollDisplay::SetYAcceleration(float y) {
	view->SetYAcceleration(y);
	this->UpdateDisplay();
}

void PitchRollDisplay::SetZAcceleration(float z) {
	view->SetZAcceleration(z);
	this->UpdateDisplay();
}

void PitchRollDisplay::SetAccelerationData(float x, float y, float z) {
	view->SetAccelerationData(x, y, z);
	this->UpdateDisplay();
}

void PitchRollDisplay::UpdateDisplay() {
	view->PaintNow();
}


PitchRollView::PitchRollView(wxWindow * parent) : wxGLCanvas(parent, -1, wxPoint(0,0)) {
	this->Bind(wxEVT_PAINT, (wxObjectEventFunction)&PitchRollView::OnPaint, this);
	xAccel = 0.0f;
	yAccel = 0.0f;
	zAccel = 9.1f;

	pitchRollContext = new wxGLContext(this);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

void PitchRollView::SetXAcceleration(float x) {
	xAccel = x;
}

void PitchRollView::SetYAcceleration(float y) {
	yAccel = y;
}

void PitchRollView::SetZAcceleration(float z) {
	zAccel = z;
}

void PitchRollView::SetAccelerationData(float x, float y, float z) {
	xAccel = x;
	yAccel = y;
	zAccel = z;
}

void PitchRollView::PaintNow() {

	this->Refresh();
}

void PitchRollView::Render() {

	
	float x2z2 = (xAccel * xAccel) + (zAccel * zAccel);
	float y2z2 = (yAccel * yAccel) + (zAccel * zAccel);

	float pitch = atan(xAccel / sqrt(x2z2));
	float roll = atan(yAccel / sqrt(y2z2));

	float pi = 3.14159265358979323846;
	float pitchDeg = (pitch * 180) / pi;
	float rollDeg = (roll * 180) / pi;

	wxGLCanvas::SetCurrent(*pitchRollContext);

	glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Draw gradient background colors
	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);

		// Bottom Left Off White
		glColor3f(0.86, 0.90, 0.92);
		glVertex2f(-1.5, 1.5);
		glVertex2f(-1.5, -1.5);

		// Top Right Teal
		glColor3f(0.47, 0.71, 0.88);
		glVertex2f(1.5, -1.5);
		glVertex2f(1.5, 1.5);
	glEnd();
	glPopMatrix();

	// Push matrix, to rotate only the object we are drawing
	glPushMatrix();

	// Scale to get longer rectangle
	glScalef(0.3, 1.0, 0.3);

	// Rotate Pitch
	glRotatef(pitchDeg, 0.0, 0.0, 1.0);
	// Rotate Roll
	glRotatef(rollDeg, 1.0, 0.0, 0.0);


	// Top - Yellow
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.5, 0.5, 0.5);
		glVertex3f(0.5, 0.5, -0.5);
		glVertex3f(-0.5, 0.5, -0.5);
		glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	// Bottom - Grey
	glBegin(GL_POLYGON);
		glColor3f(0.2, 0.2, 0.2);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(0.5, -0.5, 0.5);
		glVertex3f(-0.5, -0.5, 0.5);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	//Back - White
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.5, -0.5, 0.5);
		glVertex3f(0.5, 0.5, 0.5);
		glVertex3f(-0.5, 0.5, 0.5);
		glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	// Right Side - Light Grey
	glBegin(GL_POLYGON);
		glColor3f(0.85, 0.85, 0.85);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(0.5, 0.5, -0.5);
		glVertex3f(0.5, 0.5, 0.5);
		glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	// Left Side - Light Grey
	glBegin(GL_POLYGON);
		glColor3f(0.85, 0.85, 0.85);
		glVertex3f(-0.5, -0.5, 0.5);
		glVertex3f(-0.5, 0.5, 0.5);
		glVertex3f(-0.5, 0.5, -0.5);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	glPopMatrix();
	
	glFlush();
	SwapBuffers();
}

void PitchRollView::OnSize(wxSizeEvent& sizeEvent) {

	// Repaint the window when it is resized.
	this->Refresh();
	sizeEvent.Skip();
}

void PitchRollView::OnPaint(wxPaintEvent& WXUNUSED(event)) {
	Render();
}

PitchRollWindow::PitchRollWindow(wxWindow * parent, wxString name) : wxFrame(parent, -1, name) {
	inside = new PitchRollDisplay(this, name);
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&PitchRollWindow::OnClose, this);
}

void PitchRollWindow::SetXAcceleration(float x) {
	inside->SetXAcceleration(x);
}

void PitchRollWindow::SetYAcceleration(float y) {
	inside->SetYAcceleration(y);
}

void PitchRollWindow::SetZAcceleration(float z) {
	inside->SetZAcceleration(z);
}

void PitchRollWindow::SetAccelerationData(float x, float y, float z) {
	inside->SetAccelerationData(x, y, z);
}

void PitchRollWindow::UpdateDisplay() {
	inside->UpdateDisplay();
}

void PitchRollWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}