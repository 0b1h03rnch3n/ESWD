/*
 * blinkmuster.h
 *
 * Created: 10.10.2012 12:56:44
 *  Author: Chrisa
 */ 


#ifndef BLINKMUSTER_H_
#define BLINKMUSTER_H_
#include <avr/io.h>
#define anzahlMuster 4

struct portAusgabe {
	uint8_t port0 : 1;
	uint8_t port1 : 1;
	uint8_t port2 : 1;
	uint8_t port3 : 1;
	uint8_t port4 : 1;
	uint8_t port5 : 1;
	uint8_t port6 : 1;
	uint8_t port7 : 1;
	};	
extern uint8_t muster [];
//extern uint8_t **muster2;


#endif /* BLINKMUSTER_H_ */