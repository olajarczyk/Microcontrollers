/*
 * main.c
 *
 *  Created on: 15 kwi 2016
 *      Author: Krystian
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PS2_Keyboard_Lookup.h"

volatile static uint8_t odleglosc = 50;

void PS2_Init()
{
	// PORTD jako wyjściowy
	DDRD = 0x00;
	PORTD = 0xFF;
	
	// dokumentacja | 67
	EIMSK |= (1 << INT0);
	// dokumentacja | 68
	EICRA |= (1 << ISC01);
}

// trzeba się nauczyć
ISR(INT0_vect)
{
	// wyjściowy
	DDRC = 0xFF;
	PORTC = 0xFF;

	volatile static uint8_t bitcounter = 0;
	volatile static uint8_t data = 0;

	if(bitcounter == 0) {
		data = 0;
	}
	else if(bitcounter > 0 && bitcounter < 9) {
		if (PIND & (1 << PD3))
			data |= (1 << (bitcounter - 1));
	}
	else if (bitcounter == 9) {}
	else {
		switch(data)
		{
			case 0x1D: // przód
			{
				if(odleglosc > 0){
					odleglosc--;
				}

			}
				break;

			case 0x1B: // tył
			{
				if(odleglosc < 100){
					odleglosc++;
				}
			}
				break;
		}
		bitcounter = -1;
	}
	bitcounter++;
}


int main(void)
{
	PS2_Init();
	sei();

	while(1)
	{
		if(odleglosc > 5 && odleglosc < 25)
		{
			PORTC = 0xFE;
			_delay_ms(500);
			PORTC = 0xFF;
			_delay_ms(500);
		}
		else if(odleglosc <= 5)
		{
			PORTC = 0xFE;
		}
		else if (odleglosc > 75 && odleglosc < 95)
		{
			PORTC = 0xF1;
			_delay_ms(500);
			PORTC = 0xFF;
			_delay_ms(500);
		}
		else if (odleglosc >= 95)
		{
			PORTC = 0xF1;
		}
	}

	return 0;
}
