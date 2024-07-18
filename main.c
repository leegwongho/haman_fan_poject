
#include "motor.h"
#include "uart.h"
#include "ultrasonic.h"
#include "fan_lcd.h"
#include "timer.h"

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
	timerInit();
	timerInit_sonic();
	
	LCD_Init();
	LCD_WriteStringXY(0, 0, "PWR  SPD  SPN");
	updateLCD(state1, state2, state3);
	
	//ultrasonic_init();
	TCCR2 |= (1 << CS22) || (1 << CS21) || (1 << CS20);
	TIMSK |= (1 << TOIE2);
	
	DDRD |= (1 << PORTD6);  // Set trigger pin as output
	DDRD &= ~(1 << PORTD7); // Set echo pin as input
	
	DDRC |= (1 << PORTC7);
	
	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	while (1) {
		
		if(motor_on && timer_on && count == 31){
			timer += 1;
			if(timer == 100){
			timer = 0;
			if(motor_on){
				power_on(); timer_on = 0;
				PORTC &= ~(1 << PORTC7);
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