#include "AboutWindow.h"

AboutWindow::AboutWindow(wxWindow * parent) : wxFrame(parent, -1, "About Astro Cats Ground Control") {

	// Update Version HERE!
	versionText = "Version: 0.2.0";

	this->SetBackgroundColour(wxColor(45, 45, 45));
	title = new wxStaticText(this, -1, "Astro Cats Ground Control");
	title->SetBackgroundColour(wxColor(45, 45, 45));
	title->SetForegroundColour(wxColor(255, 255, 255));
	title->SetFont(wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	description = new wxStaticText(this, -1, "Ground control software for UC \n Astro Cats NASA USLI 2016");
	description->SetBackgroundColour(wxColor(45, 45, 45));
	description->SetForegroundColour(wxColor(220, 220, 220));
	description->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	ucRocketry = new wxHyperlinkCtrl(this, -1, "UC Astro Cats Website", "www.ucrocketry.com");
	ucRocketry->SetNormalColour(wxColor(220, 15, 15));
	ucRocketry->SetVisitedColour(wxColor(220, 15, 15));
	ucRocketry->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	author = new wxStaticText(this, -1, "Created By: Jacob Chesley");
	author->SetBackgroundColour(wxColor(45, 45, 45));
	author->SetForegroundColour(wxColor(220, 220, 220));
	author->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	wxString date = "Build Date: ";
	date += __DATE__;
	buildDate = new wxStaticText(this, -1, date);
	buildDate->SetBackgroundColour(wxColor(45, 45, 45));
	buildDate->SetForegroundColour(wxColor(220, 220, 220));
	buildDate->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	buildNumber = new wxStaticText(this, -1, versionText);
	buildNumber->SetBackgroundColour(wxColor(45, 45, 45));
	buildNumber->SetForegroundColour(wxColor(220, 220, 220));
	buildNumber->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);
	this->GetSizer()->Add(title);
	this->GetSizer()->AddSpacer(5);
	this->GetSizer()->Add(description);
	this->GetSizer()->AddSpacer(5);
	this->GetSizer()->Add(ucRocketry);
	this->GetSizer()->AddSpacer(25);
	this->GetSizer()->Add(author);
	this->GetSizer()->AddSpacer(5);
	this->GetSizer()->Add(buildDate);
	this->GetSizer()->AddSpacer(5);
	this->GetSizer()->Add(buildNumber);

	
	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&AboutWindow::OnClose, this);
	this->Fit();
}

wxString AboutWindow::GetVersion() {
	return versionText;
}

void AboutWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}
