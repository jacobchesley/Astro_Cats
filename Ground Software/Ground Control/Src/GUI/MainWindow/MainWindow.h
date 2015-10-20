// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

/**
	Main Window is the main window of the Ground Control.
*/
class MainWindow : public wxFrame {

	public:
		/**
			Constructor for the main window.
		*/
		MainWindow();

	private:

};

/**
	Enumumerations for main window.
*/
enum {
	ID_CONNECT_SERIAL,
	ID_SEND_PIL_COMMAND,
	ID_READ_PIL_STATUS,
	ID_DOC,
	ID_ABOUT
};