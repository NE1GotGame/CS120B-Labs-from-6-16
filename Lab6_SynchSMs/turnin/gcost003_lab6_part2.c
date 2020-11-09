/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Test Demo:  https://youtu.be/fFw943hII7I
 */
#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{start, B0, B1, B2, wait, reset} state;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() { TimerFlag = 1; }

unsigned char nextState;
unsigned char tmpA;
unsigned char tmpB;

void Tick() {
	tmpA = ~PINA & 0x01;	

	switch (state) {
		case start:
			state = B0;
			break;
		case B0:
			if (tmpA == 0x01) {
				state = wait;
			}
			else {
				state = B1;
				nextState = B2;
			}
			break;
		case B1:
			if (tmpA == 0x01) {
				state = wait;
			}
			else {
				state = nextState;
			}
			break;
		case B2:
			if (tmpA == 0x01) {
				state = wait;
			}
			else {
				nextState = B0;
				state = B1;
			}
			break;
		case wait:
			if (tmpA == 0x01) { 
				state = reset;
			}
			else {
				state = wait;
			}
			break;
		case reset:
			if (tmpA == 0x01) {
				state = B0;
			}
			else {
				state = reset;
			}
			break;
		default: 
			state = start;
			break;
	}
	switch (state) {
		case start:
			break;
		case B0:
			tmpB = 0x01;
			break;
		case B1:
			tmpB = 0x02;
			break;
		case B2:
			tmpB = 0x04;
			break;
		case wait:
		case reset:
			break;
		default:
			tmpB = 0x01;
			break;
	}
	PORTB = tmpB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);  //timerset 300 ms
	TimerOn();
	state = start;

    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
