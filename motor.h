/*
 * IncFile1.h
 *
 * Created: 2024-07-17 오전 10:23:10
 *  Author: Stardust
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Power PORTD0
#define Speed PORTD1
#define Spin PORTD2
#define Fan PORTB4
#define Servo PORTB5
#define LED_LOW PORTC0
#define LED_MED PORTC1
#define LED_HIGH PORTC2

#define LOW     128
#define MEDIUM  192
#define HIGH    255
#define OFF     0

void gpioInit(void);
void timerInit(void);
void initInterrupts(void);
void power_on(void);
void speed_up(void);
void rotate_servo(void);
void pause(void);

extern volatile uint8_t motor_on;
extern volatile uint8_t speed;
extern volatile uint8_t servo_on;
extern volatile uint8_t servo_state;
extern volatile uint8_t distance;

extern volatile uint8_t motor_on;
extern volatile uint8_t count;


#endif /* INCFILE1_H_ */