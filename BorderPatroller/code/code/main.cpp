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
			return "I am sigat";
		}
	};

int main(void)
{
    lcd_init(LCD_ON_CURSOR);
	signed char i = 0xFF;
	lcd_clrscr();
	SonarUnit son;
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
		_delay_ms(2000);
    }
}

