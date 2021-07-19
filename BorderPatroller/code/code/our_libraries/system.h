/*
 * system.h
 *
 * Created: 7/19/2021 3:48:49 PM
 *  Author: vodro
 */ 
#include "./util.h"
#include "./components.h"

#ifndef SYSTEM_H_
#define SYSTEM_H_


class System{
	DistanceCalculator *distanceCalculator;
	MotorUnit *motor;
	LightUnit **light;
	BuzzerUnit **buzzer;
	SonarUnit **sonar;
	
	public:
	System();
	~System();
	
	void operate(){
		// the main code goes here
	}
	void updateReadings(uint32_t timer0_elapsed_time, uint32_t timer1_elapsed_time, uint32_t timer2_elapsed_time);
	
	void showWarnings();
	
	void calculateWarnings();
	
	void showReadings();
	
};

System::System(){
	distanceCalculator = (DistanceCalculator *)malloc(sizeof(DistanceCalculator));
	//distanceCalculator->setTemperature() // not implemented // default temp = 28
	
	motor = (MotorUnit *) malloc(sizeof(MotorUnit));
	light = (LightUnit **)malloc(4 * sizeof(LightUnit *));
	for(int i = 0; i < 4; i++){
		light[i] = (LightUnit *) malloc(sizeof(LightUnit));
	}
	light[RedHigh]->setPinPosition(PORTLTR1);
	light[RedLow]->setPinPosition(PORTLTR2);
	light[Yellow]->setPinPosition(PORTLTY);
	light[Green]->setPinPosition(PORTLTG);
	
	buzzer = (BuzzerUnit **) malloc(2 * sizeof(BuzzerUnit *));
	for(int i = 0; i < 2; i++){
		buzzer[i] = (BuzzerUnit *) malloc(sizeof(BuzzerUnit));
	}
	buzzer[RedHigh]->setPinPosition(PORTBZ0);
	buzzer[RedLow]->setPinPosition(PORTBZ1);
	
	SonarUnit temp;
	sonar = (SonarUnit **) malloc(3 * sizeof(SonarUnit *));
	for(int i = 0; i < 3; i++){
		sonar[i] = (SonarUnit *) malloc(sizeof(SonarUnit));
		sonar[i]->init();
	}
	
}
System::~System(){
	free(distanceCalculator);
	free(motor);
	for(int i = 0; i < 4; i++){
		free(light[i]) ;
	}
	free(light);
	for(int i = 0; i < 2; i++){
		free( buzzer[i]);
	}
	free(buzzer);
	for(int i = 0; i < 3; i++){
		free(sonar[i]);
	}
	free(sonar);
}


#endif /* SYSTEM_H_ */