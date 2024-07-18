#include "motor.h"
#include "uart.h"
#include "fan_lcd.h"

char receive_buff = 0;

char state1[4] = "OFF";
char state2[4] = " 0";
char state3[6] = "OFF";



ISR(USART0_RX_vect)
{
	receive_buff = UDR0;
}



int main(void) {
	LCD_Init();
	LCD_WriteStringXY(0, 0, "PWR  SPD  SPN");
	updateLCD(state1, state2, state3);
	
	
	// Initialization
	gpioInit();
	timerInit();
	initInterrupts();
	
	UART0_Init();
	button_port();
	
	DDRC = 0xff;
	

	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	
	////////////////////////////////////////	
	
	
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



