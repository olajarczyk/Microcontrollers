/*
 * main.c
 *
 *  Created on: 15 kwi 2016
 *      Author: Krystian
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t TIMER_L;
uint8_t licznik = 0;
uint8_t etap = 0;

int main(void)
{
    // init przerwań
	// dokumentacja | 104
    TCCR0B = (1 << CS00);
	// dokumentacja | 106
    TIMSK0 = (1 << TOIE0);

	// cykliczne powtarzanie przerwań
    sei();

	// ust. portu D na wyjściowy
    DDRD = 0xFF;
	
	// ustawienie pinu PD4 na stan niski | 0
	// reszta pinów na stan wysoki | 1
	// 1110 1111
    PORTD = 0xEF;

	// nieskończona pętla, ale nic nie ma
    while(1)
    ;

    return 0;
}

// trzeba się nauczyć!
ISR(TIMER0_OVF_vect)
{
    TIMER_L++;
    if(TIMER_L >= 31250)
    {
        TIMER_L = 0;
		// licznik++, czyli dodaje kolejne sekundy
        licznik++;
        zmiana();
    }
}

void zmiana()
{
	// PDn - il_bitów - co?
	// PD7 - 8 - strzałka
	// PD6 - 4 - zielone
	// PD5 - 2 - żółte
	// PD4 - 1 - czerwone
	
	// PD - bity
	// 7654 3210
	
    if(etap == 0)
    {
        if(licznik == 1)
        {
            PORTD = 0x6F;
        }
        if(licznik == 7)
        {
            PORTD = 0xEF;
        }
        if(licznik == 8)
        {
            PORTD = 0xCF;
        }
        if(licznik == 9)
        {
            PORTD = 0xBF;
			licznik = 0;
			etap = 1;
        }
    }
	 if(etap == 1)
	 {
		 if(licznik == 10)
		 {
			 PORTD = 0xDF;
			 licznik = 0;
			 etap = 2;
		 }
	 }
    if(etap == 2)
    {
        if(licznik == 1)
        {
            PORTD = 0xEF;
            licznik = 0;
            etap = 0;
        }
    }

}
