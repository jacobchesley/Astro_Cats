#ifndef GROUND_CONTROL_H
#define GROUND_CONTROL_H

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "GUI\MainWindow\MainWindow.h"

class GroundControl : public wxApp
{
public:
	/** 
		Initialization of the application.  This is where the Main User Interface will be created
	*/
	virtual bool OnInit();
};

#endif