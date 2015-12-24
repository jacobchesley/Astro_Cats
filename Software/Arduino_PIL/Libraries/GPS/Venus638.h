/**
*	@author Jacob Chesley
*	@date 11/28/2015
*/

#ifndef VENUS638_H
#define VENUS638_H
#include <arduino.h>

struct GPSData{
	String Latitude;
	String Longitude;
	char NorthSouth;
	char EastWest;
	String Time;
	int Quality;
	int NumSatellites;
	float HDOP;
	float Altitude;
	int StationID;
};

class Venus638{
public:

	Venus638(HardwareSerial * serialIn);
	
	String GetLongitude();
	String GetLatitude();
	char GetNorthSouth();
	char GetEastWest();
	String GetTime();
	int GetQuality();
	int GetNumSatellites();
	float GetHDOP();
	float GetAltitude();
	int GetStationID();

	void Update();
	void FillInGPSData(GPSData * data);

	void SetBaudRate(int baud);
	void SetUpdateRate(int updateRate);

	enum {
		baud_4800,
		baud_9600,
		baud_19200,
		baud_38400,
		baud_57600,
		baud_115200
	};

	enum{
		update_1,
		update_2,
		update_4,
		update_5,
		update_8,
		update_10,
		update_20

	};
private:

	void UpdateTime();
	bool UpdateSpecificData(char * input, int len);
	bool Compare(char * comp1, char * comp2, int len);
	String GetDataAfterComma(char * data, int maxDataLen, int numCommas);
	byte ComputeChecksum(byte * payload, int len);

	char currentData[80];
	int currentDataIndex;
	char gga[80];
	char gll[80];	
	char gsa[80];
	char gsv[80];
	char rmc[80];
	char vtg[80];

	int ggaLen;
	int gllLen;	
	int gsaLen;
	int gsvLen;
	int rmcLen;
	int vtgLen;

	int hours;
	int minutes;
	int seconds;
	int milliseconds;

	HardwareSerial * gpsSerial;
};

#endif