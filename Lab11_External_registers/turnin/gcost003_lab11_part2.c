/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  Demo Test: https://youtu.be/aEGAojLrkpg
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
//unsigned char B1 = 0x00; //increase button on A0
//unsigned char B2 = 0x00; //decrease button on A1
unsigned char tempA = 0;
//static unsigned char go = 0x00;
unsigned char i, tmpC = 0;

enum FestiveLight1stSM{ step1, step2, step3}state_a;
// 0xE0, 0x18, 0x07
void DisplayLight1stTick() {
	switch (state_a) {		
		case step1:
			state_a = step2;
			//tmpC = 0xE0;
			break;
		case step2:
			state_a = step3;
			//tmpC = 0x18;
			break;
		case step3:
			state_a = step1;
			//tmpC = 0x07;
			break;
		default:
			break;
	} 
	switch(state_a) {
		case step1:
			cnt = 0xE0;
			break;
		case step2:
			cnt = 0x18;
			break;
		case step3:
			cnt = 0x07;
			break;
		default:
			break;
	}
	transmit_data(cnt);	
	//return state;
}


enum FestiveLight2ndSM{Seqence1, Seqence2}state_b;

void DisplayLight2ndTick() {
	switch (state_b) {
		case Seqence1:
			state_b = Seqence2;
			cnt = 0xAA;		
			break;
		case Seqence2:
			state_b = Seqence1;
			cnt = 0x55;
			break;
		default:
			break;
	} 
	/*switch(state_b) {
		case Seqence1:
			tmpC = 0xAA;
			break;
		case Seqence2:
			tmpC = 0x55;
			break;
		default:
			break;
	}*/
	transmit_data(cnt);
}

enum FestiveLight3rdSM{Seq1th, Seq2nd, Seq3rd, Seq4th}state_c;

void DisplayLight3rdTick() {
	switch (state_c) {
		case Seq1th:
			state_c = Seq2nd;
			cnt = 0x81;		
			break;
		case Seq2nd:
			state_c = Seq3rd;
			cnt = 0x42;
			break;
		case Seq3rd:
			state_c = Seq4th;
			cnt = 0x24;
			break;
		case Seq4th:
			state_c = Seq1th;
			cnt = 0x18;
			break;
		default:
			break;
	}
	/*switch(state_c) {
		case Seq1th:
			tmpC = 0x81;
			break;
		case Seq2nd:
			tmpC = 0x42;
			break;
		case Seq3rd:
			tmpC = 0x24;
			break;
		case Seq4th:
			tmpC = 0x18;
		default:
			break;
	}*/
	transmit_data(cnt);
}


enum FestiveLight1SM{OFF, ON, sequ1, sequ2, sequ3}state;
void incDecTick() {
	tempA = ~PINA & 0x03;
	switch (state) {	
		case OFF:
			if (tempA == 0x03) {
				state = ON;
			}
			else { state = OFF; }
			break;
		case ON:
			state = sequ1;
			break;
		case sequ1:
			if(tempA == 0x01) {
				state = sequ2;
			}
			else if (tempA == 0x03) {
				state = OFF;
			}
			else {
				state = sequ1;
			}
			break;
		case sequ2:
			if (tempA == 0x01) {
				state = sequ3;
			}
			else if(tempA == 0x02) {
				state = sequ1;
			}
			else if (tempA == 0x03) {
				state = OFF;
			}
			else {
				state = sequ2;
			}		
			break;
		case sequ3:
			if(tempA == 0x02) {
				state = sequ2;
			}
			else if(tempA == 0x03) {
				state = OFF;
			}
			else {
				state = sequ3;
			}
			break;
		default:
			state = OFF;
			break;
	} 
	switch(state) {
		case OFF:
			cnt = 0x00;
			//transmit_data(0); also can call the function to intitialize to be zero
			break;
		case ON:
			break;
		case sequ1:
			DisplayLight1stTick();
			break;
		case sequ2:
			DisplayLight2ndTick();
			break;
		case sequ3:
			DisplayLight3rdTick();
			break;
		default:
			//state = ON;
			break; 
	}
	//go = cnt;
	transmit_data(cnt);
	//return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;	
	DDRC = 0xFF;	PORTC = 0x00;

 	
	TimerSet(150);
	TimerOn();

    while (1) {

	incDecTick();	
	//DisplayLight1stTick();
	//DisplayLight2ndTick();
	//DisplayLight3rdTick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
