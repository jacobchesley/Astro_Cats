/**
*	@author Jacob Chesley
*	@date 11/28/2015
*/

#ifndef ML8511_H
#define ML8511_H
#include <arduino.h>

class ML8511{
public:

	ML8511(int outPin, int referencePin);
	
	double GetUV();	

private:

	int out;
	int reference;
};

#endif