#include "motor.h"

int main(void) {
	// Initialization
	gpioInit();
	timerInit();
	initInterrupts();
	
	// Initialize servo motor to center position
	OCR1A = 3000;
	
	while (1) {
		// Rotate servo motor
		rotate_servo();
		// Light up LEDs based on DC motor speed
		LED();
	}
}