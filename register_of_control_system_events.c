#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#define BAUD 19200
#include <util/setbaud.h>

void USART_Init()
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data, FILE * unused)
{
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    UDR0 = data;
}

FILE USART_OUTPUT_STREAM = FDEV_SETUP_STREAM(USART_Transmit,NULL,_FDEV_SETUP_WRITE);

int main(void)
{
    USART_Init();
    stdout = &USART_OUTPUT_STREAM;
    DDRB = 0x00;
    PORTB = 0xFF;
    bool changeFlag = false;
    char prevPINB = 0xFF;
    char acctPINB;
    int l1 = 0;
    int l2 = 0;
    while(1)
    {
        acctPINB = PINB;
        if (prevPINB != acctPINB)
        {
            changeFlag = true;
        }

        if (changeFlag == true)
        {
        	char prev1 = prevPINB & 0x01;
			char prev2 = prevPINB & 0x02;
			char acct1 = acctPINB & 0x01;
			char acct2 = acctPINB & 0x02;
            changeFlag = false;
            if (prev1 != acct1 && acct1 == 0x00)
            {
                l1++;
                printf("Wlacznik 1 wlaczony %d raz\n", l1);
            }
            else if (prev1 != acct1 && acct1 == 0x01)
            {
                printf("Wlacznik 1 wylaczony %d raz\n", l1);
            }
            else if (prev2 != acct2 && acct2 == 0x00)
            {
                l2++;
                printf("Wlacznik 2 wlaczony %d raz\n", l2);
            }
            else if (prev2 != acct2 && acct2 == 0x02)
            {
                printf("Wlacznik 2 wylaczony %d raz\n", l2);
            }

        }
        prevPINB = acctPINB;
    }

    return 0;
}
