
#include "motor.h"
#include "uart.h"
#include "ultrasonic.h"
#include "fan_lcd.h"

 char state1[4] = "OFF";
 char state2[4] = " 0";
 char state3[6] = "OFF";
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
	timerInit_sonic();
	UART0_Init();
	button_port();
	
	LCD_Init();
	LCD_WriteStringXY(0, 0, "PWR  SPD  SPN");
	updateLCD(state1, state2, state3);
	
	//ultrasonic_init();
	TCCR2 |= (1 << CS22) || (1 << CS21) || (1 << CS20);
	TIMSK |= (1 << TOIE2);
	
	DDRD |= (1 << PORTD6);  // Set trigger pin as output
	DDRD &= ~(1 << PORTD7); // Set echo pin as input
	

	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	while (1) {
		
		
		if (receive_buff)
		{
			UART0_char(receive_buff);
			receive_buff = 0;
		}
	}
}