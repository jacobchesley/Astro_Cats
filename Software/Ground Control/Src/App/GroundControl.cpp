#include "GroundControl.h"

IMPLEMENT_APP(GroundControl)

// 'Main program' equivalent: the program execution "starts" here
bool GroundControl::OnInit(){

    if (!wxApp::OnInit()){
		return false;
	}
   
    // Create and show the main window
	MainWindow * mainWindow = new MainWindow();
	mainWindow->Show();
    return true;
}
