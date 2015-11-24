#ifndef HEX_JPEG
#define HEX_JPEG

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/file.h"

class Interpreter {
public:
	Interpreter();
	void PushData(wxString data);


	void WriteJpegFile(wxString fileName);
	void AppendHex(char * inputHex, int length);

private:

	enum Flags {
		START_ATMOSPHERE_DATA,
		END_ATMOSPHERE_DATA,
		START_JPEG,
		PAUSE_JPEG,
		END_JPEG
	};
	wxVector<char> hexData;

	wxString CheckForFlag(wxString );
};
#endif