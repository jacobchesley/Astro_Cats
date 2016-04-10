#ifndef TRACKING_H
#define TRACKING_H

#include <../ArduinoJson/src/ArduinoJson.h>
#include <../RadioAPI/RadioAPI.h>
#include <../RadioINIT/RadioINIT.h>
#include <../GPS/Venus638.h>

class Tracking{
public:
	Tracking();
	void Loop();

private:
	void ParseMessage();
	void ExecuteLastCommand();
	void SendMessageConfirmation();

	void SendTrackingData();

	RadioAPI * radio;
	RadioINIT * radioAddresses;
	Venus638 * gps;
	GPSData gpsData;

	String incomingMessage;
	String lastCommand;
	String lastCommandValue;

	long microsecondCount;
	long microsecondUpdate;
};

#endif