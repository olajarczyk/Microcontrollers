/*
 * main.c
 *
 *  Created on: 16 kwi 2016
 *      Author: Krystian
 */
 
//WINDA LCD-KLAWIATURA
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "HD44780.h"
#include "PS2_Keyboard_Lookup.h"

bool jazda = false;
uint8_t pietro = 0;
uint8_t aktpietro = 0;
uint8_t czas = 0;
uint8_t krok = 0;

void PS2_Init()
{
	DDRD |= 0x00;
	PORTD |= 0xFF;

	// dokumentacja | 67
	EIMSK |= (1 << INT0);
	// dokumentacja | 68
	EICRA |= (1 << ISC01);
}

// trzeba się nauczyć!
ISR(INT0_vect)
{
	volatile static uint8_t bitcounter = 0;
	volatile static uint8_t data = 0;

	if(bitcounter == 0){
		data = 0;
	}
	else if(bitcounter > 0 && bitcounter < 9){
		if (PIND & (1 << PD3))
			data |= (1 << (bitcounter - 1));
	}
	else if (bitcounter == 9) {}
	else {
		if(jazda == false) {
			switch(data) {
				case 0x45: // 0
				{
						pietro = 0;
						czas = 0;
						jazda = true;
				}
				break;
				
				case 0x16: // 1
				{
						pietro = 1;
						czas = 0;
						jazda = true;
				}
				break;
				
				case 0x1E: // 2
				{
						pietro = 2;
						czas = 0;
						jazda = true;
				}
				break;
			}

		}
		bitcounter = -1;
	}
	bitcounter++;
}

// HD44780.c | ostatnia metoda | 220-225 wiersz
// wysyła znak do LCD
// a_Character | przekazany znak
// a_Unused | potrzebny do utworzenia strumieni STDOUT
int HD44780_WriteChar(char a_Character, FILE* a_Unused)
{
	// wysyła przekazany znak do LCD
    HD44780_WriteData(a_Character);
    return 0;
}

// HD44780.c | ostatni wiersz
// utworzenie strumienia, który wyświetla znaki na wyświetlaczu
// [arg 1] | umieszczenie znaku na LCD
// [arg 2] | pobranie znaku z LCD
// [arg 3] | ustawienie flagi
FILE HD44780_STREAM = FDEV_SETUP_STREAM(HD44780_WriteChar, NULL, _FDEV_SETUP_WRITE);

// trzeba się na nauczyć!
int main(void)
{
	// HD44780.c | jakby ktoś chciał na siłę | metoda 144 wiersz (parametr 177 wiersz)
	// inicjalizacja wyświetlacza LCD
	HD44780_Init(HD44780_INIT_DEFAULT);
	
	// ustawienie standardowego wyjścia na strumień LCD
	stdout = &HD44780_STREAM;

	// wyczyszczenie wyświetlacza
	HD44780_Clear();
	// ustawienie wyświetlacza na starcie
	HD44780_Home();

	PS2_Init();

	sei();

	while(1)
	{
		if (aktpietro < pietro)
		{
			// pętla for | 2 przejścia, żeby zmieniać sekundy co 1
			// bo przejazd na każde piętro zajmuje 2 sekundy
			for(krok = 0; krok < 2; krok++)
			{
				// ustawienie pozycji kursora
				// x | numer wiersza
				// y | numer kolumny
				HD44780_GoTo(0,0);
				
				// %c | liczba całkowita, jest konwertowana na bajt o danej wartości
				// 0x7E odpowiada ~
				// 0x7F odpowiada DELETE
				// dlatego na LCD pokazują się odpowiednie znaki -> oraz <-
				printf_P(PSTR("P:%d, Akt:%d, T:%d"), pietro, aktpietro, czas);

				HD44780_GoTo(1,0);
				printf_P(PSTR("Jazda"));

				_delay_ms(1000);
				czas++;
			}

			krok = 0;
			aktpietro++;
		}
		else if (aktpietro > pietro)
		{
			for(krok = 0; krok < 2; krok++)
			{
				HD44780_GoTo(0,0);
				printf_P(PSTR("P:%d, Akt:%d, T:%d"), pietro, aktpietro, czas);

				HD44780_GoTo(1,0);
				printf_P(PSTR("Jazda"));

				_delay_ms(1000);
				czas++;
			}

			krok = 0;
			aktpietro--;
		}
		else
		{
			HD44780_GoTo(0,0);
			printf_P(PSTR("P:%d ,Akt:%d, T:%d"), pietro, aktpietro, czas);

			HD44780_GoTo(1,0);
			printf_P(PSTR("Stop!"));

			jazda = false;
		}
	}

	return 0;
}