/*
 * Aufgabe2.c
 *
 * Created: 11.10.2012 17:16:10
 *  Author: Chrisa
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>


enum mode {		bereitschaft,
				wartenAufSpieler,
				folgeAbspielen};
				
enum readyLED {	left,
				up,
				right,
				down};
				
volatile enum mode aktiverMode = bereitschaft;
enum readyLED readyTurn = left;
#define TASTERLINKS PINA & (1<<PINA1)
#define TASTEROBEN PINA & (1<<PINA0)
#define TASTERRECHTS PINA & (1<<PINA2)
#define TASTERUNTEN PINA & (1<<PINA3)
#define TASTERMENU PINA & (1<<PINA4)

uint8_t links = 0;
uint8_t rechts = 0;
uint8_t oben = 0;
uint8_t unten = 0;
uint8_t intervalCounter = 0;


/*
uint8_t ledLinks = (1<<PORTA6);
uint8_t ledRechts = (1<<PORTC6);
uint8_t ledOben = (1<<PORTB5);
uint8_t ledUnten = (1<<PORTB7);
*/

inline void LEDOnLinks() {
	PORTA |= (1<<PORTA6);
}

inline void LEDOffLinks() {
	PORTA |= (0<<PORTA6);
}

inline void LEDOnRechts(){
	 PORTC |= (1<<PORTC6);
}	 

inline void LEDOffRechts(){
	PORTC |= (0<<PORTC6);
}

inline void LEDOnOben() {
	PORTB |= (1<<PORTB5);
}

inline void LEDOffOben() {
	PORTB |= (0<<PORTB5);
}

inline void LEDOnUnten() {
	PORTB |= (1<<PORTB7);
}

inline void LEDOffUnten() {
	PORTB |= (0<<PORTB7);
}

inline void LEDLeftOnly() {
	LEDOnLinks();
	LEDOffRechts();
	LEDOffOben();
	LEDOffUnten();
}

inline void LEDObenOnly() {
	LEDOffLinks();
	LEDOffRechts();
	LEDOnOben();
	LEDOffUnten();
}

inline void LEDRechtsOnly() {
	LEDOffLinks();
	LEDOnRechts();
	LEDOffOben();
	LEDOffUnten();
}

inline void LEDUntenOnly() {
	LEDOffLinks();
	LEDOffRechts();
	LEDOffOben();
	LEDOnUnten();
}


void zeigeBereitschaft() {
	//Interrupts deaktivieren
	cli();
	TCNT0 = 0x00; //Timer zurücksetzen
	TCCR0B = (1<<CS02) | (1<<CS00) ;	//Prescaler auf 1024;~32ms bis Interrupt;
	TIMSK0 = (1<<TOIE0);  //Overflow Interrupt aktivieren
	sei();
	
}

void startWaitingTimer() {
	cli();
	TCCR1A = 0x00;
	TCCR1B = (1<<WGM12);//OCR1A
	TCNT1 = (uint16_t) (0x0000);
	OCR1A = 39063;//(8000000 / 1024)*5   genau 5 Sekunden
	TCCR1B = (1<<CS02) | (1<<CS00);	//Prescaler auf 1024
	TIMSK1 = (1<<OCIE1A);
	//Interrupts aktivieren
	sei();
	
}

void startLEDBereitschaft() {
	cli();
	TCNT0 = 0x00; //Timer zurücksetzen
	TCCR0B = (1<<CS02) | (1<<CS00) ;	//Prescaler auf 1024;~32ms bis Interrupt;
	TIMSK0 = (1<<TOIE0);  //Overflow Interrupt aktivieren
	sei();
}

void stopWaitingTimer() {
	TCCR1B = 0x00;
}

//Timer für die Anzeige der LEDs
ISR(TIMER0_OVF_vect) {
	if (aktiverMode==bereitschaft) {	//Wenn im bereitschaftsmodus, dann alle ~100 ms die LED wechseln
		if (intervalCounter==3) {
			intervalCounter = 0;
			switch (readyTurn) {
				case left:
					readyTurn = up;
					LEDLeftOnly();
				break;
				case up:
					readyTurn = right;
					LEDObenOnly();
				break;
				case right:
					readyTurn = down;
					LEDRechtsOnly();
				break;
				case down:
					readyTurn = left;
					LEDUntenOnly();
				break;
			}//switch
		}//intervalCounter==3
	}//aktiverMode==bereitschaft
}

//Timer für Tastendrücke
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
	//Setting Out/Inputs
	DDRA = 0x20;//(0<<PORTA0) | (0<<PORTA1) | (0<<PORTA2) | (0<<PORTA3) | (0<<PORTA4) | (1<<PORTA6);// PortA 0-4 Eingänge für Tasten; 6 Ausgang für linke LED
	DDRB = 0xA0;//(1<<PORTB5) | (1<<PORTB7); 
	DDRC = (1<<PORTC6);
	//Tastaturtimer starten;läuft die ganze Zeit
	TCNT2 = 0x00; //Timer zurücksetzen
	TCCR2B = (1<<CS02) | (1<<CS00) ;	//Prescaler auf 1024;~32ms bis Interrupt;
	TIMSK2 = (1<<TOIE2);  //Overflow Interrupt aktivieren
	sei();
	//initialize all variables
	zeigeBereitschaft();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}