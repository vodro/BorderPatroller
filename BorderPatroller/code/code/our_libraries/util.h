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

// Motor related
#define MOTOR_PERIOD 2000

#define PORTM PORTA
#define PORTM0 PORTA4
#define PORTM1 PORTA5
#define PORTM2 PORTA6
#define PORTM3 PORTA7

// FOR LED
#define PORTLT PORTD 
#define PORTLTR1 PORTD7
#define PORTLTR2 PORTD6
#define PORTLTY PORTD5
#define PORTLTG PORTD4

#define RED 200
#define YELLOW 300
#define GREEN 400

//FOR BUZZER
#define PORTBZ PORTD
#define PORTDBZ0 PORTD0
#define PORTDBZ1 PORTD1



enum WARNING_STATUS{RedHigh, RedLow, Yellow, Green};
//enum WARNING_DISTANCE{150, 500, 1500, 10000};


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

extern  char temp_string[16];
char *itoa(uint32_t n){
	//char s[10];
	temp_string[0] = 0;
	itoa(n, temp_string, 10);
	return temp_string;
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






#endif /* UTIL_H_ */