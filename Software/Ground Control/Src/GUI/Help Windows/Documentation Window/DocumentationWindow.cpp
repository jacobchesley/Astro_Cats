#include "DocumentationWindow.h"

DocumentationWindow::DocumentationWindow(wxWindow * parent) : wxHtmlHelpController(wxHF_DEFAULT_STYLE, parent) {
	this->AddBook(wxFileName("Documentation.zip"));
	UseConfig(wxConfig::Get());
}

//void DocumentationWindow::OnClose(wxCloseEvent& evt) {
	//this->Hide();
	//evt.Veto();
//}
