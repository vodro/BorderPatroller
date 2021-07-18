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

#define  PORTM PORTA
#define PORTM0 PORTA4
#define PORTM1 PORTA5
#define PORTM2 PORTA6
#define PORTM3 PORTA7

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


class MotorUnit{
		double gearRatio;
		int fullRotationStep;
		
		void unsetPins();
		
		void rotateAntiClockWise(double degree);
		void rotateClockWise(double degree);
	public:
		MotorUnit(){
			this->gearRatio = 64;
			//this->gearRatio = 63.68395;
			this->fullRotationStep = 32;
		}
		
		
		void revolveClockwise(double n){
			rotateClockWise(n * 360.0);
		}
		
		void revolveAntiClockWise(double n){
			rotateAntiClockWise(n * 360.0);
		}
		
};

void MotorUnit::unsetPins(){
	PORTM = unsetBit(PORTM, PORTM1);
	PORTM = unsetBit(PORTM, PORTM2);
	PORTM = unsetBit(PORTM, PORTM3);
	PORTM = unsetBit(PORTM, PORTM0);
}

void MotorUnit:: rotateClockWise(double degree){
	int step = ceil( degree * gearRatio * fullRotationStep / 360.0 );
	lcd_gotoxy(0, 0);
	lcd_puts("degree : ");
	lcd_puts(itoa(degree));
	lcd_gotoxy(0, 1);
	lcd_puts("step : ");
	lcd_puts(itoa(step));
	if(!step)
		return;
	unsetPins();
	int now = 2;
	while(step--){
		/*
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts("jolbe : " );
			lcd_puts(itoa(now));
			lcd_gotoxy(0, 1);
			lcd_puts("stp r : ");
			lcd_puts(itoa(step));
			//_delay_ms(1000);
			*/
		switch(now){
			case 0:
				PORTM = unsetBit(PORTM, PORTM0);
				PORTM = setBit(PORTM, PORTM1);
				PORTM = setBit(PORTM, PORTM2);
				_delay_us(MOTOR_PERIOD);
				break;
			case 1:
				PORTM = unsetBit(PORTM, PORTM1);
				PORTM = setBit(PORTM, PORTM2);
				PORTM = setBit(PORTM, PORTM3);
				_delay_us(MOTOR_PERIOD);
				break;
			case 2:
				PORTM = unsetBit(PORTM, PORTM2);
				PORTM = setBit(PORTM, PORTM3);
				PORTM = setBit(PORTM, PORTM0);
				_delay_us(MOTOR_PERIOD);
				break;
			case 3:
				PORTM = unsetBit(PORTM, PORTM3);
				PORTM = setBit(PORTM, PORTM0);
				PORTM = setBit(PORTM, PORTM1);
				_delay_us(MOTOR_PERIOD);
				break;
		}
		now++;
		now &= (0x03);
	}
			
	unsetPins();
			 
}
		
void MotorUnit::rotateAntiClockWise(double degree){
	int step = ceil( degree * gearRatio * fullRotationStep / 360.0 );
	lcd_gotoxy(0, 0);
	lcd_puts("degree : ");
	lcd_puts(itoa((int)degree));
	lcd_gotoxy(0, 1);
	lcd_puts("step : ");
	lcd_puts(itoa(step));
	if(!step)
		return;
	unsetPins();
	int now = 2; // M0 on first
	while(step--){
		/*
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts("jolbe : " );
			lcd_puts(itoa(now));
			lcd_gotoxy(0, 1);
			lcd_puts("stp r : ");
			lcd_puts(itoa(step));
			//_delay_ms(1000);
			*/
		switch(now){
			case 0:
				PORTM = unsetBit(PORTM, PORTM0);
				PORTM = setBit(PORTM, PORTM3);
				PORTM = setBit(PORTM, PORTM2);
				_delay_us(MOTOR_PERIOD);
				break;
			case 1:
				PORTM = unsetBit(PORTM, PORTM1);
				PORTM = setBit(PORTM, PORTM0);
				PORTM = setBit(PORTM, PORTM3);
				_delay_us(MOTOR_PERIOD);
				break;
			case 2:
				PORTM = unsetBit(PORTM, PORTM2);
				PORTM = setBit(PORTM, PORTM1);
				PORTM = setBit(PORTM, PORTM0);
				_delay_us(MOTOR_PERIOD);
				break;
			case 3:
				PORTM = unsetBit(PORTM, PORTM3);
				PORTM = setBit(PORTM, PORTM2);
				PORTM = setBit(PORTM, PORTM1);
				_delay_us(MOTOR_PERIOD);
				break;
		}
		now--;
		now &= (0x03);
	}
			
	unsetPins();
			 
}




#endif /* UTIL_H_ */