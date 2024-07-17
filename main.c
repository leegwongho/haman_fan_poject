#include "motor.h"
#include "uart.h"



char receive_buff = 0;

ISR(USART0_RX_vect)
{
	receive_buff = UDR0;
}




int main(void) {
	// Initialization
	gpioInit();
	timerInit();
	initInterrupts();
	
	UART0_Init();
	button_port();
		
	DDRC = 0xff;
		
	uint8_t UART_button_on_off = 0;
	uint8_t UART_button_Power = 0;
	uint8_t UART_button_rote = 0;
		
	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	while (1) {
		// Rotate servo motor
		rotate_servo();
		// Light up LEDs based on DC motor speed
		LED();
		if(!(PIND & (1<<DDRD0)))
				{
					UART_button_on_off++;
					
					if(UART_button_on_off == 1)
					{
						UART_button_Power++;
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



