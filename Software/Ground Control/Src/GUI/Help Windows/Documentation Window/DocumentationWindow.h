#ifndef DOCUMENTATION_WINDOW_H
#define DOCUMENTATION_WINDOW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"
 
// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/html/helpctrl.h"

class DocumentationWindow : public wxHtmlHelpController {
public:
	DocumentationWindow(wxWindow * parent);

private:
	//void OnClose(wxCloseEvent& evt);
};

#endif