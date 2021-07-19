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
#include "our_libraries/components.h"
#include "our_libraries/system.h"
//using namespace  std;

/*
// Distance Calculator Boss
DistanceCalculator distanceCalculator(20); // current temperature 29 degree celcius
LightUnit redLed1(PORTLTR1),redLed2(PORTLTR2),yellowLed(PORTLTY),greenLed(PORTLTG);
BuzzerUnit buzzer0(PORTBZ0), buzzer1(PORTBZ0);
*/

volatile uint32_t timer_count;
volatile uint32_t timer0_elapsed_time;
volatile uint32_t timer1_elapsed_time;
volatile uint32_t timer2_elapsed_time;

ISR(TIMER1_OVF_vect){
	
	timer_count++;
}

ISR(INT0_vect){
	timer0_elapsed_time=(uint32_t)TCNT1 + (timer_count << 16);	
}

ISR(INT1_vect){
	timer1_elapsed_time=(uint32_t)TCNT1 + (timer_count << 16);	
}

ISR(INT2_vect){
	timer2_elapsed_time = (uint32_t)TCNT1 + (timer_count << 16);
}


void test(){
	System system;
		
}

int main(void)
{
	//Timer1 initialization
	TCCR1A=0b00000000;
	TCCR1B=0b00000001;
	TIMSK =0b00000100;
	
	//LCD INITIALIZATION
    lcd_init(LCD_ON_CURSOR);
	lcd_clrscr();
	
	
	
	/* motor */
	//DDRA |= (0b11110000);
	DDRA = DDRA | (0xF0);
	
	// led & buzzer configuration
	DDRD|=0b11110011;
	
	// Trigger for All sonar
	DDRB |= (1 << SN1_TRGR_1);
	
	
	// All interrupts initialized
	MCUCR = MCUCR | (0b00000010); // falling edge INT0
	GICR = GICR | (1 << INT0);
	MCUCR = MCUCR | (0b00001000); // falling edge INT1
	GICR = GICR | (1 << INT1);
	MCUCSR = MCUCSR & ~(1 << 7);
	GICR = GICR | (1 << INT2);
	//MCUCR = MCUCR | (0b00001010);
	//GICR = GICR | ( 1 << INT0 | 1 << INT1);
	
	
	//test();
	
	System system;
	
    while (1) 
    {	
		
		//lcd_puts(itoa(n));
		PORTB = setBit(PORTB, SN1_TRGR_1);
	
		_delay_us(10); // Sonar requires 10ms pulse
		PORTB = unsetBit(PORTB, SN1_TRGR_1);
		_delay_us(200); // 8 40khz signal
		
		sei();
		timer_count=0;
		TCNT1=0;
		
		//uint32_t wait_time = distanceCalculator.getMaximumWaitTime();
		_delay_us(25000); // How long we should wait before all sonar values are read
		
		cli();
		
		
		system.updateReadings(timer0_elapsed_time,timer1_elapsed_time,timer2_elapsed_time);
		system.calculateWarnings();	
		//system.showWarnings();
		system.showReadings();
		
		
		
    }
}

