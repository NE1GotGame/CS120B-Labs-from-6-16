#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
 
#include "scheduler.h"
#include "timer.h"
 
//Global Variables
 
static unsigned char row[3] = {0xFD, 0xFB, 0xF7};
static unsigned char pattern[3]= {0x3C, 0x24, 0x3C};

static unsigned char cnt = 0x00; 
unsigned char tempC;
unsigned char tempD;
//static unsigned char pattern2; 
 
//static unsigned char row2; 
unsigned char tmpC;  //pattern
unsigned char tmpD;  //rows   
unsigned char c, r;
 
enum Demo_States {states};
int Demo_Tick(int state) {
 
                // Local Variables
                //B1 = ~PINA & 0x01;
                //B2 = ~PINA & 0x02;
                //tempA = ~PINA & 0x00;              
                                
                //static unsigned char pattern; = 0x3C;   LED pattern - 0: LED off; 1: LED on
                //static unsigned char row;     = 0xE1;       Row(s) displaying pattern. 
                                                                                                                // 0: display pattern on row
                                                                                                                // 1: do NOT display pattern on row
               // Transitions
		switch (state) {
			case states:
			default:
				state = states;
				break;
	}	
	// Action
	switch (state) {
			case states:
				if(cnt < 3) {
					tempC = pattern[cnt];
					tempD = row[cnt];
					cnt++;
				}
				else if (cnt == 3) {
					cnt = 0;
				}
				break;
			default:
				break;
		}            
         
                PORTC = tempC;         // Pattern to display
                PORTD = tempD;                        // Row(s) displaying pattern        
                return state;
}
 
 
static unsigned char tempA = 0x00;
 
enum MovementSM{wait, up, shiftUp, down, shiftDown, right, shiftRight, left, shiftLeft};
int MovenmentTick(int state) {
 
                // Local Variables
                tempA = ~PINA & 0x0F;  
                                
                //static unsigned char pattern; = 0x3C;   LED pattern - 0: LED off; 1: LED on
                //static unsigned char row;     = 0xE1;       Row(s) displaying pattern. 
                                                                                                                // 0: display pattern on row
                                                                                                                // 1: do NOT display pattern on row
                // Transitions
                switch (state) {
                                case wait:                            
                                                if (!tempA) {
                                                        state = wait;
                                                }
                                                else if(tempA == 0x01) {
                                                        state = up;
                                                }
                                                else if(tempA == 0x02) {
                                                        state = down;
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
                                case up:
                                                state = shiftUp;
                                                break;
                                case shiftUp:
                                                if(tempA == 0x01) {
                                                        state = up;
                                                }
                                                else if (tempA == 0x02) {
                                                        state = down;
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
                                case down:
                                                state = shiftDown;
                                                break;
                                case shiftDown:
                                                if(tempA == 0x02) {
                                                        state = down;
                                                } 
                                                else if (tempA == 0x01) {
                                                        state = up;
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
						else if(tempA == 0x01) {
                                                        state = up;
                                                }
                                                else if(tempA == 0x02) {
                                                        state = down;
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
						else if(tempA == 0x01) {
                                                        state = up;
                                                }
                                                else if(tempA == 0x02) {
                                                        state = down;
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
                                case up:
					
                                                if(row[0] != 0xFE) {  //move LED up the next row
							for(int i=0; i <3; i++) {
								row[i] = (row[i] >> 1) |  0x80;
							}
                                                }
                                                break;
                                case shiftUp:
                                                break;
                                case down:
                                                if(row[2] != 0xEF) {  //move LED up the next row
							for(int i=0; i <3; i++) {
								row[i] = (row[i] << 1) |  0x01;
							}
						}
                                                break;
                                case shiftDown:
                                                break;
				case right:		
						if(pattern[2] != 0x0F) {  //move LED up the next row
							for (int i = 0; i < 3; i++) {
								pattern[i] = (pattern[i] >> 1) & 0x7F;
							}			
						}					
						break;
				case shiftRight:
						break;
				case left:
						if (pattern[2] != 0xF0) { 
							for(int i = 0; i < 3; i++) {
								pattern[i] = (pattern[i] << 1) & 0xFE;
							}
						}
						break;
				case shiftLeft:
						break;
                                default:
                                        	break;
                }
                //c = pattern2;
                //r = row2;              
                //PORTC = pattern2;           // Pattern to display
                //PORTD = row2;                           // Row(s) displaying pattern        
                return state;
}
 
 
int main(void) {
    /* Insert DDR and PORT initializations */
                DDRA = 0x00;     PORTA = 0xFF;
                DDRC = 0xFF;     PORTC = 0x00;   //output for pattern
                DDRD = 0xFF;     PORTD = 0x00;   //output for row
                
                static task task1, task2;
                task *tasks[] = {&task1, &task2};
                const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
                
                const char start = 0;
                task1.state = start;
                task1.period = 100;
                task1.elapsedTime = task1.period;
                task1.TickFct =  &MovenmentTick;
 
                task2.state = start;
                task2.period = 1;
                task2.elapsedTime = task2.period;
                task2.TickFct =  &Demo_Tick;
 
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

