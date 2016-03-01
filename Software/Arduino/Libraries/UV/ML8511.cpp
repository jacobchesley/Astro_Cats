#include "ML8511.h"

ML8511::ML8511(int outPin, int referncePin){

	out = outPin;
	reference = referncePin;
}

double ML8511::GetUV(){

	// Read voltages from arduino
	int uvValue = analogRead(out);
	int refValue = analogRead(reference);
	double actualVoltage = 3.30 / refValue * uvValue;

	// Values from datasheet, graph of UV intensity with respect to voltage
	double minVoltage = 0.99;
	double maxVoltage = 2.85;
	double minMilliWattsCM2 = 0;
	double maxMilliWattsCM2 = 15;

	// Calculate mW/cm^2 of UV intensity
	double slope = (maxMilliWattsCM2 - minMilliWattsCM2) / (maxVoltage - minVoltage);
	double milliWattsCM2 = ((actualVoltage - minVoltage) * slope) + minMilliWattsCM2;

	return milliWattsCM2;
}