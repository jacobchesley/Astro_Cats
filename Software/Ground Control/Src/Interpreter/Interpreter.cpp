#include "Interpreter.h"

Interpreter::Interpreter(){}

void Interpreter::PushData(wxString data) {
	//switch (this->CheckForFlag(data)) {

	//}
}

void Interpreter::WriteJpegFile(wxString fileName) {

	wxFile jpegFile;
	jpegFile.Create(fileName, true);

	
	jpegFile.Write(&hexData[0], hexData.size());
	
	jpegFile.Close();
}

void Interpreter::AppendHex(char * inputHex, int length) {

	for (int i = 0; i < length; i++) {
		hexData.push_back(inputHex[i]);
	}
}

wxString Interpreter::CheckForFlag(wxString inString) {
	return "";
}