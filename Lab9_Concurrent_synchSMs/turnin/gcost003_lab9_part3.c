/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Video: https://youtu.be/aQRmQW-6HWQ
 */
#include <avr/io.h>
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
	
	unsigned long LEDs_Period = 300;
	unsigned long blink_Period = 1000;
	unsigned long period = 1;
	unsigned long speaker_Period = 2;

	TimerOn();
	TimerSet(period);

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
	if (speaker_Period >= 2) {
		tickSpeaker();
		speaker_Period = 0;
	} 

	tickCombine();
	while(!TimerFlag) {};
	TimerFlag = 0;

	LEDs_Period += period;    //increase timer by the period
	blink_Period += period;   //increase timer by the period
	speaker_Period += period;  //increase timer by the period
    } 
    return 1;
}
ttps://youtu.be/-9aLnkO9S-8
