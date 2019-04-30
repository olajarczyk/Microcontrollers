/*
 * main.c
 *
 *  Created on: 15 kwi 2016
 *      Author: Krystian
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "PS2_Keyboard_Lookup.h"

void PS2_Init() {
	// wejściowy
	DDRD = 0x00;
	PORTD = 0xFF;

	// dokumentacja | 67
	EICRA |= (1 << ICS00);
	// dokumentacja | 68
	EIMSK |= (1 << INT0);
}

// trzeba się nauczyć!
ISR(INT0_vect) {
	volatile static uint8_t bitcounter = 0;
	volatile static uint8_t data = 0;
	
	if (bitcounter == 0) {
		data = 0;
	} 
	else if (bitcounter > 0 && bitcounter < 9) {
		if (PIND & (1 << PD3)) {
			data |= (1 << (bitcounter - 1));
		}
	} 
	else if (bitcounter == 9) {} 
	else {
		// PBn - il_bitów - co?
		// PB0 - 1 - przód
		// PB1 - 2 - tył
		// PB2 - 4 - lewo
		// PB3 - 8 - prawo
		// PB4 - 1 - stop
		
		// PB - bity
		// 7654 3210
		
		// dla 0 dioda świeci!!!
		
		// |= nie mogą być
		// np. przy skręcie w lewo (A)
		// nie mogą być PB3 i PB4
		// i w te miejsca wstawiamy 1
		// PORTB |= 0x18 (0001 1000)
		
		// &= musi być
		// np. przy skręcie w lewo (A)
		// musi być PB2
		// i w te miejsca wstawiamy 0
		// PORTB &= 0xFB (1111 1011)
		
		switch (data) {
			case 0x1C: // A, lewo
			{
				PORTB |= 0x18;
				PORTB &= 0xFB;
			}
				break;
			case 0x23: // D, prawo
			{
				PORTB |= 0x14;
				PORTB &= 0xF7;
			}
				break;
			case 0x1D: // W, przód
			{
				PORTB |= 0x1E;
				PORTB &= 0xFE;
			}
				break;
			case 0x1B: // S, tył
			{
				PORTB |= 0x1D;
				PORTB &= 0xFD;
			}
				break;
			case 0x2B: // F, stop
			{
				PORTB = 0xEF;
			}
				break;
		}
		bitcounter = -1;
	}
	bitcounter++;
}

int main (void) {
	// wyjściowy
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	PS2_Init();
	sei();
	
	while (1)
		;
	
	return 0;
}