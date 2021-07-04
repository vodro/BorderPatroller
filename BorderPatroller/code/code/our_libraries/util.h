/*
 * util.h
 *
 * Created: 7/4/2021 10:00:55 AM
 *  Author: vdrko
 */ 


#ifndef UTIL_H_
#define UTIL_H_
/**
Here goes our own functions
*/

#define SN1_TRGR_1 PORTA2

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
	float temperature;
	float velocity;
	float maximumAllowedDistance;
	float maximumAllowedTime; 
	public:
	DistanceCalculator(float temp)
	{
		maximumAllowedDistance = 4000 + 500; // .5m extra for extra :p
		temperature=temp;
		setTemperature(temp);

	}


	float calculateVelocity()
	{

		return velocity = 0.3313 * sqrt(1 + temperature/273.15);

	}

	float calculateDistance(float time_us)
	{
		

		return (getVelocity()*time_us)/2.0;

	}
	float calculateMaximumWaitTime(){
		return  maximumAllowedTime =  2 * maximumAllowedDistance / velocity ;
	}	
	float getVelocity(){
		return velocity;
	}
	
	float getMaximumWaitTime(){
		return maximumAllowedTime;
	}
	
	void setTemperature(float temp){
		this->temperature = temp;
		this->velocity = this->calculateVelocity();
		this->maximumAllowedTime = calculateMaximumWaitTime();
		
	}


};


#endif /* UTIL_H_ */