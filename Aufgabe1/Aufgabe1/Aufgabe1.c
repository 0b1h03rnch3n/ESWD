/*
 * Simpleblink
 * Demo1-1.c 
 *
 * Created: 01.04.2011 22:05:04
 *  Author: Ole
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "blinkmuster.h"
#define SIMULATOR 
#define interruptCntMin 3
#define interruptCntMax interruptCntMin*20
volatile uint8_t state = 0;
volatile uint8_t interruptInterval = interruptCntMin;	//volatile um die Variable nicht kaputt zu optimieren
volatile uint8_t interruptCounter = 0;
volatile uint8_t blinkmuster = 0;
uint8_t maxBlinks = 0;

uint8_t getLEDs(uint8_t selmuster, uint8_t state, uint8_t *countInPattern) {
	uint8_t i = 0;
	uint8_t j = 0;
	while (i!=selmuster) {
		j += muster[i];
		i++;
	}
	*countInPattern = muster[--i];
	return muster[j+state];
}


ISR(TIMER0_OVF_vect) {
	uint8_t countInPattern = 0;
	if (++interruptCounter == interruptInterval) {
		interruptCounter=0;
		//get the LED-Set and write it to PortB
		PORTB = getLEDs(blinkmuster,state, &countInPattern);
		state++;
		if (state>countInPattern) state=0;
/*		
		if (!state) {
			//PORTB ^= (1<<PB0);
			//PORTA |=(1<<0b00000001);
			//PORTA |=(1<<PORTA0);		// LED on
			PORTB &=~(1<<PORTB0);		// LED off
		}			
		else {
			PORTA &=~(1<<PORTA0);		// LED off
			PORTB |=(1<<PORTB0);		// LED on
		}			
		state=(state+1)%2;		// toggle internal state*/
	}
}


int main(void)
{
	// init
   	DDRA  = 0x0F;		// set lowest bit of PORTA Output
    PORTA = 0x00;		// all LEDs off
	DDRB  = 0x0F;
	PORTB = 0x00;
	DDRD = 0x00;		//Input an PortD
	
	uint8_t abort=0; 
	uint8_t freqDown = 0;
	uint8_t freqUp = 0;
	uint8_t changeMuster = 0;
		//Set CS to internal clock source
		//Normal mode (WGM02:0=0)
		//TCCR0A 
		//COM0A1:0;COM0A0:0
		//COM0B1:0;COM0B0:0
		//WGM1:0;WGM0:0;
		//TCCR0B:
		//WGM2:0;
		//FOC0A:0;FOC0B:0
		//CS02:1;CS01:0;CS00:1;
		//TIMSK0:
		//OCIE0B:0
		//OCIE0A:0
		//TOIE0:1
		//TIFR0:
		//OCF0B:0
		//OCF0A:0
		//TOV0:0;->Counter Overflow flag
	TCCR0B = (1<<CS02) | (1<<CS00) ;	//Prescaler auf 1024;~32ms bis Interrupt;
	TIMSK0 = (1<<TOIE0);  //Overflow Interrupt aktivieren
	//TIFR0 = (1<<TOV0);
	sei();
		
		
    do {

		freqDown = (PIND & (1<<PIND7));//PD7
		freqUp = (PIND & (1<<PIND6));//PD6
		changeMuster = (PIND & (1<<PIND5));//PD5
		if (freqDown && (interruptInterval > interruptCntMin)) interruptInterval--; 
		if (freqUp && (interruptInterval < interruptCntMax)) interruptInterval++;
		if (changeMuster) {
			//zum nächsten Muster wechseln
			blinkmuster = (blinkmuster + 1) % 4;
		}
		abort=(PINA & (1<<PINA4));	// check for Button "ENTER"
	} while (!abort);
		PORTA = 0x00;  // all LEDs off
		PORTB = 0x00;
	// Put a BREAKPOINT at the line below this comment
	return 0; 
}