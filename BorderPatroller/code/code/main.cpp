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
			return "I Sonar ";
		}
	};

int main(void)
{
    lcd_init(LCD_ON_CURSOR);
	
	DDRA |= (0b11110000);
	
	lcd_clrscr();
	signed char i = 0xFF;
	SonarUnit son;
	unsigned char rotate = 0;
    while (1) 
    {
		
		lcd_gotoxy(0,0);
		char s[10];
		itoa(i, s, 10);
		//s[9]=0;
		lcd_puts(s);
		i--;
		lcd_gotoxy(0, 1);
		lcd_puts(son.foo());
		lcd_puts(" Sifat");
		
		PORTA = PORTA & (0b00001111);
		PORTA |= (1 << (rotate + 4 ) );
		rotate++;
		rotate= rotate%4;
		
		_delay_ms(2000);
    }
}

