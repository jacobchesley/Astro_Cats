#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/splitter.h"
#include "wx/sstream.h"
#include "wx/url.h"

#include "Serial\SerialController.h"
#include "GUI\Command Window\CommandWindow.h"
#include "GUI\Indicator Panels\Radio Signal Strength\RadioSignalStrength.h"
#include "GUI\Indicator Panels\Incoming Data Stream\IncomingDataStream.h"
#include "GUI\Indicator Panels\Linear Guage\LinearGuage.h"
#include "GUI\Indicator Panels\Radial Guage\RadialGuage.h"
#include "GUI\Indicator Panels\Pressure Altitude Guage\PressureAltitude.h"
#include "GUI\Indicator Panels\Pitch Roll Display\PitchRoll.h"
#include "GUI\Indicator Panels\GPS View\GPSView.h"
#include "GUI\Serial Port Connection\SerialPortConnection.h"
#include "GUI\Help Windows\Documentation Window\DocumentationWindow.h"
#include "GUI\Help Windows\About Window\AboutWindow.h"
#include "Interpreter\Interpreter.h"
#include "JSON\json.cpp"
#include "Logger\Logger.h"

class UIUpdateThread;
class PlaybackThread;
class PlaybackWindow;

/**
	Main Window is the main window of the Ground Control.
*/
class MainWindow : public wxFrame {
	
	public:
		/**
			Constructor for the main window.
		*/
		MainWindow();
		void ReciveSerialData(wxString serialData);
		void UpdateStatusBar(wxString statusMessage);
		SerialController * GetSerialController();

		enum DataParam {
			DATA_RADIO_SIGNAL_STRENGTH_FROM_PIL,
			DATA_RADIO_SIGNAL_STRENGTH_FROM_TRACKING
		};

	private:

		wxMenuBar * menuBar;
		wxMenu * menuFile;
		wxMenu * menuSerial;
		wxMenu * menuCommand;
		wxMenu * menuView;
		wxMenu * menuHelp;

		void ShowSetLogFile(wxCommandEvent& WXUNUSED(event));
		void ShowSerialConnection(wxCommandEvent& WXUNUSED(event));
		void ShowPlayback(wxCommandEvent& WXUNUSED(event));
		void ShowCommands(wxCommandEvent& WXUNUSED(event));
		void ShowCommandsResponse(wxCommandEvent& WXUNUSED(event));
		void ShowPilSignalStrength(wxCommandEvent& WXUNUSED(event));
		void ShowRocketSignalStrength(wxCommandEvent& WXUNUSED(event));
		void ShowTemperature(wxCommandEvent& WXUNUSED(event));
		void ShowHumidity(wxCommandEvent& WXUNUSED(event));
		void ShowUV(wxCommandEvent& WXUNUSED(event));
		void ShowSolar(wxCommandEvent& WXUNUSED(event));
		void ShowPressureAltitude(wxCommandEvent& WXUNUSED(event));
		void ShowPitchRoll(wxCommandEvent& WXUNUSED(event));
		void ShowGPSRocket(wxCommandEvent& WXUNUSED(event));
		void ShowGPSPIL(wxCommandEvent& WXUNUSED(event));

		void ShowAll(wxCommandEvent& WXUNUSED(event));
		void HideAll(wxCommandEvent& WXUNUSED(event));
		void RepositionAll(wxCommandEvent& WXUNUSED(event));
		
		void ShowDoc(wxCommandEvent& WXUNUSED(event));
		void ShowAbout(wxCommandEvent& WXUNUSED(event));

		void IsUpdatesAvailable();
		void OnClose(wxCloseEvent& evt);
		bool isClosed;

		SerialController * serialController;
		UIUpdateThread * uiUpdater;
		Logger * logger;
		bool isPLayingBack;

		IncomingDataStream * dataWindow;
		wxString tempJsonData;

		// Serial Port Connection window
		SerialPortConnection * serialPortConnection;
		PlaybackWindow * playbackWindow;

		// Command Window
		MainCommandWindow * mainCommandWindow;
		CommandResponseWindow * commandResponseWindow;

