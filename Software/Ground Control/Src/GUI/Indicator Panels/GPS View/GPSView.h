#ifndef GPS_VIEW_H
#define GPS_VIEW_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

struct GPSCoord {
	float Lat;
	float Lon;
	wxString NS;
	wxString EW;
};

class GPSInfoPanel : public wxPanel {

public:
	GPSInfoPanel(wxWindow * parent);
	void UpdateGPSPos(GPSCoord pos);
	void UpdateAltitude(float altitude);
	void UpdateQuality(int quality);
	void UpdateNumSat(int numSat);
	void UpdateSatList(wxString satList);
	void UpdatePDOP(float hdop);
	void UpdateHDOP(float hdop);
	void UpdateVDOP(float hdop);

private:
	wxGridSizer * layout;

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
};

class GPSView : public wxPanel {
public:
	GPSView(wxWindow * parent);
	void UpdateGPSPos(GPSCoord pos);
	void UpdateAltitude(float altitude);
	void UpdateQuality(int quality);
	void UpdateNumSat(int numSat);
	void UpdateSatList(wxString satList);
	void UpdatePDOP(float hdop);
	void UpdateHDOP(float hdop);
	void UpdateVDOP(float hdop);

private:
	wxBoxSizer * layout;
	GPSInfoPanel * info;
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

private:
	void OnClose(wxCloseEvent& evt);

	wxBoxSizer * layout;
	GPSView * view;
};
#endif