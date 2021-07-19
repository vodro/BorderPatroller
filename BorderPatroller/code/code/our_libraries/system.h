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

char temp_string[16];

class System{
	DistanceCalculator *distanceCalculator;
	MotorUnit *motor;
	LightUnit **light;
	BuzzerUnit **buzzer;
	SonarUnit **sonar;
	int sonarIDCount;
	
	
	public:
	System();
	~System();
	
	void operate(){
		// the main code goes here
	}
	void updateReadings(uint32_t timer0_elapsed_time, uint32_t timer1_elapsed_time, uint32_t timer2_elapsed_time);
	
	void calculateWarnings();
	
	void showWarnings();
	
	void showReadings();
	
	void  resetLightsBuzzers();
};

System::System(){
	sonarIDCount = 0;
	distanceCalculator = (DistanceCalculator *)malloc(sizeof(DistanceCalculator));
	distanceCalculator->init();
	//distanceCalculator->setTemperature() // not implemented // default temp = 28
	
	motor = (MotorUnit *) malloc(sizeof(MotorUnit));
	motor->init();
	light = (LightUnit **)malloc(4 * sizeof(LightUnit *));
	for(int i = 0; i < 4; i++){
		light[i] = (LightUnit *) malloc(sizeof(LightUnit));
		light[i]->init();
	}
	light[RedHigh]->setPinPosition(PORTLTR1);
	light[RedLow]->setPinPosition(PORTLTR2);
	light[Yellow]->setPinPosition(PORTLTY);
	light[Green]->setPinPosition(PORTLTG);
	
	buzzer = (BuzzerUnit **) malloc(2 * sizeof(BuzzerUnit *));
	for(int i = 0; i < 2; i++){
		buzzer[i] = (BuzzerUnit *) malloc(sizeof(BuzzerUnit));
		buzzer[i]->init();
	}
	buzzer[RedHigh]->setPinPosition(PORTBZ0);
	buzzer[RedLow]->setPinPosition(PORTBZ1);
	
	;
	sonar = (SonarUnit **) malloc(3 * sizeof(SonarUnit *));
	for(int i = 0; i < 3; i++){
		sonar[i] = (SonarUnit *) malloc(sizeof(SonarUnit));
		sonar[i]->init(sonarIDCount++);
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

void System:: updateReadings(uint32_t timer0_elapsed_time, uint32_t timer1_elapsed_time, uint32_t timer2_elapsed_time){
	
	sonar[0]->pushReading(distanceCalculator->calculateDistance(timer0_elapsed_time));
	sonar[1]->pushReading(distanceCalculator->calculateDistance(timer1_elapsed_time));
    sonar[2]->pushReading(distanceCalculator->calculateDistance(timer2_elapsed_time));

	
}

void System:: resetLightsBuzzers(){
	light[RedHigh]->switchOff();
	light[RedLow]->switchOff();
	light[Yellow]->switchOff();
	light[Green]->switchOff();
	
	buzzer[RedHigh]->switchOff();
	buzzer[RedLow]->switchOff();
	
}
void System::calculateWarnings(){
	
  resetLightsBuzzers();
	
	for(int i=0; i<3;i++){
		
		switch(sonar[i]->getWarningStatus()){
			
			case RedHigh:
				light[RedHigh]->switchOn();
				light[RedLow]->switchOn();
				buzzer[RedHigh]->switchOn();
				buzzer[RedLow]->switchOn();
				break;
			case RedLow:
			    light[RedLow]->switchOn();
			    buzzer[RedLow]->switchOn();
			    break;
			case Yellow:
			    light[Yellow]->switchOn();
			    break;
		   default:
		           light[Green]->switchOn();		
			
		}	
	}
}

void System::showReadings(){
	
	for (int i=0;i<3;i++)
	{
		sonar[i]->printCurrentReading();
	}
	
}




#endif /* SYSTEM_H_ */