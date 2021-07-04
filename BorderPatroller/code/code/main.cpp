/*
 * code.cpp
 *
 * Created: 6/29/2021 2:29:38 PM
 * Author : vdrko
 */ 

#include <avr/io.h>
#ifndef F_CPU
	#define F_CPU 1000000UL
#endif

#include <util/delay.h>

#include "external_libraries/lcd/lcd.h"
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "our_libraries/util.h"
//using namespace  std;


// Distance Calculator Boss
DistanceCalculator distanceCalculator(29); // current temperature 29 degree celcius
class SonarUnit{
	int distance;
	public:
	
		SonarUnit(){
			distance = 10000;
		}
		~SonarUnit(){
			
		}
		
		char * foo(){
			return "I am sifat";
		}
	};


volatile uint32_t n;
volatile uint32_t timer_elapsed_time;
char temp_string[16];
ISR(TIMER1_OVF_vect){
	
	n++;
}

ISR(INT0_vect){
	timer_elapsed_time=n*65535+(uint32_t)TCNT1;
	/*
	lcd_gotoxy(0,0);
	lcd_puts(itoa(timer_elapsed_time));
	lcd_puts(" us INT0!");
	lcd_gotoxy(0, 1);
	
	lcd_puts(itoa(distanceCalculator.calculateDistance(timer_elapsed_time) / 10));
	lcd_puts(" cm");
	_delay_ms(1000);
	lcd_clrscr();
	*/
}



int main(void)
{
	
	int i,j;
	uint32_t elapsed_time;
	
	TCCR1A=0b00000000;
	TCCR1B=0b00000001;
	TIMSK =0b00000100;
	
	
    lcd_init(LCD_ON_CURSOR);
	lcd_clrscr();
	
	
	// motor rotate
	DDRA |= (0b11110000);
	
	// Trigger for Sonar1
	DDRB |= (1 << SN1_TRGR_1);
	
	
	
	unsigned char rotate = 0;
	unsigned int counter = 0;
	MCUCR = MCUCR | (0b00000010); // falling edge
	GICR = GICR | (1 << INT0);
    while (1) 
    {	
		n=0;
		TCNT1=0;
		//lcd_puts(itoa(n));
		PORTB = setBit(PORTB, SN1_TRGR_1);
		sei();
		_delay_us(12); // Sonar requires 12ms pulse
		
		PORTB = unsetBit(PORTB, SN1_TRGR_1);
		
		
		uint32_t wait_time = distanceCalculator.getMaximumWaitTime();
		_delay_us(25000); // How long we should wait before all sonar values are read
		
		cli();
		
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(itoa(counter));
		lcd_putc(':');
	   // lcd_gotoxy(1,0);	
		 int distance = distanceCalculator.calculateDistance(timer_elapsed_time) / 10;
		 lcd_puts(itoa(distance));
		//lcd_puts(itoa(0.0175 * (int)timer_elapsed_time));		
		//lcd_puts( itoa( distanceCalculator.calculateDistance(timer_elapsed_time) / 10 ) );
		
		lcd_puts(" cm");
		
		lcd_gotoxy(3, 1);
		lcd_puts(itoa(timer_elapsed_time));
		lcd_puts(" us");
		
		// motor roation
		PORTA = PORTA & (0b00001111);
		PORTA |= (1 << (rotate + 4 ) );
		rotate++;
		rotate= rotate%4;
		
		counter++;
		
		_delay_ms(2000);
    }
}

