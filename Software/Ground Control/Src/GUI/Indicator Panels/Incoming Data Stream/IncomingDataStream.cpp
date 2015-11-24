#include "IncomingDataStream.h"

IncomingDataStream::IncomingDataStream(wxWindow * parent, wxString title) : wxPanel(parent){

	allText = "";

	this->SetBackgroundColour(wxColor(0, 0, 0));

	titleText = new wxTextCtrl(this, -1);
	titleText->SetBackgroundColour(wxColor(0, 0, 0));
	titleText->SetForegroundColour(wxColor(255, 255, 255));
	titleText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleText->SetValue(title);
	titleText->SetEditable(false);

	dataText = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	dataText->SetBackgroundColour(wxColor(0, 0, 0));
	dataText->SetForegroundColour(wxColor(255, 255, 255));
	dataText->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	dataText->SetEditable(false);

	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(titleText, 0, wxEXPAND);
	this->GetSizer()->Add(dataText, 1, wxEXPAND);

	this->GetSizer()->Layout();
	numLines = 0;
}

void IncomingDataStream::AppendText(wxString appendedText) {
	allText += appendedText;
	dataText->SetValue(allText);
	numLines += 1;
	dataText->ScrollLines(numLines);
}

void IncomingDataStream::ClearText() {
	allText = "";
	dataText->SetValue(allText);
}