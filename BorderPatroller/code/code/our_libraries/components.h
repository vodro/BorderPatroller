#ifndef COMPONENTS_H_
#define COMPONENTS_H_
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


/*MOVING OBJECT DETECTOR CLASS*/


class SonarUnit{
	int id;
	Container *container;
	int warningStatus;
	
	public:
	static int idCount;
	SonarUnit(int saved_readings = CONTAINER_DEFAULT_SIZE){
		init(saved_readings);
	}
	
	void init(int saved_readings = CONTAINER_DEFAULT_SIZE ){
		this->id = idCount;
		idCount++;
		container = (Container *)malloc(sizeof(Container));
		warningStatus = Green;
	}
	
	bool isMoving(){
		// NEED TO MAKE FAULT TOLERANT
		//int previousReading = container->getLastReading(1);
		//int currentReading = container->getLastReading(0);
		for(int i = 1; i <= MOVING_READING_COUNT; i++){
			if(isDifferent(0, i))
				return true;
		}
		return false;
	}
	
		
	double distanceBetween(int x, int y){
		return abs(x-y);
	}
	
	bool isDifferent(int i, int j){
		return distanceBetween(container->getLastReading(i), container->getLastReading((j))) >= TOLERANCE_DISTANCE;
	}
	
	int getWarningStatus(){
		return warningStatus;
	}
	
	void setWarningStatus(int status){
		this->warningStatus = status;
	}
	
	void pushReading(int distance){
		container->addElement(distance);
		if(isMoving()){
			if(distance <= RedHighLimit){
				setWarningStatus(RedHigh);
			}else if(distance <= RedLowLimit){
				setWarningStatus(RedLow);
			}else if(distance <= YellowLimit){
				setWarningStatus(Yellow);
			}else{
				setWarningStatus(Green);
			}
		}else{
			setWarningStatus(Green);
		}
	}
	
	void printCurrentReading(){
		lcd_clrscr();
		lcd_gotoxy(0, this->id & 1);
		//lcd_puts(itoa(counter));
		lcd_putc(':');
		lcd_puts(itoa(id));
		lcd_puts(">");
		;
		int distance = container->getLastReading() / 10;
		lcd_puts(itoa(distance));
		lcd_puts(" cm");
	}
};

class LightUnit{
	int pinPosition;
	public:
	LightUnit(int pinPosition){
		this->pinPosition = pinPosition;
	}
	void setPinPosition(int pos){
		pinPosition=pos;
	}
	
	int getPinPosition(){
		return pinPosition;
	}
	
	void switchOn(){
		PORTLT=setBit(PORTLT,pinPosition);	
	}
	
	void switchOff(){
		PORTLT=unsetBit(PORTLT,pinPosition);
	}

};

class BuzzerUnit{
	int pinPosition;
	public:
	BuzzerUnit(int pinPosition){
		this->pinPosition = pinPosition;
	}
	void setPinPosition(int pos){
		pinPosition=pos;
	}
	
	int getPinPosition(){
		return pinPosition;
	}
	
	void switchOn(){
		PORTBZ=setBit(PORTBZ,pinPosition);
	}
	
	void switchOff(){
		PORTBZ=unsetBit(PORTBZ,pinPosition);
	}
	


};
#endif /* COMPONENTS_H_ */