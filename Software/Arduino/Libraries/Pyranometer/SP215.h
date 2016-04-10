/**
*	@author Jacob Chesley
*	@date 3/24/2016
*/

#ifndef SP215_H
#define SP215_H
#include <arduino.h>

class SP215{
public:

	SP215(int outPin);
	
	double GetSolar();	

private:

	int out;
};

#endif