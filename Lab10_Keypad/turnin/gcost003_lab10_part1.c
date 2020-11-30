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
enum KeypadSM{keyword};

int keypadTick(int keypad_state)
{
	unsigned char key = 0x00; 
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

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;	PORTB = 0x00;
	DDRC = 0xF0;	PORTC = 0x0F;
    /* Insert your solution below */

	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	const char start = 0;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct =  &keypadTick;

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
