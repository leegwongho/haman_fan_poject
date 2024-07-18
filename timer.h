/*
 * timer.h
 *
 * Created: 2024-07-18 오후 4:38:12
 *  Author: Stardust
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void shut_down(void);

extern volatile uint16_t timer;
extern volatile uint8_t timer_on;

#endif /* TIMER_H_ */