/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Test: https://youtu.be/nHBghT6oPDM
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
void transmit_data2nd(unsigned char data) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
   	 PORTB = 0x08;
   	 // set SER = next bit of data to be sent.
   	 PORTB |= ((data >> i) & 0x01);
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 PORTB |= 0x02;  
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTB |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTB = 0x00;
}

unsigned char cnt = 0x00;
//unsigned char B1 = 0x00; //increase button on A0
//unsigned char B2 = 0x00; //decrease button on A1
unsigned char tempA = 0;
unsigned char cnt2 = 0x00;
//static unsigned char go = 0x00;
unsigned char BS2 = 0; //Button set 2

enum FestiveLight1stSM{ step1, step2, step3}state_a;
// 0xE0, 0x18, 0x07
void DisplayLight1stTick() {
	switch (state_a) {		
		case step1:
			state_a = step2;
			break;
		case step2:
			state_a = step3;
			break;
		case step3:
			state_a = step1;
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
	//transmit_data2(cnt);	
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
	transmit_data(cnt);
	//transmit_data2(cnt);
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
	transmit_data(cnt);
	//transmit_data2(cnt);
}

enum LEDLight1stSM{ step1s, step2s, step3s}state_d;
// 0xE0, 0x18, 0x07
void Display1Tick() {
	switch (state_d) {		
		case step1s:
			state_d = step2s;
			break;
		case step2s:
			state_d = step3s;
			break;
		case step3s:
			state_d = step1s;
			break;
		default:
			break;
	} 
	switch(state_d) {
		case step1s:
			cnt2 = 0xE0;
			break;
		case step2s:
			cnt2 = 0x18;
			break;
		case step3s:
			cnt2 = 0x07;
			break;
		default:
			break;
	}
	//transmit_data(cnt);
	transmit_data2nd(cnt2);	
}


enum LEDLights2ndSM{Seqence1s, Seqence2s}state_e;

void LightTick2() {
	switch (state_e) {
		case Seqence1s:
			state_e = Seqence2s;
			cnt2 = 0xAA;		
			break;
		case Seqence2s:
			state_e = Seqence1s;
			cnt2 = 0x55;
			break;
		default:
			break;
	} 
	//transmit_data(cnt);
	transmit_data2nd(cnt2);
}


enum Lights3rdSM{Seq1ths, Seq2nds, Seq3rds, Seq4ths}state_f;

void seq3Tick() {
	switch (state_f) {
		case Seq1ths:
			state_f = Seq2nds;
			cnt2 = 0x81;		
			break;
		case Seq2nds:
			state_f = Seq3rds;
			cnt2 = 0x42;
			break;
		case Seq3rds:
			state_f = Seq4ths;
			cnt2 = 0x24;
			break;
		case Seq4ths:
			state_f = Seq1ths;
			cnt2 = 0x18;
			break;
		default:
			break;
	}
	//transmit_data(cnt);
	transmit_data2nd(cnt2);
}

enum FestiveLight1SM{OFF, ON, sequ1, sequ2, sequ3}state;
void incDecTick() {
	tempA = ~PINA & 0x03;
	//BS2 = ~PINA & 0x30;
	switch (state) {	
		case OFF:
			if (tempA == 0x03 ) {
				state = ON;
			}
			else { state = OFF; }
			break;
		case ON:
			state = sequ1;
			break;
		case sequ1:
			if(tempA == 0x01 ) {
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
			if (tempA == 0x01 ) {
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
			if(tempA == 0x02 ) {
				state = sequ2;
			}
			else if(tempA == 0x03 || BS2 == 0x03) {
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
	//transmit_data2(cnt);
	//return state;
}

enum upDown1SM{OFFs, ONs, sequ1s, sequ2s, sequ3s}state_g;
void upDownTick2() {
	//tempA = ~PINA & 0x03;
	BS2 = ~PINA & 0x0C;
	switch (state_g) {	
		case OFFs:
			if (BS2 == 0x0C ) {
				state_g = ONs;
			}
			else { state_g = OFFs; }
			break;
		case ONs:
			state_g = sequ1s;
			break;
		case sequ1s:
			if(BS2 == 0x04 ) {
				state_g = sequ2s;
			}
			else if (BS2 == 0x0C) {
				state_g = OFFs;
			}
			else {
				state_g = sequ1s;
			}
			break;
		case sequ2s:
			if (BS2 == 0x04 ) {
				state_g = sequ3s;
			}
			else if(BS2 == 0x08) {
				state_g = sequ1s;
			}
			else if (BS2 == 0x0C) {
				state_g = OFFs;
			}
			else {
				state_g = sequ2s;
			}		
			break;
		case sequ3s:
			if(BS2 == 0x08 ) {
				state_g = sequ2s;
			}
			else if(BS2 == 0x0C) {
				state_g = OFFs;
			}
			else {
				state_g = sequ3s;
			}
			break;
		default:
			state_g = OFFs;
			break;
	} 
	switch(state_g) {
		case OFFs:
			cnt2 = 0x00;
			//transmit_data(0); also can call the function to intitialize to be zero
			break;
		case ONs:
			break;
		case sequ1s:
			Display1Tick();
			break;
		case sequ2s:
			LightTick2();
			break;
		case sequ3s:
			seq3Tick();
			break;
		default:
			//state = ON;
			break; 
	}
	//go = cnt;
	//transmit_data(cnt);
	transmit_data2nd(cnt2);
	//return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;	
	DDRC = 0xFF;	PORTC = 0x00;
	DDRB = 0xFF;	PORTB = 0x00;
 	
	TimerSet(160);
	TimerOn();

    while (1) {

	incDecTick();
	upDownTick2();	
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
