#include "CommandWindow.h"

CommandDisplay::CommandDisplay(wxWindow * parent, SerialController * controller, wxString commandDestination, wxString commandNameTitle, wxString commandNameJSON, wxVector<wxString> parametersListDisplay, wxVector<wxString> parametersListJSON, int defaultParam) : wxPanel(parent) {

	this->SetBackgroundColour(wxColor(0, 0, 0));

	control = controller;
	titleName = commandNameTitle;
	destination = commandDestination;
	jsonCommand = commandNameJSON;
	jsonParams = parametersListJSON;

	// Create and style command name text
	commandNameDisplay = new wxTextCtrl(this, -1, commandNameTitle);
	commandNameDisplay->SetBackgroundColour(wxColor(0, 0, 0));
	commandNameDisplay->SetForegroundColour(wxColor(255, 255, 255));
	commandNameDisplay->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	commandNameDisplay->SetEditable(false);

	parametersShown = false;

	// Create and style parameter combo box, if parameters exist
	if (parametersListDisplay.size() > 0) {
		parametersShown = true;
		parametersBox = new wxComboBox(this, -1);
		parametersBox->SetBackgroundColour(wxColor(0, 0, 0));
		parametersBox->SetForegroundColour(wxColor(255, 255, 255));
		parametersBox->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

		// Append parameters to combo box, and set default selection
		for (int i = 0; i <(int) parametersListDisplay.size(); i++) {
			parametersBox->AppendString(parametersListDisplay[i]);
		}
		parametersBox->SetSelection(defaultParam);
	}

	fireCommand = new wxButton(this, FIRE_COMMAND, "Send Command");
	fireCommand->SetBackgroundColour(wxColor(0, 255, 0));
	fireCommand->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(commandNameDisplay, 0, wxEXPAND);

	if (parametersShown) {
		this->GetSizer()->Add(parametersBox, 0, wxEXPAND);
	}

	this->GetSizer()->AddStretchSpacer();
	this->GetSizer()->Add(fireCommand);

	this->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&CommandDisplay::OnFireCommand, this, FIRE_COMMAND);
}

wxString CommandDisplay::GetCommandNameTitle() {
	return titleName;
}

wxString CommandDisplay::BuildJSONCommandString(bool indent) {
	nlohmann::json jsonObject;
	jsonObject["Source"] = "GroundSoftware";
	jsonObject["Destination"] = destination.mb_str();
	jsonObject["MessageType"] = "Command";
	jsonObject["Command"] = jsonCommand.mb_str();
	if (parametersShown) {
		jsonObject["Value"] = jsonParams[parametersBox->GetSelection()].mb_str();
	}
	else {
		jsonObject["Value"] = "NO_VALUE";
	}

	// Dump json data to string, with tab width of 5.
	if (indent) {
		return wxString(jsonObject.dump(5));
	}
	else {
		return wxString(jsonObject.dump());
	}
}

void CommandDisplay::OnFireCommand(wxCommandEvent& WXUNUSED(event)) {
	wxString jsonString = this->BuildJSONCommandString(true);

	wxString warningString = "Are you sure you want to send this command to " + destination + "? \n \n";
	warningString += "JSON Data: \n";
	warningString += jsonString;

	wxMessageDialog * warning = new wxMessageDialog(this, warningString, "Send Command?", wxYES_NO | wxICON_WARNING);
	if (warning->ShowModal() == wxID_YES) {
		wxString jsonStringNoIndent = this->BuildJSONCommandString(false);
		control->WriteData(jsonStringNoIndent);
	}

}

