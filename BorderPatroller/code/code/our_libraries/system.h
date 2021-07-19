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
	MotorUnit motor;
	//LightUnit light[4];
	BuzzerUnit buzzer[2];
	SonarUnit sonar[4];
	public:
	System(){
		// bu
	}
	void operate(){
		// the main code goes here
	}
};


#endif /* SYSTEM_H_ */