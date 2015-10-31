#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/splitter.h"

#include "Serial\SerialController.h"
#include "GUI\Indicator Panels\Radio Signal Strength\RadioSignalStrength.h"
#include "GUI\Indicator Panels\Incoming Data Stream\IncomingDataStream.h"
#include "GUI\Serial Port Connection\SerialPortConnection.h"

class UIUpdateThread;

/**
	Main Window is the main window of the Ground Control.
*/
class MainWindow : public wxFrame {

	public:
		/**
			Constructor for the main window.
		*/
		MainWindow();
		void UpdateData(int dataParameter, int dataValue);
		void ReciveSerialData(char * serialData, int length);

		enum DataParam {
			DATA_RADIO_SIGNAL_STRENGTH_FROM_PIL,
			DATA_RADIO_SIGNAL_STRENGTH_FROM_TRACKING
		};

	private:

		void ShowSerialConnection(wxCommandEvent& WXUNUSED(event));

		SerialController * serialController;
		UIUpdateThread * uiUpdater;

		// Radio Signal Strength Panels
		wxSplitterWindow * radioSplitter;
		RadioSignalStrength * radioSignalStrengthPil;
		RadioSignalStrength * radioSignalStrengthRocket;

		wxSplitterWindow * radioSignalAndDataSplitter;
		IncomingDataStream * dataWindow;

		// Serial Port Connection window
		SerialPortConnection * serialPortConnection;


		// Layout for this main window
		wxBoxSizer * mainLayout;
		enum MenuBar{
			ID_CONNECT_SERIAL,
			ID_SEND_PIL_COMMAND,
			ID_READ_PIL_STATUS,
			ID_DOC,
			ID_ABOUT
		};
};


// TESTING THREAD
class UIUpdateThread : wxThread {

public:
	UIUpdateThread(MainWindow * window);
protected:
	virtual ExitCode Entry();

private:
	MainWindow * mainWindow;

};

#endif
