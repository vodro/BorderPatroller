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
#define PORTBZ0 PORTD0
#define PORTBZ1 PORTD1

#define TOLERANCE_DISTANCE 50 // may change later

#define CONTAINER_DEFAULT_SIZE 20
#define MOVING_READING_COUNT 4

enum WARNING_STATUS{RedHigh, RedLow, Yellow, Green};
enum WARNING_DISTANCE_LIMIT{RedHighLimit = 150, RedLowLimit = 500, YellowLimit = 1500, GreenLimit = 10000};



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
	DistanceCalculator(float temp = 28)
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


class Container{

	int *elements;
	int capacity;
	int currentPointer;

	public:
	Container(int capacity = CONTAINER_DEFAULT_SIZE){
		this->capacity=capacity;
		elements = (int *) malloc(sizeof(int) * capacity);
		currentPointer = -1;
		for(int i=0; i < capacity; i++)
		elements[i] = (int)INFINITY;
		
	}

	void addElement(int element){

		currentPointer++;
		
		currentPointer %= capacity;

		elements[currentPointer] = element;

	}
	
	int getLastReading(int kotoNumber = 0){
		kotoNumber %= capacity;
		int nowPointer = currentPointer - kotoNumber;
		nowPointer += capacity;
		nowPointer %= capacity;
		return elements[nowPointer];
	}

};





#endif /* UTIL_H_ */