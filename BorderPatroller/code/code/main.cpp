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
#ifndef  WARNINGS
    #define  RED 1
	#define  YELLOW 2
	#define  GREEN 3
#endif
#include <util/delay.h>

#include "external_libraries/lcd/lcd.h"
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "our_libraries/util.h"
//using namespace  std;


// Distance Calculator Boss
DistanceCalculator distanceCalculator(20); // current temperature 29 degree celcius



volatile uint32_t n;
volatile uint32_t timer0_elapsed_time;
volatile uint32_t timer1_elapsed_time;
volatile uint32_t timer2_elapsed_time;
char temp_string[16];
ISR(TIMER1_OVF_vect){
	
	n++;
}

ISR(INT0_vect){
	timer0_elapsed_time=(uint32_t)TCNT1 + (n << 16);
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


ISR(INT1_vect){
	timer1_elapsed_time=(uint32_t)TCNT1 + (n << 16);	
}

ISR(INT2_vect){
	timer2_elapsed_time = (uint32_t)TCNT1 + (n << 16);
}


int main(void)
{
	
	
	TCCR1A=0b00000000;
	TCCR1B=0b00000001;
	TIMSK =0b00000100;
	
	
    lcd_init(LCD_ON_CURSOR);
	lcd_clrscr();
	
	
	// motor rotate
	
	// Trigger for Sonar1
	DDRB |= (1 << SN1_TRGR_1);
	
	
	
	unsigned char rotate = 0;
	unsigned int counter = 0;
	
	MCUCR = MCUCR | (0b00000010); // falling edge INT0
	GICR = GICR | (1 << INT0);
	MCUCR = MCUCR | (0b00001000); // falling edge INT1
	GICR = GICR | (1 << INT1);
	MCUCSR = MCUCSR & ~(1 << 7);
	GICR = GICR | (1 << INT2);
	//MCUCR = MCUCR | (0b00001010);
	//GICR = GICR | ( 1 << INT0 | 1 << INT1);
	
	
	/* motor  testing */
	DDRA |= (0b11110000);
	DDRA = DDRA | (0xF0);
	MotorUnit motorUnit;
	int motorCounter = 1;
	while(1){
		motorUnit.rotateAntiClockWise(motorCounter * 360);
		//motorCounter++;
		_delay_ms(1000);
		motorUnit.rotateClockWise(motorCounter * 360);
		_delay_ms(1000);
		
		
	}
	
	int sonar0_distance,sonar1_distance,sonar2_distance=0;
    while (1) 
    {	
		
		//lcd_puts(itoa(n));
		PORTB = setBit(PORTB, SN1_TRGR_1);
		
		
		
		_delay_us(10); // Sonar requires 10ms pulse
		PORTB = unsetBit(PORTB, SN1_TRGR_1);
		_delay_us(200); // 8 40khz signal
		
		sei();
		n=0;
		TCNT1=0;
		
		//uint32_t wait_time = distanceCalculator.getMaximumWaitTime();
		_delay_us(25000); // How long we should wait before all sonar values are read
		
		cli();
		
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(itoa(counter));
		lcd_putc(':');
		lcd_puts("0>");
		;
		sonar0_distance=distanceCalculator.calculateDistance(timer0_elapsed_time) / 10;
		lcd_puts(itoa(sonar0_distance));
		lcd_puts(" cm");
		
		
		lcd_puts(itoa(timer0_elapsed_time));
		lcd_puts(" us");
		
		
		lcd_gotoxy(0, 1);
		lcd_puts(itoa(counter));
		lcd_putc(':');
		lcd_puts("1>");
        sonar1_distance=distanceCalculator.calculateDistance(timer1_elapsed_time) / 10;
        lcd_puts(itoa(sonar1_distance));
		lcd_puts(" cm");
		
		
		lcd_puts(itoa(timer1_elapsed_time));
		lcd_puts(" us");
		
		_delay_ms(1000);
		
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(itoa(counter));
		lcd_putc(':');
		lcd_puts("2>");
		;
		sonar2_distance=distanceCalculator.calculateDistance(timer2_elapsed_time) / 10;
		lcd_puts(itoa(sonar2_distance));
		lcd_puts(" cm");
		lcd_puts(itoa(timer2_elapsed_time));
		lcd_puts(" us");
		
		// led configuration
		DDRD|=0xFF;
		int sn0_warn=getSignal(sonar0_distance);
		int sn1_warn=getSignal(sonar1_distance);
		int sn2_warn=getSignal(sonar2_distance);

		if(sn0_warn==RED||sn1_warn==RED||sn2_warn==RED)
		 PORTD=0b11000000;
		 else if(sn0_warn==YELLOW||sn1_warn==YELLOW||sn2_warn==YELLOW)
		 PORTD=0b00100000;
		 else PORTD=0b00010000;
		
		
		// motor roation
		PORTA = PORTA & (0b00001111);
		PORTA |= (1 << (rotate + 4 ) );
		rotate++;
		rotate= rotate%4;
		
		counter++;
		
		_delay_ms(2000);
    }
}