PILCommandDisplay::PILCommandDisplay(wxWindow * parent, SerialController * controller) : wxPanel(parent) {
	notebook = new wxNotebook(this, -1);

	// Create parameters for Radio Power Levls
	wxVector<wxString> radioPowerList;
	radioPowerList.push_back("1 mW");
	radioPowerList.push_back("10 mW");
	radioPowerList.push_back("100 mW");
	radioPowerList.push_back("500 mW");
	radioPowerList.push_back("1 W");
	// Create JSON Paramters
	wxVector<wxString> radioPowerJSON;
	radioPowerJSON.push_back("0");
	radioPowerJSON.push_back("1");
	radioPowerJSON.push_back("2");
	radioPowerJSON.push_back("3");
	radioPowerJSON.push_back("4");

	// Create command window for Radio Power Level
	setRadioPowerCommand = new CommandDisplay(notebook, controller, "PIL", "Set Radio Power", "RadioPower", radioPowerList, radioPowerJSON, 4);

	// Create parameters for Rocket GPS Update Rate
	wxVector<wxString> updateRateList;
	updateRateList.push_back("5,000,000 uS");
	updateRateList.push_back("4,000,000 uS");
	updateRateList.push_back("3,000,000 uS");
	updateRateList.push_back("2,000,000 uS");
	updateRateList.push_back("1,000,000 uS");
	updateRateList.push_back("750,000 uS");
	updateRateList.push_back("500,000 uS");
	updateRateList.push_back("250,000 uS");
	updateRateList.push_back("100,000 uS");

	// Create JSON Parameters
	wxVector<wxString> updateRateJSON;
	updateRateJSON.push_back("5000000");
	updateRateJSON.push_back("4000000");
	updateRateJSON.push_back("3000000");
	updateRateJSON.push_back("2000000");
	updateRateJSON.push_back("1000000");
	updateRateJSON.push_back("750000");
	updateRateJSON.push_back("500000");
	updateRateJSON.push_back("250000");
	updateRateJSON.push_back("100000");

	// Create command window for Update Rate
	setUpdateRateCommand = new CommandDisplay(notebook, controller, "PIL", "Set Update Interval", "UpdateInterval", updateRateList, updateRateJSON, 5);

	wxVector<wxString> emptyList;

	// Create command windows for capture image, parachute release, and parachute lock
	takePhotoCommand = new CommandDisplay(notebook, controller, "PIL", "Capture Image", "CapureImage", emptyList, emptyList);
	lockParachuteCommand = new CommandDisplay(notebook, controller, "PIL", "Lock Parachute", "LockParachute", emptyList, emptyList);
	releaseParachuteCommand = new CommandDisplay(notebook, controller, "PIL", "Release Parachute", "ReleaseParachute", emptyList, emptyList);
	
	// Add the pages
	notebook->InsertPage(0, setRadioPowerCommand, setRadioPowerCommand->GetCommandNameTitle());
	notebook->InsertPage(1, setUpdateRateCommand, setUpdateRateCommand->GetCommandNameTitle());
	notebook->InsertPage(2, takePhotoCommand, takePhotoCommand->GetCommandNameTitle());
	notebook->InsertPage(3, lockParachuteCommand, lockParachuteCommand->GetCommandNameTitle());
	notebook->InsertPage(4, releaseParachuteCommand, releaseParachuteCommand->GetCommandNameTitle());

	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(notebook, 1, wxEXPAND);
}

