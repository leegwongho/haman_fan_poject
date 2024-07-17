#define F_CPU 16000000UL  

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

char receive_buff = 0;

ISR(USART0_RX_vect)
{
	receive_buff = UDR0;
}

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

int main(void)
{
	UART0_Init();
	button_port();
	
	DDRC = 0xff;
	
	uint8_t UART_button_on_off = 0;
	uint8_t UART_button_Power = 0;
	uint8_t UART_button_rote = 0;
	
	while(1)
	{
		if(!(PIND & (1<<DDRD0)))
		{
			UART_button_on_off++;
			
			if(UART_button_on_off == 1)
			{
				UART0_str("Power ON\n");
				_delay_ms(300);
			}
			
			else if(UART_button_on_off == 2)
			{
				UART0_str("Power OFF\n");
				_delay_ms(300);
				UART_button_on_off = 0;
				UART_button_Power = 0;
				UART_button_rote = 0;
			}
		}
		
		else if (!(PIND & (1<<DDRD1)))
		{
			UART_button_Power++;
			
			if (UART_button_on_off == 1)
			{
				char* power_messages[] = {"Power 1\n", "Power 2\n", "Power 3\n"};
				
				for (uint8_t i = 0; i < 3; i++)
				{
					if (UART_button_Power == (i + 1))
					{
						UART0_str(power_messages[i]);
						_delay_ms(300);
						
						if (i == 2)
						{
							UART_button_Power = 0;
						}
						break;
					}
				}
			}
		}

		else if (!(PIND & (1<<DDRD2)))
		{
			UART_button_rote++;
			
			if(UART_button_on_off == 1)
			{
				char* Rotation_messages[] = {"Rotation ON\n", "Rotation OFF\n"};
				
				for (uint8_t i = 0; i < 2; i++)
				{
					if (UART_button_rote == (i + 1))
					{
						UART0_str(Rotation_messages[i]);
						_delay_ms(300);
						
						if (i == 1)
						{
							UART_button_rote = 0;
						}
						break;
					}
				}
			}
		}
		
		if (receive_buff)
		{
			UART0_char(receive_buff);
			receive_buff = 0;
		}
	}
}