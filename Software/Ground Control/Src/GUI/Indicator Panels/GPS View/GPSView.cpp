#include "GPSView.h"


GPSInfoPanel::GPSInfoPanel(wxWindow * parent) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	layout = new wxGridSizer(2, 0, 0);
	this->SetSizer(layout);

	lonLatLabel = new wxStaticText(this, -1, "Lat , Lon ");
	lonLatLabel->SetBackgroundColour(wxColor(0, 0, 0));
	lonLatLabel->SetForegroundColour(wxColor(200, 200, 200));
	lonLatLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	lonLatText = new wxTextCtrl(this, -1, "Test");
	lonLatText->SetBackgroundColour(wxColor(0, 0, 0));
	lonLatText->SetForegroundColour(wxColor(255, 255, 255));
	lonLatText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(lonLatLabel);
	this->GetSizer()->Add(lonLatText, 0, wxEXPAND);

	altitudeLabel = new wxStaticText(this, -1, "Altitude ");
	altitudeLabel->SetBackgroundColour(wxColor(0, 0, 0));
	altitudeLabel->SetForegroundColour(wxColor(200, 200, 200));
	altitudeLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	altitudeText = new wxTextCtrl(this, -1, "Test");
	altitudeText->SetBackgroundColour(wxColor(0, 0, 0));
	altitudeText->SetForegroundColour(wxColor(255, 255, 255));
	altitudeText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(altitudeLabel);
	this->GetSizer()->Add(altitudeText, 0, wxEXPAND);

	qualityLabel = new wxStaticText(this, -1, "Quality ");
	qualityLabel->SetBackgroundColour(wxColor(0, 0, 0));
	qualityLabel->SetForegroundColour(wxColor(200, 200, 200));
	qualityLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	qualityText = new wxTextCtrl(this, -1, "Test");
	qualityText->SetBackgroundColour(wxColor(0, 0, 0));
	qualityText->SetForegroundColour(wxColor(255, 255, 255));
	qualityText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(qualityLabel);
	this->GetSizer()->Add(qualityText, 0, wxEXPAND);

	numSatLabel = new wxStaticText(this, -1, "Num Sat ");
	numSatLabel->SetBackgroundColour(wxColor(0, 0, 0));
	numSatLabel->SetForegroundColour(wxColor(200, 200, 200));
	numSatLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	numSatText = new wxTextCtrl(this, -1, "Test");
	numSatText->SetBackgroundColour(wxColor(0, 0, 0));
	numSatText->SetForegroundColour(wxColor(255, 255, 255));
	numSatText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(numSatLabel);
	this->GetSizer()->Add(numSatText, 0, wxEXPAND);

	satListLabel = new wxStaticText(this, -1, "Sat List ");
	satListLabel->SetBackgroundColour(wxColor(0, 0, 0));
	satListLabel->SetForegroundColour(wxColor(200, 200, 200));
	satListLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	satListText = new wxTextCtrl(this, -1, "Test");
	satListText->SetBackgroundColour(wxColor(0, 0, 0));
	satListText->SetForegroundColour(wxColor(255, 255, 255));
	satListText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(satListLabel);
	this->GetSizer()->Add(satListText, 0, wxEXPAND);

	pdopLabel = new wxStaticText(this, -1, "PDOP ");
	pdopLabel->SetBackgroundColour(wxColor(0, 0, 0));
	pdopLabel->SetForegroundColour(wxColor(200, 200, 200));
	pdopLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	pdopText = new wxTextCtrl(this, -1, "Test");
	pdopText->SetBackgroundColour(wxColor(0, 0, 0));
	pdopText->SetForegroundColour(wxColor(255, 255, 255));
	pdopText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(pdopLabel);
	this->GetSizer()->Add(pdopText, 0, wxEXPAND);

	hdopLabel = new wxStaticText(this, -1, "HDOP ");
	hdopLabel->SetBackgroundColour(wxColor(0, 0, 0));
	hdopLabel->SetForegroundColour(wxColor(200, 200, 200));
	hdopLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	hdopText = new wxTextCtrl(this, -1, "Test");
	hdopText->SetBackgroundColour(wxColor(0, 0, 0));
	hdopText->SetForegroundColour(wxColor(255, 255, 255));
	hdopText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(hdopLabel);
	this->GetSizer()->Add(hdopText, 0, wxEXPAND);

	vdopLabel = new wxStaticText(this, -1, "VDOP ");
	vdopLabel->SetBackgroundColour(wxColor(0, 0, 0));
	vdopLabel->SetForegroundColour(wxColor(200, 200, 200));
	vdopLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	vdopText = new wxTextCtrl(this, -1, "Test");
	vdopText->SetBackgroundColour(wxColor(0, 0, 0));
	vdopText->SetForegroundColour(wxColor(255, 255, 255));
	vdopText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	this->GetSizer()->Add(vdopLabel);
	this->GetSizer()->Add(vdopText, 0, wxEXPAND);

}

