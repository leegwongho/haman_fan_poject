#include "motor.h"
#include "ultrasonic.h"
#include "uart.h"
#include "fan_lcd.h"

volatile uint8_t motor_on = 0;
volatile uint8_t speed = 0;
volatile uint8_t servo_on = 0;
volatile uint8_t servo_state = 0;
volatile uint8_t distance = 0;
volatile uint8_t count = 0;
volatile uint8_t UART_button_on_off = 0;
volatile uint8_t UART_button_Power = 0;
volatile uint8_t UART_button_rote = 0;


ISR(TIMER2_OVF_vect){
	rotate_servo();
	if(count < 31)
	count += 1;
	else{
		count = 0;
		triggerPin();
		distance = meanDistance();
		if(motor_on){
			if(distance < 10)
			OCR0 = OFF;
			else
			switch (speed) {
				case 0:
				OCR0 = LOW;
				break;
				case 1:
				OCR0 = MEDIUM;
				break;
				case 2:
				OCR0 = HIGH;
				break;
			}
		}
	}
}

// GPIO setup
void gpioInit() {
	DDRD &= ~(1 << Power);
	DDRD &= ~(1 << Speed);
	DDRD &= ~(1 << Spin);
	DDRB |= (1 << Fan);
	DDRB |= (1 << Servo); // PB5 as output (OC1A)
	DDRC |= (1 << LED_LOW) | (1 << LED_MED) | (1 << LED_HIGH);
}

// Timer setup
void timerInit() {
	TCCR0 |= (1 << WGM01) | (1 << WGM00);   // Fast PWM
	TCCR0 |= (1 << COM01);                  // non-inverting
	TCCR0 |= (1 << CS00) || (1 << CS01);                   // No prescaling
	// TIMSK |= (1 << TOIE0);
	OCR0 = 0;

	// Timer1 for Servo
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS11);
	ICR1 = 39999; // 50 Hz
}

// Interrupt setup
void initInterrupts() {
	// Set up INT0, INT1, INT2 interrupts
	EICRA |= (1 << ISC01) | (1 << ISC11);  // Falling edge trigger for INT0, INT1
	EICRA |= (1 << ISC21);                 // Falling edge trigger for INT2
	EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2); // Enable INT0, INT1, INT2
	sei();                                 // Enable global interrupts
}

// Power toggle
void power_on() {
	motor_on = !motor_on;
	if (motor_on) {
		OCR0 = LOW;
		PORTC |= (1 << LED_LOW);
		PORTC &= ~((1 << LED_MED) | (1 << LED_HIGH));
		strcpy(state1, "ON ");
		strcpy(state2, " 1");
	}
	else {
		OCR0 = OFF;
		PORTC = 0x00;
		strcpy(state1, "OFF");
		strcpy(state2, " 0");
		strcpy(state3, "OFF");
	}
	updateLCD(state1, state2, state3);
}

// Speed toggle
void speed_up() {
	speed = (speed + 1) % 3;
	if (motor_on) {
		switch (speed) {
			case 0:
			OCR0 = LOW;
			PORTC |= (1 << LED_LOW);
			PORTC &= ~((1 << LED_MED) | (1 << LED_HIGH));
			strcpy(state2, " 1");
			break;
			case 1:
			OCR0 = MEDIUM;
			PORTC |= (1 << LED_MED) | (1 << LED_LOW);
			PORTC &= ~(1 << LED_HIGH);
			strcpy(state2, " 2");
			break;
			case 2:
			OCR0 = HIGH;
			PORTC |= (1 << LED_HIGH) | (1 << LED_MED) | (1 << LED_LOW);
			strcpy(state2, " 3");
			break;
		}
		updateLCD(state1, state2, state3);
	}
}

// Servo motor control
void rotate_servo() {
	if(!motor_on)
	servo_on = 0;
	if (motor_on && servo_on) {
		if (!servo_state) {
			OCR1A += 1;
			if (OCR1A > 4500) {
				servo_state = 1;
			}
		}
		else if (servo_state) {
			OCR1A -= 1;
			if (OCR1A < 1500) {
				servo_state = 0;
			}
		}
	}
}


ISR(INT0_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Power))) {
		power_on(); // Toggle DC motor on/off
	
		UART_button_on_off++;
		
		if (UART_button_on_off == 1)
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

	
}

ISR(INT1_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Speed))) {
		speed_up(); // Increase speed
		
	UART_button_Power++;
			
		if (UART_button_on_off == 1)
			{
				char* power_messages[] = {"Power 1\n", "Power 2\n", "Power 3\n"};
				int flag[] = {LOW, MEDIUM, HIGH};
				
				for (uint8_t i = 0; i < 3; i++)
				{
					if (UART_button_Power == (i + 1))
					{
						UART0_str(power_messages[i]);
						OCR0 = flag[i];
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

}

ISR(INT2_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Spin))) {
		if(motor_on)
		servo_on = !servo_on; // Toggle servo motor on/off
		
		if(servo_on)
		strcpy(state3, "ON");
		else strcpy(state3, "OFF");
		
		UART_button_rote++;
			
			if(UART_button_on_off == 1)
			{
				char* Rotation_messages[] = {"Rotation ON\n", "Rotation OFF\n"};
				
				for (uint8_t i = 0; i < 2; i++)
				{
					if ((UART_button_rote == (i + 1)))
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
	updateLCD(state1, state2, state3);
	

	
	
	
}