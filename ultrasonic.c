#include "ultrasonic.h"

void timerInit_sonic() {
	// Set prescaler to 64
	TCCR3B |= (1 << CS31) | (1 << CS30);
}

void triggerPin() {
	// Output pulse on trigger pin
	PORTD &= ~(1 << PORTD6);  // Trigger pin LOW
	_delay_us(2);
	PORTD |= (1 << PORTD6);   // Trigger pin HIGH, hold for 10us
	_delay_us(10);
	PORTD &= ~(1 << PORTD6);  // Trigger pin LOW
}

uint8_t meanDistance() {
	TCNT3 = 0;
	while (!(PIND & (1 << PORTD7))) {  // Wait for ECHO pin to go HIGH
		if (TCNT3 > 65000) {
			return 0;
		}
	}

	TCNT3 = 0;
	while (PIND & (1 << PORTD7)) {  // Wait for ECHO pin to go LOW
		if (TCNT3 > 65000) {
			TCNT3 = 0;
			break;
		}
	}

	// Convert pulse width to distance
	double pulseWidth = 1000000.0 * TCNT3 * 64 / 16000000;
	return (uint8_t)(pulseWidth / 58);
}
/*
void ultrasonic_init() {
	

}
*/