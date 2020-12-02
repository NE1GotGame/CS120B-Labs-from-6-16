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

//static int row[3] = {0xFD, 0xFB, 0xF7};
//static int pattern[3]= {0x3C, 0x24, 0x3C};

static unsigned char cnt = 0x00; 

enum Demo_States {S1, S2, S3};
int Demo_Tick(int state) {

	// Local Variables
	//B1 = ~PINA & 0x01;
	//B2 = ~PINA & 0x02;
	//tempA = ~PINA & 0x00;	
		
	static unsigned char pattern; //= 0x3C;	 LED pattern - 0: LED off; 1: LED on
	static unsigned char row;     //= 0xE1;  	 Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case S1:
			if(cnt == 0) {
				cnt++;
				row = 0xFD;
				pattern = 0x3C;
				state = S2;			
			}			
			break;
		case S2:
			if(cnt == 1) {
				cnt++;
				row = 0xFB;
				pattern = 0x24;
				state = S3;
			}
			break;
		case S3:
			if(cnt == 2) {
				cnt = 0;
				row = 0xF7;
				pattern = 0x3C;
				state = S1;	
			}
			break;
		default:
			break;
	}	
	// Actions
	/*switch (state) {
		case Display:
			pattern = pattern & 0x
		default:
			break;
	}*/
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
	task1.period = 1;
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