void GPSInfoPanel::UpdateGPSPos(GPSCoord pos) {
	lonLatText->SetValue(std::to_string(pos.Lat) + pos.NS + " , " + std::to_string(pos.Lon) + pos.EW);
}

void GPSInfoPanel::UpdateAltitude(float altitude) {
	altitudeText->SetValue(std::to_string(altitude));
}

void GPSInfoPanel::UpdateQuality(int quality) {

	switch (quality) {
		case 0:
			qualityText->SetValue("Position fix unavailable.");
			break;
		case 1:
			qualityText->SetValue("Valid position fix, SPS mode.");
			break;
		case 2:
			qualityText->SetValue("Valid position fix, differential GPS mode.");
			break;
		case 3:
			qualityText->SetValue("GPS PPS mode, fix valid.");
			break;
		case 4:
			qualityText->SetValue("Real Time Kinematic.  Fixed Integers.");
			break;
		case 5:
			qualityText->SetValue("Real Time Kinematic.  Floating Integers.");
			break;
		case 6:
			qualityText->SetValue("Estimated (dead reckoning) mode.");
			break;
		case 7:
			qualityText->SetValue("Manual input mode.");
			break;
		case 8:
			qualityText->SetValue("Simulator mode.");
			break;
	}
}

void GPSInfoPanel::UpdateNumSat(int numSat) {
	numSatText->SetValue(std::to_string(numSat));
}

void GPSInfoPanel::UpdateSatList(wxString satList) {
	satListText->SetValue(satList);
}

void GPSInfoPanel::UpdatePDOP(float pdop) {
	pdopText->SetValue(std::to_string(pdop));
}

void GPSInfoPanel::UpdateHDOP(float hdop) {
	hdopText->SetValue(std::to_string(hdop));
}

void GPSInfoPanel::UpdateVDOP(float vdop) {
	vdopText->SetValue(std::to_string(vdop));
}

GPSView::GPSView(wxWindow * parent) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	info = new GPSInfoPanel(this);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);
	this->GetSizer()->Add(info, 0, wxEXPAND);
}

void GPSView::UpdateGPSPos(GPSCoord pos) {
	info->UpdateGPSPos(pos);
}

void GPSView::UpdateAltitude(float altitude) {
	info->UpdateAltitude(altitude);
}

void GPSView::UpdateNumSat(int numSat) {
	info->UpdateNumSat(numSat);
}

void GPSView::UpdateSatList(wxString satList) {
	info->UpdateSatList(satList);
}

void GPSView::UpdateQuality(int quality) {
	info->UpdateQuality(quality);
}

void GPSView::UpdatePDOP(float pdop) {
	info->UpdatePDOP(pdop);
}

void GPSView::UpdateHDOP(float hdop) {
	info->UpdateHDOP(hdop);
}

void GPSView::UpdateVDOP(float vdop) {
	info->UpdateVDOP(vdop);
}

GPSViewWindow::GPSViewWindow(wxWindow * parent, wxString title) : wxFrame(parent, -1, title) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	view = new GPSView(this);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);
	this->GetSizer()->Add(view, 1, wxEXPAND);

	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&GPSViewWindow::OnClose, this);
}

void GPSViewWindow::UpdateGPSPos(GPSCoord pos) {
	view->UpdateGPSPos(pos);
}

void GPSViewWindow::UpdateAltitude(float altitude) {
	view->UpdateAltitude(altitude);
}

void GPSViewWindow::UpdateNumSat(int numSat) {
	view->UpdateNumSat(numSat);
}

void GPSViewWindow::UpdateSatList(wxString satList) {
	view->UpdateSatList(satList);
}

void GPSViewWindow::UpdateQuality(int quality) {
	view->UpdateQuality(quality);
}

void GPSViewWindow::UpdatePDOP(float pdop) {
	view->UpdatePDOP(pdop);
}

void GPSViewWindow::UpdateHDOP(float hdop) {
	view->UpdateHDOP(hdop);
}

void GPSViewWindow::UpdateVDOP(float vdop) {
	view->UpdateVDOP(vdop);
}

void GPSViewWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}