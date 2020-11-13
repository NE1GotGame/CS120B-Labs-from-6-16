/*	Author: Giovanni Costagliola
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Test Demo: https://youtu.be/YW70DpgB77w
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;
	DDRB = 0xFF;	PORTB = 0x00;
	DDRD = 0xFF;	PORTD = 0x00;

	unsigned short x;
	unsigned char tmpB;
	//unsigned char tmpD;
	unsigned char Max = 0x80;
	ADC_init();

    /* Insert your solution below */
    while (1) {
	x = ADC;
	tmpB = (char)x;
	if (tmpB >= (Max/2) ) {
		tmpB = 0x01;
	}
	else {
		tmpB = 0x00;
	}	
	PORTB = tmpB;
	//PORTD = tmpD;
    }
    return 1;

}
