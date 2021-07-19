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
#define TOLERANCE 15 // may change later
#define CONTAINER_DEFAULT_SIZE 20
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
			elements[i] = INFINITY;
        
	}

	void addElement(int element){

		currentPointer++;
		
		currentPointer %= capacity;

		elements[currentPointer] = element;

	}
	
	int getLastReading(int kotoNumber){
		kotoNumber %= capacity;
		int nowPointer = currentPointer - kotoNumber;
		nowPointer += capacity;
		nowPointer %= capacity;
		return elements[nowPointer];
	}

/*
	bool isMoving(){
		
		if(currentPointer==-1) return false;

		for(int i=1;i<=4;i++)
		if(!isEqual(currentPointer,currentPointer-i)) return false;

		return true;

	}


	bool isEqual(int i,int j){

		if(j<0)
		j=j+capacity;

		return abs(distances[i]-distances[j])<=TOLERANCE;
	}
*/
/*
	void print(){

		for(int i=0;i<capacity;i++) 
		printf("%d ",distances[i]);
	
	}
	*/

};

class SonarUnit{
	Container *container;
	int warningStatus;
	public:
	SonarUnit(int saved_readings = CONTAINER_DEFAULT_SIZE){
		container = (Container *)malloc(sizeof(Container));
		warningStatus = Green;
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