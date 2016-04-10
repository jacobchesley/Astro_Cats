#include "SP215.h"

SP215::SP215(int outPin){

	out = outPin;
}

double SP215::GetSolar(){

	// Read voltages from arduino
	int solarValue = analogRead(out);
	double solarVoltageMV = 1000.0 * (solarValue * (5.0/1023.0));
	
	// Calibration Factor (0.25 W m^-2 per mV) * Sensor Output Signal (mV) = W m^-2
	return 0.25 * solarVoltageMV;
}