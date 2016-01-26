#include "GPSView.h"

GPSView::GPSView(wxWindow * parent, wxString title) : wxFrame(parent, -1, title) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	layout = new wxBoxSizer(wxVERTICAL);

	titleText = new wxTextCtrl(this, -1);
	titleText->SetBackgroundColour(wxColor(0, 0, 0));
	titleText->SetForegroundColour(wxColor(255, 255, 255));
	titleText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleText->SetValue(title);
	titleText->SetEditable(false);

	web = wxWebView::New(this, wxID_ANY);

	this->SetSizer(layout);
	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(web, 1, wxEXPAND);
	this->GetSizer()->Layout();

	CreateHTML();
}

void GPSView::CreateHTML() {

	wxString gpsCoord = "center:new google.maps.LatLng(51.508742,-0.120850),";

	wxString htmlTextTop = "<!DOCTYPE html><html><head><script src=\"http://maps.googleapis.com/maps/api/js\"></script><script>";
	wxString htmlInitMap = "function initialize() {var mapProp = { " + gpsCoord + " zoom:5, mapTypeId:google.maps.MapTypeId.ROADMAP}; var map = new google.maps.Map(document.getElementById(\"googleMap\"), mapProp);}";
	wxString htmlBottom =  "google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id='googleMap' style=\"width:500px;height:380px;\"></div><div>test</div></body></html>";

	htmlText = htmlTextTop + htmlInitMap + htmlBottom;
	//web->SetPage(htmlText,"./");
	web->LoadURL(wxT("file://") + wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath() + wxT("/test.html"));
}