/*
 * main.c
 *
 *  Created on: 15 kwi 2016
 *      Author: Krystian
 */

// trzeba się nauczyć bibliotek!
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define BAUD 19200
#include <util/setbaud.h>

// dokumentacja | 178
void USART_Init() {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

// dokumentacja | 182
unsigned char USART_Receive() {
	while ( !(UCSR0A & (1 << RXC0)) );
	return UDR0;
}

// to w nawiasie chyba nie musi być
int ReadChar (FILE* a_Unused) {
	return USART_Receive();
}

// trzeba się nauczyć!
FILE USART_STREAM = FDEV_SETUP_STREAM (NULL, ReadChar, _FDEV_SETUP_READ);

int main (void) {
	char usart_buffer[20] = {0};
	uint8_t licznik;

	// wyjściowy
	DDRB = 0xFF;
	// wszystkie diody zgaszone
	PORTB = 0x00;

	USART_Init();

	stdin = &USART_STREAM;

	// PBn - il_bitów - co
	// PB0 - 1 - czerwona
	// PB1 - 2 - czerwona
	// PB2 - 4 - czerwona
	// PB3 - 8 - zielona

	while (1) {
		fgets(usart_buffer, 19, stdin);

		if (licznik < 3) {
			if (!strcmp(usart_buffer, "pass\n")) {
				PORTB |= 0x08; // (1 << PD4)
				licznik = 3;
			} else {
				licznik++;

				switch (licznik) {
					case 1:
						PORTB |= 0x01; // (1 << PD0)
						break;
					case 2:
						PORTB |= 0x02; // (1 << PD1)
						break;
					case 3:
						PORTB |= 0x04; // (1 << PD2)
						break;
				}
			}
		}
	}

	return 0;
}

