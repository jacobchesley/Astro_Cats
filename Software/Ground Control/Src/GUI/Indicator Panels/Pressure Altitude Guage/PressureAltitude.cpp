#include "PressureAltitude.h"

PressureAltitudeDisplay::PressureAltitudeDisplay(wxWindow * parent, wxString title, float minPressure, float maxPressure, int numPressureMarks, float minAltitude, float maxAltitude, int numAltitudeMarks) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	pressureBar = new LinearBar(this, minPressure, maxPressure, wxColor(180, 180, 180), true, false, " hPa", "", numPressureMarks, 0, 0);
	altitudeBar = new LinearBar(this, minAltitude, maxAltitude, wxColor(129, 188, 250), true, false, " Ft", "", numAltitudeMarks, 0, 0);

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

	setGround = new wxButton(this, ID_SET_GROUND, "Set Ground Pressure");
	setGround->SetBackgroundColour(wxColor(40,255, 10));
	setGround->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	
	layout = new wxBoxSizer(wxVERTICAL);
	barLayout = new wxBoxSizer(wxHORIZONTAL);

	barLayout->Add(pressureBar, 1, wxEXPAND);
	barLayout->Add(altitudeBar, 1, wxEXPAND);
	
	this->SetSizer(layout);
	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(barLayout, 1, wxEXPAND);
	this->GetSizer()->Add(setGround, 0, wxEXPAND);
	this->GetSizer()->Add(bottomText, 0, wxEXPAND);
	this->GetSizer()->Layout();

	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PressureAltitudeDisplay::SetGround, this, ID_SET_GROUND);
	groundFeet = 0;
}

void PressureAltitudeDisplay::SetPressure(float newPressure) {
	pressureBar->SetValue(newPressure);

	// Convert pressure to feet
	float power = pow((newPressure / 1013.25f), 0.190284);
	float feet = (1.0f - power) * 145366.45f;

	// subtract ground altitude
	feet -= groundFeet;
	// Update altitude bar
	altitudeBar->SetValue(feet);
	lastFeet = feet;

	// set bottom text
	wxString pressureString = wxNumberFormatter::ToString(pressureBar->GetLeft(), 2);
	wxString altitudeString = wxNumberFormatter::ToString(altitudeBar->GetLeft(), 2);
	bottomText->SetValue(pressureString + pressureBar->GetLeftUnits() + "  ,  " + altitudeString + altitudeBar->GetLeftUnits());
}

void PressureAltitudeDisplay::SetTitle(wxString newTitle) {
	titleText->SetValue(newTitle);
}

void PressureAltitudeDisplay::SetGround(wxCommandEvent& WXUNUSED(event)) {

	wxString pressureString = wxNumberFormatter::ToString(pressureBar->GetLeft(), 2);
	wxString altitudeString = wxNumberFormatter::ToString(altitudeBar->GetLeft(), 2);

	wxString warningString = "You are about to set ground altitude at pressure of ";
	warningString += pressureString + pressureBar->GetLeftUnits();
	warningString += " , and an altitude of ";
	warningString += altitudeString + altitudeBar->GetLeftUnits();
	warningString += ".  Are you sure you wish to continue?";

	wxMessageDialog * warning = new wxMessageDialog(this, warningString, "Set Ground Altitude?", wxYES_NO | wxCANCEL | wxICON_WARNING);
	if (warning->ShowModal() == wxID_YES) {
		groundFeet = lastFeet;
	}
}

PressureAltitudeWindow::PressureAltitudeWindow(wxWindow * parent, wxString name, float minPressure, float maxPressure, int numPressureMarks, float minAltitude, float maxAltitude, int numAltitudeMarks) : wxFrame(parent, -1, name){
	inside = new PressureAltitudeDisplay(this, name, minPressure, maxPressure, numPressureMarks, minAltitude, maxAltitude, numAltitudeMarks);
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&PressureAltitudeWindow::OnClose, this);
}

void PressureAltitudeWindow::SetPressure(float newPressure) {
	inside->SetPressure(newPressure);
}

void PressureAltitudeWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}