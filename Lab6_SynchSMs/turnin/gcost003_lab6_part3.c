/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Test Demo:  https://youtu.be/E_qTNnFkpfI
 */
#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{init, increase, wait1, decrease, wait2, reset}state;

unsigned char Button1;
unsigned char Button2;
unsigned char tmpB = 0x07;

//volatile unsigned char TimerFlag = 0;
//void TimerISR() { TimerFlag = 1; }

void Tick() {
	Button1 = ~PINA & 0x01;	
	Button2 = ~PINA & 0x02;

	switch (state) {
		case init:
			if (Button1) {
				state = wait1;
			}
			else if (Button2) {
				state = wait2;
			}
			else {
				state = init;
			}
			break;
		case increase:
			state = init;
			break;
		case wait1:
			if (Button1 && Button2) {
				state = reset;
			}
			else if (Button1) {
				if(Button1) {				
					while(!TimerFlag && Button1) {
						state = increase;
					}			
				}
				else {	
					state = wait1;
					TimerOn();
				}
			}
			else {
				state = increase;
			}
			break;

		case decrease:
			state = init;
			break;
		case wait2:
			if (Button1 && Button2) {
				state = reset;
			}
			else if (Button2) {
				if(Button2) {				
					while(!TimerFlag && Button2) {
						state = decrease;
					}			
				}
				else {	
					state = wait2;
					TimerOn();
				}
			}
			else {
				state = decrease;
			}
			break;
		case reset:
			if (Button1 && Button2) {
				state = reset;
			}
			else {
				state = init;
			}
			break;
		default:
			state = init;
			break;
	}
	switch (state) {
		case init:
			break;
		case increase:
			if(tmpB < 9) {
				tmpB += 0x01;
			}
			TimerFlag = 0;
			break;
		case wait1:
			break;
		case decrease:
			if(tmpB >= 1) {
				tmpB = tmpB - 0x01;
			}
			TimerFlag = 0;
			break;
		case wait2:
			break;
		case reset:
			tmpB = 0x00;
			break;
		
	}
	PORTB = tmpB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);  //timerset 100 ms
	TimerOn();
	state = init;

    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
