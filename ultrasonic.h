#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void timerInit_sonic(void);
void triggerPin(void);
uint8_t meanDistance(void);

//void ultrasonic_init(); 


#endif /* ULTRASONIC_H_ */
