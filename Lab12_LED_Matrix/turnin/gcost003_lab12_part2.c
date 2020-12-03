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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "scheduler.h"
#include "timer.h"

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------

//Global Variables
//static unsigned char B1 = 0x00;
//static unsigned char B2 = 0x00;
static unsigned char tempA = 0x00; 

enum Demo_States {wait, right, shiftRight, left, shiftLeft};
int Demo_Tick(int state) {

	// Local Variables
	//B1 = ~PINA & 0x01;
	//B2 = ~PINA & 0x02;
	tempA = ~PINA & 0x0C;	
		
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x00;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case wait:
			if (!tempA) {
				state = wait;
			}
			else if(tempA == 0x04) {
				state = right;
			}
			else if(tempA == 0x08) {
				state = left;
			}
			else {
				state = wait;
			}
			break;
		case right:
			state = shiftRight;
			break;
		case shiftRight:
			if(tempA == 0x04) {
				state = right;
			}
			else if (tempA == 0x08) {
				state = left;
			}
			else {
				state = wait;
			}
			break;
		case left:
			state = shiftLeft;
			break;
		case shiftLeft:
			if(tempA == 0x08) {
				state = left;
			} 
			else if (tempA == 0x04) {
				state = right;
			}
			else {
				state = wait;
			}
			break;
		default:	
			state = wait;
			break;
	}	
	// Actions
	switch (state) {
		case wait:
			break;
		case right:
			if(row == 0x00 && pattern == 0x01) {  //making it stay there
				pattern = 0x01;	
				row = 0x00; 
			}
			else if(pattern > 0x01) {  //move LED up the next row
				row = 0x00;
				pattern = (pattern >> 1);
				
			}
			else { // Shift LED one spot to the left on current pattern
				row >>= 1;
			}
			break;
		case shiftRight:
			break;
		case left:
			if (row == 0x00 && pattern == 0x80) { 
				row = 0x00;		    
				pattern = 0x80;
			}
			else if (!(pattern > 0x80)) { // Move LED to start of next pattern
				row = 0x00;
				pattern = (pattern << 1);
			}
			else { // Shift LED one spot to the right on current pattern
				row <<= 1;
			}
			break;
		case shiftLeft:
			break;
		default:
			break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;
	DDRC = 0xFF;	PORTC = 0x00;	//output for pattern
	DDRD = 0xFF;	PORTD = 0x00;	//output for row
	
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	const char start = 0;
	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct =  &Demo_Tick;

	unsigned long GCD = tasks[0]->period;
	for(unsigned short i = 1; i < numTasks; i++) {
		GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();
    /* Insert your solution below */
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
