/*
 * main.c
 *
 *  Created on: 25 sie 2015
 *      Author: Kuba
 */

//STEROWANIE NATĘŻENIEM OŚWIETLENIA
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BAUD 19200
#include <util/setbaud.h>

// dokumentacja | 178
void USART_Init() {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

// dokumentacja | 179
void USART_Transmit( unsigned char data ) {
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

// dokumentacja | 182
unsigned char USART_Receive() {
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

int ReadChar(FILE* a_Unused) {
	return USART_Receive();
}

FILE USART_STREAM = FDEV_SETUP_STREAM(NULL, ReadChar, _FDEV_SETUP_READ);

int main(void) {
	char usart_buffer[20] = {0};

	DDRC = 0xFF;
	
	// przy tranzystorach jest zmiana!
	// 0 nie świeci | 1 świeci
	PORTC = 0x00;

	USART_Init();
	stdin = &USART_STREAM;
	
	// wersja prostsza, testowana przez Karola/Damiana
	// nasza wersja w komentarzu na samym dole
	// PD7 | BS170 | ten mały po lewej
	// PD6 | IRF520 | ten większy po prawej

	while(1)
	{
		fgets(usart_buffer, 19, stdin);
		
		if (!strcmp(usart_buffer, "strong\n"))
		{
			// logiczne 1 | świeci
			PORTC = 0xFF;
		}
		else if (!strcmp(usart_buffer, "weak\n"))
		{
			// blokada IRF520 | PD6
			// wychodzi na to, że słabiej świeci
			PORTC = 0xBF;
		}
		else if (!strcmp(usart_buffer, "turnoff\n"))
		{
			// logiczne 0 | nie świeci
			PORTC = 0x00;
		}
	}

	return 0;
}

/* 
// alternatywna wersja - działała!
// podpięcia na PC4 i PC5
while(1)
	{
		fgets(usart_buffer, 19, stdin);

		// nie wiem jak dokładnie wytłumaczyć to co jest w ifach
		// po prostu chyba jeżeli na 
		
		if (!strcmp(usart_buffer, "strong\n"))
		{
			// przekazujemy 0 z PC5
			// więc nie będzie tam sygnału
			PORTC &= ~(1 << PC5); 
			// przekazujemy 1 z PC4
			// więc będzie tam sygnał
			PORTC |= (1 << PC4);
		}
		else if (!strcmp(usart_buffer, "weak\n"))
		{
			// przekazujemy 0 z PC4
			// więc nie będzie tam sygnału
			PORTC &= ~(1 << PC4);
			// przekazujemy 1 z PC5
			// więc będzie tam sygnał
			PORTC |= (1 << PC5);
		}
		else if (!strcmp(usart_buffer, "turnoff\n"))
		{
			// kompletny brak sygnału
			// nie świeci | inaczej niż przy diodach!
			PORTC = 0x00;
		}
	}
*/