RocketCommandDisplay::RocketCommandDisplay(wxWindow * parent, SerialController * controller) : wxPanel(parent) {

	notebook = new wxNotebook(this, -1);

	// Create parameters for Radio Power Levls
	wxVector<wxString> radioPowerList;
	radioPowerList.push_back("1 mW");
	radioPowerList.push_back("10 mW");
	radioPowerList.push_back("100 mW");
	radioPowerList.push_back("500 mW");
	radioPowerList.push_back("1 W");
	// Create JSON Paramters
	wxVector<wxString> radioPowerJSON;
	radioPowerJSON.push_back("0");
	radioPowerJSON.push_back("1");
	radioPowerJSON.push_back("2");
	radioPowerJSON.push_back("3");
	radioPowerJSON.push_back("4");

	// Create command window for Radio Power Level
	setRadioPowerCommand = new CommandDisplay(notebook, controller, "Rocket", "Set Radio Power", "RadioPower", radioPowerList, radioPowerJSON, 4);

	// Create parameters for Rocket GPS Update Rate
	wxVector<wxString> updateRateList;
	updateRateList.push_back("5,000,000 uS");
	updateRateList.push_back("4,000,000 uS");
	updateRateList.push_back("3,000,000 uS");
	updateRateList.push_back("2,000,000 uS");
	updateRateList.push_back("1,000,000 uS");
	updateRateList.push_back("750,000 uS");
	updateRateList.push_back("500,000 uS");
	updateRateList.push_back("250,000 uS");
	updateRateList.push_back("100,000 uS");
	// Create JSON Parameters
	wxVector<wxString> updateRateJSON;
	updateRateJSON.push_back("5000000");
	updateRateJSON.push_back("4000000");
	updateRateJSON.push_back("3000000");
	updateRateJSON.push_back("2000000");
	updateRateJSON.push_back("1000000");
	updateRateJSON.push_back("750000");
	updateRateJSON.push_back("500000");
	updateRateJSON.push_back("250000");
	updateRateJSON.push_back("100000");

	// Create command window for Update Rate
	setUpdateRateCommand = new CommandDisplay(notebook, controller, "Rocket", "Set Update Interval", "UpdateInterval", updateRateList, updateRateJSON, 5);

	// Add the pages
	notebook->InsertPage(0, setRadioPowerCommand, setRadioPowerCommand->GetCommandNameTitle(), true);
	notebook->InsertPage(1, setUpdateRateCommand, setUpdateRateCommand->GetCommandNameTitle());

	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(notebook, 1, wxEXPAND);
}

MainCommandWindow::MainCommandWindow(wxWindow * parent, wxString name, SerialController * controller) : wxFrame(parent, -1, name) {

	// Create the notebook
	notebook = new wxNotebook(this, -1);

	// Create the rocket and PIL commands windows, each window will have it's own page in the notebook
	rocketCommands = new RocketCommandDisplay(notebook, controller);
	pilCommands = new PILCommandDisplay(notebook, controller);

	// Add the pages
	notebook->InsertPage(0, rocketCommands, "Rocket Commands", true);
	notebook->InsertPage(1, pilCommands, "PIL Commands");

	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);
	
	this->GetSizer()->Add(notebook, 1, wxEXPAND);


	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&MainCommandWindow::OnClose, this);
}

void MainCommandWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}

CommandResponseWindow::CommandResponseWindow(wxWindow * parent, wxString name) : wxFrame(parent, -1, name) {

	// Create the notebook
	notebook = new wxNotebook(this, -1);
	
	// Create the rocket and PIL successful commands windows, each will have their own page in the notebook
	rocketCommands = new wxListCtrl(notebook, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	pilCommands = new wxListCtrl(notebook, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

	rocketCommands->SetBackgroundColour(wxColor(0, 0, 0));
	rocketCommands->SetForegroundColour(wxColor(255, 255, 255));
	rocketCommands->InsertColumn(0, "Command Name");
	rocketCommands->InsertColumn(1, "Command Value");

	pilCommands->SetBackgroundColour(wxColor(0, 0, 0));
	pilCommands->SetForegroundColour(wxColor(255, 255, 255));
	pilCommands->InsertColumn(0, "Command Name");
	pilCommands->InsertColumn(1, "Command Value");

	// Add the pages
	notebook->InsertPage(0, rocketCommands, "Successful Rocket Commands", true);
	notebook->InsertPage(1, pilCommands, "Sucessful PIL Commands");
	
	// Create and add layout to window
	layout = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(layout);

	this->GetSizer()->Add(notebook, 1, wxEXPAND);

	this->SetIcon(wxIcon("IDI_ICON1"));
	this->Bind(wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&CommandResponseWindow::OnClose, this);
}

void CommandResponseWindow::OnClose(wxCloseEvent& evt) {
	this->Hide();
	evt.Veto();
}

void CommandResponseWindow::RecieveResponsePIL(wxString commandName, wxString commandValue) {

	OutputDebugStringA("Got PIL Success!");
	long index = pilCommands->InsertItem(1000000, commandName);
	pilCommands->SetItem(index, 1, commandValue);
}

void CommandResponseWindow::RecieveResponseRocket(wxString commandName, wxString commandValue) {

	long index = rocketCommands->InsertItem(1000000, commandName);
	rocketCommands->SetItem(index, 1, commandValue);
}