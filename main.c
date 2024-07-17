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
	

	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	while (1) {
		// Rotate servo motor
		rotate_servo();
		// Light up LEDs based on DC motor speed
		LED();
		
		
		if (receive_buff)
		{
			UART0_char(receive_buff);
			receive_buff = 0;
		}
	}
}



