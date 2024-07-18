<<<<<<< HEAD
#include "motor.h"
#include "uart.h"


volatile uint8_t motor_on = 0;
volatile uint8_t speed = 0;
volatile uint8_t servo_on = 0;
volatile uint8_t servo_state = 0;
volatile uint8_t UART_button_on_off = 0;
volatile uint8_t UART_button_Power = 0;
volatile uint8_t UART_button_rote = 0;




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
	TCCR0 |= (1 << CS00);                   // No prescaling
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
	}
	else {
		OCR0 = OFF;
	}
}

// Speed toggle
void speed_up() {
	speed = (speed + 1) % 3;
	if (motor_on) {
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

// Servo motor control
void rotate_servo() {
	if (OCR1A > 4500) {
		servo_state = 1;
	}
	else if (OCR1A < 1500) {
		servo_state = 0;
	}
	if(!motor_on)
	servo_on = 0;
	if (motor_on && servo_on) {
		if (!servo_state) {
			OCR1A += 1;
			_delay_ms(1);
		}
		else if (servo_state) {
			OCR1A -= 1;
			_delay_ms(1);
		}
	}
}

// LED control
void LED() {
	if(motor_on){
		// Step 1
		if(OCR0 == LOW){
			PORTC |= (1 << LED_LOW);
			PORTC &= ~((1 << LED_MED) | (1 << LED_HIGH));
		}
		// Step 2
		if(OCR0 == MEDIUM){
			PORTC |= (1 << LED_MED) | (1 << LED_LOW);
			PORTC &= ~(1 << LED_HIGH);
		}
		// Step 3
		if(OCR0 == HIGH){
			PORTC |= (1 << LED_HIGH) | (1 << LED_MED) | (1 << LED_LOW);
		}
	}
	else {
		// Turn off LEDs if DC motor is off
		PORTC = 0x00;
	}
}

ISR(INT0_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Power))) {
		power_on(); // Toggle DC motor on/off
	UART_button_on_off++;
				
	if (UART_button_on_off == 1 && (OCR0 == LOW))
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
	}
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

ISR(INT2_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Spin))) {
		if(motor_on)
		servo_on = !servo_on; // Toggle servo motor on/off
	}
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
=======
>>>>>>> parent of c859f9c (first_uart)
