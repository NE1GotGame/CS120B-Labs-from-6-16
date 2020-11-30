/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Test: https://youtu.be/XvWmJp4gOwg
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "scheduler.h"
#include "timer.h"

void transmit_data(unsigned char data) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
   	 PORTC = 0x08;
   	 // set SER = next bit of data to be sent.
   	 PORTC |= ((data >> i) & 0x01);
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 PORTC |= 0x02;  
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}

unsigned char cnt = 0x00;
unsigned char B1 = 0x00; //increase button on A0
unsigned char B2 = 0x00; //decrease button on A1
//unsigned char j = 0x00;

enum incdecSM{init, increase, incRelease, decrease, decRelease, reset};

int IncDecLED(int state) {
	B1 = ~PINA & 0x01;
	B2 = ~PINA & 0x02;
	switch (state) {	
		case init:
			if(!B1 && !B2){
				state = init;
			}
			else if(B1 && !B2) {
				state = increase;
			}
			else if(!B1 && B2) {
				state = decrease;
			}
			else { state = reset; }		
			break;
		case increase:
			if (B1 && B2) {
				state = reset;
			}
			else {
				state = incRelease;
			}
			break;
		case incRelease:
			if (B1 && B2) {
				state = reset;
			}
			else if (B1 && !B2) {
				state = increase;
			}
			else if(!B1 && B2) {
				state = decrease;
			}
			else {
				state = incRelease;
			}
			break;
		case decrease:
			if (B1 && B2) {
				state = reset;
			}
			else {
				state = decRelease;
			}
		case decRelease:
			if (B1 && B2) {
				state = reset;
			}
			else if (!B1 && B2) {
				state = decrease;
			}
			else if (B1 && !B2) {
				state = increase;
			}
			else {
				state = decRelease;
			}
			break;
		case reset:
			if(!B1 && !B2) {
				state = init;
			}
			else if (B1 && B2) {
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

	switch(state) {
		case init:
			transmit_data(0); //also can call the function to intitialize to be zero
			break;
		case increase:
			cnt = cnt + 1;
			transmit_data(cnt);
			break;
		case incRelease:
			break;
		case decrease:
			if (cnt > 0x00) {
				cnt = cnt - 1;
			}
			else { cnt = 0x00; }
			transmit_data(cnt);
			break;
		case decRelease:
			break;
		case reset:
			cnt = 0x00;
			transmit_data(0);
			break;
		default:
			state = init;
			break; 
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;	
	DDRC = 0xFF;	PORTC = 0x00;
	
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	const char start = 0;
	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct =  &IncDecLED;

	unsigned long GCD = tasks[0]->period;
	for(unsigned short i = 1; i < numTasks; i++) {
		GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();

    while (1) {
	
	    for(unsigned short j = 0; j < numTasks; j++)
	    {
		    if(tasks[j]->elapsedTime == tasks[j]->period){
			    tasks[j]->state = tasks[j]->TickFct(tasks[j]->state);
			    tasks[j]->elapsedTime = 0;
		    }
		    tasks[j]->elapsedTime += GCD;
	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
