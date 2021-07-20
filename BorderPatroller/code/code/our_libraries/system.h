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
	
	int warning_status;
	
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
	warning_status = Green;
	distanceCalculator = (DistanceCalculator *)malloc(sizeof(DistanceCalculator));
	distanceCalculator->init();
	//distanceCalculator->setTemperature() // not implemented // default temp = 28
	
	motor = (MotorUnit *) malloc(sizeof(MotorUnit));
	motor->init();
	motor->setRevolutionNeeded(MOTOR_REVOLUTION_NEEDED);
	
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
	
	//resetLightsBuzzers();
	warning_status = Green;
	for(int i=0; i<3;i++){
		  this->warning_status = min(this->warning_status, sonar[i]->getWarningStatus() );
	}
	
}

void System::showReadings(){
	for(int i=0; i<3;i++){
		
		if(i > 1)
		{
			lcd_clrscr();
		}
		lcd_gotoxy(0,i&1);
		lcd_puts(itoa(i));
		lcd_puts("> ");
		lcd_puts(itoa(sonar[i]->getLastReading()/10));
		lcd_puts(" cm");
		switch(sonar[i]->getWarningStatus()){
			
			
			case RedHigh:
			lcd_puts(" : RDHg");
			break;
			case RedLow:
			lcd_puts(" : RDLw");
			break;
			case Yellow:
			lcd_puts(" : YE");
			break;
			default:
			lcd_puts(" : GRN");
		}
		lcd_puts("   ");
		_delay_ms(PRINTING_DELAY);
		
	}
	
}

void System::showWarnings(){
	resetLightsBuzzers();
	switch(this->warning_status){
		case RedHigh:
			light[RedHigh]->switchOn();
			light[RedLow]->switchOn();
			buzzer[RedHigh]->switchOn();
			buzzer[RedLow]->switchOn();
			motor->load();
			break;
		case RedLow:
			light[RedLow]->switchOn();
			buzzer[RedLow]->switchOn();
			motor->unLoad();
			break;
		case Yellow:
			light[Yellow]->switchOn();
			motor->unLoad();
			break;
		case Green:
			light[Green]->switchOn();
			motor->unLoad();
			break;
		default:
			vulval();
	}
}


#endif /* SYSTEM_H_ */