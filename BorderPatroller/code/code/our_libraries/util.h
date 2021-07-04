/*
 * util.h
 *
 * Created: 7/4/2021 10:00:55 AM
 *  Author: vdrko
 */ 
#include <math.h>


#ifndef UTIL_H_
#define UTIL_H_
/**
Here goes our own functions
*/

#define SN1_TRGR_1 PORTB1

/**
Sets i th bit in number n
*/
int setBit(int n, int pos){
	return n |(1 << pos);
}

/**
Unsets i th bit in number n
*/
int unsetBit(int n, int pos){
	return n & ~(1 << pos);
}

class DistanceCalculator
{
	/**
	times are in  micro second
	distances are in mili meter
	*/
	double temperature;
	double velocity;
	uint32_t maximumAllowedDistance;
	uint32_t maximumAllowedTime; 
	public:
	DistanceCalculator(float temp)
	{
		maximumAllowedDistance = 4000 + 500; // .5m extra for extra :p
		temperature=temp;
		setTemperature(temp);

	}


	double calculateVelocity()
	{

		return velocity = 331.3 * sqrt(1 + temperature/273.15) / 1000.0;

	}

	int calculateDistance(int time_us)
	{
		

		return (int)(getVelocity()*time_us) >> 1;

	}
	int calculateMaximumWaitTime(){
		return  maximumAllowedTime =  2 * maximumAllowedDistance / getVelocity() ;
	}	
	double getVelocity(){
		return velocity;
	}
	
	int getMaximumWaitTime(){
		return maximumAllowedTime;
	}
	
	void setTemperature(float temp){
		this->temperature = temp;
		this->velocity = this->calculateVelocity();
		this->maximumAllowedTime = calculateMaximumWaitTime();
		
	}
	
	

};

extern  char temp_string[16];
char *itoa(uint32_t n){
	//char s[10];
	temp_string[0] = 0;
	itoa(n, temp_string, 10);
	return temp_string;
}


#endif /* UTIL_H_ */