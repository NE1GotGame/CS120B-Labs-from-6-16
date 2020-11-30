/*	Author: lab
 *  Partner(s) Name: kgonz
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
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#endif

unsigned char key = 0x00; 

enum KeypadSM{keyword};
//static unsigned char button;

int keypadTick(int keypadState) {
	key = GetKeypadKey();

	switch(keypadState) {
		case keyword:
			keypadState = keyword;
			break;

		default:
			keypadState = keyword;
			break;
	}

	switch(keypadState) {
		case keyword:
			switch(key)
			{
				case '\0':
                  			PORTB = 0x00;
                  			break;

				case '0':
					PORTB = 0x80;
					break;

                  		case '1':
                  			PORTB = 0x80;
                  			break;

                  		case '2':
                  			PORTB = 0x80;
                  			break;

                  		case '3':
                  			PORTB = 0x80;
                  			break;

                  		case '4':
                  			PORTB = 0x80;
                  			break;

                  		case '5':
                  			PORTB = 0x80;
                  			break;

                  		case '6':
                  			PORTB = 0x80;
                  			break;

                  		case '7':
                  			PORTB = 0x80;
                  			break;

                  		case '8':
                  			PORTB = 0x80;
                  			break;

                  		case '9':
                  			PORTB = 0x80;
                  			break;

                  		case 'A':
                  			PORTB = 0x80;
                  			break;

                  		case 'B':
                  			PORTB = 0x80;
                  			break;

                  		case 'C':
                  			PORTB = 0x80;
                  			break;

                  		case 'D':
                  			PORTB = 0x80;
                  			break;

                  		case '*':
                  			PORTB = 0x80;
                  			break;

                  		case '#':
                  			PORTB = 0x80;
                  			break;

				default:
					PORTB = 0x00;
					break;
			}
	break;
	}

	return keypadState;
}

enum KeypadUnlockSM{init, pound, one, two, three, four, unlock, lock};  //I need pressState and releaseState
//should of create array
//int keysequenace[5]= {'1', '2', '3', '4', '5' };
//
int unlockTick(int unlock_state) {
	switch (unlock_state) {
		case init:
			key = GetKeypadKey();
			if (key == '#'){ 
				unlock_state = pound;
				key = 0;
			}
			else { unlock_state = lock; }
			break;
		case pound:
			key = GetKeypadKey();
			if(key == /*keysequenace[0]*/) {
				unlock_state = one;
				key = 0;
			}
			else {
				unlock_state = init;
			}
			break;
		case one:
			key = GetKeypadKey();
			if(key == '2') {
				unlock_state = two;
				key = 0;
			}
			else {
				unlock_state = init;
			}
			break;
		case two:
			key = GetKeypadKey();
			if(key == '3') {
				unlock_state = three;
				key = 0;
			}
			else {
				unlock_state = init;
			}
			break;
		case three:
			if(key == '4') {
				unlock_state = four;
				key = 0;
			}
			else {
				unlock_state = init;
			}
			break;
		case four:
			key = GetKeypadKey();
			if(key == '5') {
				unlock_state = unlock;
				key = 0;
			}
			else {
				unlock_state = init;
			}
			break;
		case unlock:
			unlock_state = unlock;
			break;
		case lock:
			unlock_state = lock;
		default:
			unlock_state = init;
			break;
	}
	switch(unlock_state) {
		case init:
			break;
		case pound:
			break;
		case one:
			break;
		case two:
			break;
		case three:
			break;
		case four:
			break;
		case unlock:
			PORTB = 0x01;
			break;
		case lock:
			PORTB = 0x00;
		default:
			break;
	}
	return unlock_state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;	PORTB = 0x00;
	DDRC = 0xF0;	PORTC = 0x0F;
    /* Insert your solution below */

	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	const char start = -1;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct =  &unlockTick;
	task2.state = start;  //or can do the initial start state
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.TickFct =  &unlockTick;
	
	unsigned long GCD = tasks[0]->period;
	for(unsigned short i = 1; i < numTasks; i++)
	{
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
    return 0;
}
