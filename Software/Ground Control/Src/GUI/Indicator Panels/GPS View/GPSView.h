#ifndef GPS_VIEW_H
#define GPS_VIEW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/splitter.h"
#include "wx/dcbuffer.h"

struct GPSCoord {
	float Lat;
	float Lon;
	wxString NS;
	wxString EW;
};

class GPSRadarPanel;

class GPSInfoPanel : public wxPanel {

public:
	GPSInfoPanel(wxWindow * parent, GPSRadarPanel * radarPanel);
	void SetTitleText(wxString title);
	void UpdateGPSPos(GPSCoord pos);
	void UpdateAltitude(float altitude);
	void UpdateQuality(int quality);
	void UpdateNumSat(int numSat);
	void UpdateSatList(wxString satList);
	void UpdatePDOP(float hdop);
	void UpdateHDOP(float hdop);
	void UpdateVDOP(float hdop);
	void UpdateTime(wxString time);

private:

	void SetBase(wxCommandEvent& WXUNUSED(event));
	GPSCoord currentCoord;

	GPSRadarPanel * radar;
	wxGridSizer * layout;

	wxStaticText * titleLabel;
	wxTextCtrl * titleText;

	wxStaticText * lonLatLabel;
	wxTextCtrl * lonLatText;

	wxStaticText * altitudeLabel;
	wxTextCtrl * altitudeText;

	wxStaticText * qualityLabel;
	wxTextCtrl * qualityText;

	wxStaticText * numSatLabel;
	wxTextCtrl * numSatText;

	wxStaticText * pdopLabel;
	wxTextCtrl * pdopText;

	wxStaticText * hdopLabel;
	wxTextCtrl * hdopText;

	wxStaticText * vdopLabel;
	wxTextCtrl * vdopText;

	wxStaticText * satListLabel;
	wxTextCtrl * satListText;

	wxStaticText * timeLabel;
	wxTextCtrl * timeText;

	wxButton * setBaseButton;

	enum {
		ID_SET_BASE
	};
};

class GPSRadarPanel : public wxPanel {
public:
	GPSRadarPanel(wxWindow * parent);
	void SetBaseCoord(GPSCoord coord);
	void SetMobileCoord(GPSCoord coord);
	void PaintNow();

private:
	void Render(wxDC& dc);
	void OnPaint(wxPaintEvent& paintEvent);
	void OnSize(wxSizeEvent& sizeEvent);

	const double pi = 3.1415926535897932384626433832795028841971693993751;
	float rad;
	float deg;
	float CalculateDistance(GPSCoord coord1, GPSCoord coord2);
	float CalculateAngle(GPSCoord coord1, GPSCoord coord2);
	GPSCoord baseCoord;
	GPSCoord mobileCoord;

	int units = 0;
	enum Units {
		MILES,
		FEET,
		KILOMETERS,
		METERS	
	};
};

class GPSView : public wxPanel {
public:
	GPSView(wxWindow * parent, wxString title);
	void UpdateGPSPos(GPSCoord pos);
	void UpdateAltitude(float altitude);
	void UpdateQuality(int quality);
	void UpdateNumSat(int numSat);
	void UpdateSatList(wxString satList);
	void UpdatePDOP(float hdop);
	void UpdateHDOP(float hdop);
	void UpdateVDOP(float hdop);
	void UpdateTime(wxString time);

private:
	wxBoxSizer * layout;
	wxSplitterWindow * splitter;
	GPSInfoPanel * info;
	GPSRadarPanel * radar;
};

class GPSViewWindow : public wxFrame {
public:
	GPSViewWindow(wxWindow * parent, wxString title);
	void UpdateGPSPos(GPSCoord pos);
	void UpdateAltitude(float altitude);
	void UpdateQuality(int quality);
	void UpdateNumSat(int numSat);
	void UpdateSatList(wxString satList);
	void UpdatePDOP(float hdop);
	void UpdateHDOP(float hdop);
	void UpdateVDOP(float hdop);
	void UpdateTime(wxString time);

private:
	void OnClose(wxCloseEvent& evt);

	wxBoxSizer * layout;
	GPSView * view;
};
#endif