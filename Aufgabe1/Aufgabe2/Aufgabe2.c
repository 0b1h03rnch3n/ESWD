/*
 * Aufgabe2.c
 *
 * Created: 11.10.2012 17:16:10
 *  Author: Chrisa
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
enum mode {	bereitschaft,
				wartenAufSpieler,
				folgeAbspielen};
enum mode aktiverMode = bereitschaft;


void zeigeBereitschaft() {
	//Interrupts deaktivieren
	cli();
	TCNT0 = 0x00; //Timer zurücksetzen
	TCCR0B = (1<<CS02) | (1<<CS00) ;	//Prescaler auf 1024;~32ms bis Interrupt;
	TIMSK0 = (1<<TOIE0);  //Overflow Interrupt aktivieren
	TCCR1A = 0x00;
	TCCR1B = (1<<WGM12);
	
	OCR1A = 39063;//(8000000 / 1024)*5
	TCCR1B = (1<<CS02) | (1<<CS00);
	//TIMSK1 = (1<<TOIE1);
	TIMSK1 = (1<<OCIE1A);
	//Interrupts aktivieren
	sei();
}

void startWaitingTimer() {
	
}

void stopWaitingTimer() {
	
}

ISR(TIMER0_OVF_vect) {
	
}
//Timer für die Anzeige der LEDs
ISR(TIMER2_OVF_vect) {
	
}

ISR (TIMER1_COMPA_vect) {
	PORTD = 0xFF;
}

ISR(TIMER1_OVF_vect) {
	PORTD = 0x0F;
}

int main(void)
{
	//initialize all variables
	zeigeBereitschaft();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}