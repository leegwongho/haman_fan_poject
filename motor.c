#include "motor.h"
#include "ultrasonic.h"

volatile uint8_t motor_on = 0;
volatile uint8_t speed = 0;
volatile uint8_t servo_on = 0;
volatile uint8_t servo_state = 0;
volatile uint8_t distance = 0;
volatile uint8_t count0, count1 = 0;

ISR(TIMER2_OVF_vect){
	if(count0 < 7)
	count0 += 1;
	else
	count0 = 0;
	rotate_servo();
	if(count1 < 31)
	count1 += 1;
	else{
		count1 = 0;
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
	}
	else {
		OCR0 = OFF;
		PORTC = 0x00;
	}
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
			break;
			case 1:
			OCR0 = MEDIUM;
			PORTC |= (1 << LED_MED) | (1 << LED_LOW);
			PORTC &= ~(1 << LED_HIGH);
			break;
			case 2:
			OCR0 = HIGH;
			PORTC |= (1 << LED_HIGH) | (1 << LED_MED) | (1 << LED_LOW);
			break;
		}
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
	}
}

ISR(INT1_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Speed))) {
		speed_up(); // Increase speed
	}
}

ISR(INT2_vect) {
	_delay_ms(10); // Debouncing
	if (!(PIND & (1 << Spin))) {
		if(motor_on)
		servo_on = !servo_on; // Toggle servo motor on/off
	}
}