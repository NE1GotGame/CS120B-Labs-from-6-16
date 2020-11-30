/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Video: https://youtu.be/iuaQlOQ9mlg
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

unsigned char blinkLED = 0x00;  //The last LED that will be blinking

void tickBlink() {
	switch (BlinkingLEDSM) {
		case Blink1:
			BlinkingLEDSM = Blink2;
			break;
		case Blink2:
			BlinkingLEDSM = Blink1;
			break;
		default:
			BlinkingLEDSM = Blink1;
			break; 
	}
	switch (BlinkingLEDSM) {
		case Blink1:
			blinkLED = 0x08;
			break;
		case Blink2:
			blinkLED = 0x00;
			break;
		default:
			break;
	}
}

enum SMcombine{combine1, combine2}CombineLEDsSM;

void tickCombine() {
	switch (CombineLEDsSM) {
		case combine1:
			CombineLEDsSM = combine2;
			break;
		case combine2:
			CombineLEDsSM = combine1;
			break;
		default:
			CombineLEDsSM = combine1;
			break;	
	}
	switch (CombineLEDsSM) {
		case combine1:
			PORTB = LEDs | blinkLED;
			break;
		case combine2:
			PORTB = LEDs | blinkLED;
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	//DDRA = 0x00;	PORTA = 0xFF;	
	DDRB = 0xFF;	PORTB = 0x00;
	
	unsigned long LEDs_Period = 300;
	unsigned long blink_Period = 1000;

	TimerOn();
	TimerSet(2);

    /* Insert your solution below */
    while (1) {
	if (LEDs_Period >= 300) {
		tick3LED();
		LEDs_Period = 0;
	}
	if (blink_Period >= 1000) {
		tickBlink();
		blink_Period = 0;
	}
	tickCombine();
	while(!TimerFlag) {};
	TimerFlag = 0;

	LEDs_Period += 2;
	blink_Period += 2;
    }
    return 1;
}

