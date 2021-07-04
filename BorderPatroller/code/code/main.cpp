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
#include "avr/interrupt.h"
#include <stdlib.h>
//using namespace  std;
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
ISR(TIMER1_OVF_vect){
	
	n++;
}
int main(void)
{
	
	int i,j;
	uint32_t elapsed_time;
	
	TCCR1A=0b00000000;
	TCCR1B=0b00000001;
	TIMSK =0b00000100;
	
	
    lcd_init(LCD_ON_CURSOR);
	//signed char i = 0xFF;
	lcd_clrscr();
	SonarUnit son;
	unsigned int counter = 0;
    while (1) 
    {
		char s[10];
		lcd_gotoxy(0,0);
		itoa(counter, s, 10);
		lcd_puts(s); lcd_putc(':');
		
		
		n=0;
		TCNT1=0;
		
		sei();
		
		for (i=0;i<100;i++)
		{ for (j=0;j<counter;j++)
			{
				//;
			}
		}
		elapsed_time=n*65535+(uint32_t)TCNT1;
		cli();
		
		
		
		
	   // lcd_gotoxy(1,0);			
		itoa(elapsed_time, s, 10);
		lcd_puts(s);
		counter++;
		
		//i--;
		//lcd_gotoxy(0, 1);
		//lcd_puts(son.foo());
		_delay_ms(2000);
    }
}

