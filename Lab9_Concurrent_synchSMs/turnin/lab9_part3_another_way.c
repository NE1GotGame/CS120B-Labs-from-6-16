
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
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct _Task {
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct) (int);
} Task;

const unsigned char tasksSize = 4;
Task tasks[4];
const unsigned char tasksPeriod = 1;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    unsigned char i;
    for(i = 0; i < tasksSize; i++) {
        if(tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriod;
    }
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum SM3LED{LED1, LED2, LED3}ThreeLEDsSM;

unsigned char LEDs = 0x00;      //this is only for the first 3 LED

void tick3LED() {
	switch (ThreeLEDsSM) {
		case LED1:
			ThreeLEDsSM = LED2;
			break;
		case LED2:
			ThreeLEDsSM = LED3;
			break;
		case LED3:
			ThreeLEDsSM = LED1;
			break;
		default:
			ThreeLEDsSM = LED1;
			break;
	}
	switch (ThreeLEDsSM) {
		case LED1:
			LEDs = 0x01;
			break;
		case LED2:
			LEDs = 0x02;
			break;
		case LED3:
			LEDs = 0x04;
			break;
		default:
			break;
	}
}

enum SMblinking{Blink1, Blink2}BlinkingLEDSM;

unsigned char blinkLED = 0x01;  //The last LED that will be blinking

void tickBlink() {
	switch (BlinkingLEDSM) {
		case Blink1:
			BlinkingLEDSM = Blink2;
			break;
		case Blink2:
			break;
		default:
			break; 
	}
	switch (BlinkingLEDSM) {
		case Blink1:
			break;
		case Blink2:
			blinkLED = !blinkLED;
			break;
		default:
			break;
	}
}

enum SMspeaker{init, OFF, ON} SpeakerState;

unsigned char speakerValue = 0;
unsigned char switchA2;

void tickSpeaker() {
	switchA2 = ~PINA & 0x04;
	switch (SpeakerState) {
		case init:
			SpeakerState = OFF;
			break;
		case OFF:
			if(switchA2 == 0x04) {	   //if switch is on then...
				SpeakerState = ON;
			}
			break;
		case ON:
			if(switchA2 != 0x00) {	   //if switch is off then... speaker turn off
				SpeakerState = OFF;
			}
			break;
		default:
            		SpeakerState = init;
            		break;
	}
	switch (SpeakerState) {
		case init:
			break;
		case OFF:
			speakerValue = 0x00;  //turn off PB4 while keeping the LED going
			break;
		case ON:
			speakerValue = 0x01;  //turn on PB4 while keeping the LED going
			break;
		default:
			break;
	}
}

enum SMcombine{combine1}CombineLEDsSM;

void tickCombine() {
	switch (CombineLEDsSM) {
		case combine1:
			break;
		default:
			break;	
	}
	switch (CombineLEDsSM) {
		case combine1:
			PORTB = LEDs | (blinkLED << 3) | (speakerValue << 4);
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;	
	DDRB = 0xFF;	PORTB = 0x00;
	
	unsigned char i = 0;
    	tasks[i].state = ThreeLEDsSM;
   	tasks[i].period = 300;
   	tasks[i].elapsedTime = tasks[i].period;
    	tasks[i].TickFct = &tick3LED;
    	i++;    
    	tasks[i].state = BlinkingLEDSM;
    	tasks[i].period = 1000;
    	tasks[i].elapsedTime = tasks[i].period;
    	tasks[i].TickFct = &tickBlink;
    	i++;
    	tasks[i].state = SpeakerState;
    	tasks[i].period = 2;
    	tasks[i].elapsedTime = tasks[i].period;
    	tasks[i].TickFct = &tickSpeaker;
    	i++;
    	tasks[i].state = CombineLEDsSM;
    	tasks[i].period = 1;
    	tasks[i].elapsedTime = tasks[i].period;
    	tasks[i].TickFct = &tickCombine;


	TimerOn();
	TimerSet(tasksPeriod);

    /* Insert your solution below */
    while (1) {
	//sleep();
    } 
    return 1;
}