		// Data display windows
		RadioSignalStrengthWindow * pilRadioStrength;
		RadioSignalStrengthWindow * rocketRadioStrength;
		LinearWindow * temperatureWindow;
		LinearWindow * humidityWindow;
		RadialWindow * uvWindow;
		RadialWindow * solarWindow;
		PressureAltitudeWindow * pressureAltitudeWindow;
		PitchRollWindow * pitchRollWindow;
		GPSViewWindow * gpsViewRocket;
		GPSViewWindow * gpsViewPIL;

		// Help Menu Windows
		DocumentationWindow * docWindow;
		AboutWindow * aboutWindow;

		// Layout for this main window
		wxBoxSizer * mainLayout;

		wxPoint pilRadioPos;
		wxSize pilRadioSize;

		wxPoint rocketRadioPos;
		wxSize rocketRadioSize;

		wxPoint tempPos;
		wxSize tempSize;

		wxPoint humidPos;
		wxSize humidSize;

		wxPoint uvPos;
		wxSize uvSize;

		wxPoint solarPos;
		wxSize solarSize;

		wxPoint pressurePos;
		wxSize pressureSize;

		wxPoint pitchPos;
		wxSize pitchSize;

		enum MenuBar{
			ID_SET_LOG_FILE,
			ID_LOG,
			ID_CONNECT_SERIAL,
			ID_SHOW_PLAYBACK,
			ID_VIEW_COMMANDS,
			ID_VIEW_COMMANDS_RESPONSE,
			ID_VIEW_PILSTRENGTH,
			ID_VIEW_ROCKETSTRENGTH,
			ID_VIEW_TEMP,
			ID_VIEW_HUMID,
			ID_VIEW_UV,
			ID_VIEW_SOLAR,
			ID_VIEW_PRESSURE_ALTITUDE,
			ID_VIEW_PITCH_ROLL,
			ID_VIEW_GPS_ROCKET,
			ID_VIEW_GPS_PIL,
			ID_VIEW_ALL,
			ID_HIDE_ALL,
			ID_REPO_ALL,
			ID_DOC,
			ID_ABOUT
		};
};

// Thread to update user interface with serial data
class UIUpdateThread : wxThread {

public:
	UIUpdateThread(MainWindow * window);
	void StopThread();
protected:
	virtual ExitCode Entry();

private:
	MainWindow * mainWindow;
	bool continueWatching;
};

// Thread to update user interface with recorded file data
class PlaybackThread : wxThread {

public:
	PlaybackThread(MainWindow * window);
	void SetPlaybackFile(wxString playbackFile);
	void Start();
	void PauseResume();
	void Stop();
	void SetHZ(int hz);

protected:
	virtual ExitCode Entry();

private:
	MainWindow * mainWindow;
	wxString playback;
	Logger * reader;
	bool pause;
	bool stop;
	int sleepTime;

};

class PlaybackWindow : public wxFrame {
public:
	PlaybackWindow(wxWindow * parent, MainWindow * mainParent);
	bool IsPlaying();
	void StopThread();

private:

	MainWindow * par;
	void OnPlay(wxCommandEvent& WXUNUSED(event));
	void OnPause(wxCommandEvent& WXUNUSED(event));
	void OnStop(wxCommandEvent& WXUNUSED(event));
	void ShowFiles(wxCommandEvent& WXUNUSED(event));
	void OnClose(wxCloseEvent& evt);

	wxBoxSizer * layout;
	
	wxBoxSizer * fileLayout;
	wxStaticText * fileLabel;
	wxTextCtrl * fileName;
	wxButton * fileButton;

	wxBoxSizer * hzLayout;
	wxStaticText * hzLabel;
	wxComboBox * hzText;

	wxBoxSizer * buttonLayout;
	wxButton * playButton;
	wxButton * pauseButton;
	wxButton * stopButton;

	PlaybackThread * playThread;
	bool isPlaying;

	enum {
		ID_SHOW_FILE,
		ID_PLAY,
		ID_PAUSE,
		ID_STOP
	};
};

#endif
