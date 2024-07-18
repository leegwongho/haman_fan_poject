#include "uart.h"



void UART0_char(char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}


void UART0_Init()
{
	UBRR0H = 0;
	UBRR0L = 207;
	
	UCSR0A = (1<<U2X0);
	
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
}

void UART0_str(char *str)
{
	while(*str)
	UART0_char(*str++);
}

void button_port()
{
	DDRD &= (~1<<DDRD0);
	PORTD |= (1<<DDRD0);
	
	DDRD &= ~(1<<DDRD1);
	PORTD |= (1<<DDRD1);
	
	DDRD &= ~(1<<DDRD2);
	PORTD |= (1<<DDRD2);
}


