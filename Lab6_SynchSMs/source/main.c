/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{B0, B1, B2} LED_state;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() { TimerFlag = 1; }

void Tick() {
	unsigned char tmpB;
	switch (LED_state) {
		case B0:
			LED_state = B1;
			tmpB = 0x01;
			break;
		case B1:
			LED_state = B2;
			tmpB = 0x02;
			break;
		case B2:
			LED_state = B0;
			tmpB = 0x04;
			break;
		default:
			LED_state = B0;
			tmpB = 0x01;
			break;
	}
	PORTB = tmpB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);  //timerset 1000 ms which is 1 sec
	TimerOn();
	LED_state = B0;

    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
